#ifndef DBPROCESSOR_H
#define DBPROCESSOR_H
#include "DataTypes.hpp"
#include "DomainDatabase.hh"
#include "DomainDBListeners.hh"
#include "Enums.hh"
#include "MatcherDomainObjects.hh"
#include "ExchangeApi.hpp"
#include <cstring> 
#include "context.hpp"
#include <math.h>
#include <stdlib.h>
using namespace std;

SymbolLookup * findSymbol(context &ctx, const char * csym, bool create = false) 
{
    SymbolLookup key;
    key.setName(csym);
    auto &db = ctx.imdb.getTable<SymbolLookup>();
    auto obj = db.findByPrimaryKey(&key);
    if (obj == nullptr && create) {
        obj = db.createObject(); 
        obj->setName(csym);
    }
    return obj;
}

FirmLookup * findFirmByName(context &ctx, FirmRecordType_t type, const char * cname) {
    FirmLookup key;
    key.setFirmRecordType(type);
    key.setName(cname);
    auto &db = ctx.imdb.getTable<FirmLookup>();
    return db.findByUniqueKey("NameKey", &key);
}

FirmLookup * findFirm(context &ctx, int64_t firmid, bool create = false) 
{
    FirmLookup key;
    key.setFirmId(firmid);
    auto &db = ctx.imdb.getTable<FirmLookup>();
    auto obj = db.findByPrimaryKey(&key);
    if (obj == nullptr && create) {
        obj = db.createObject();
        obj->setFirmId(firmid);
    }
    return obj;
}

OrderLookup * findOrder(context &ctx, int64_t ordid, bool create = false) 
{
    OrderLookup key;
    key.setOrderId(ordid);
    auto &db = ctx.imdb.getTable<OrderLookup>();
    auto obj = db.findByPrimaryKey(&key);
    if (obj == nullptr && create) {
        obj = db.createObject();
        obj->setOrderId(ordid);
    }
    return obj;
}

OrderLookup * findConditionalOrder(context &ctx, int64_t invite, Side_t side) 
{
    auto &db = ctx.imdb.getTable<OrderLookup>();    
    OrderLookup key;
    key.setInviteId(invite);
    key.setSide(side);
    return db.findByUniqueKey("ConditionalIndex", &key);
}


void updatebbo(context &ctx, int32_t symbol, int32_t lord) {
    auto sym = ctx.imdb.getTable<SymbolLookup>().getObject(symbol);
    auto ord = ctx.imdb.getTable<OrderLookup>().getObject(lord);
    if (sym != nullptr && ord != nullptr) {
        if (ord->getSide()() == Side_t::BUY) {
            if (ord->getClientType()() == ClientType_t::INVESTOR) 
                sym->setINBidPx(max(sym->getINBidPx(), ord->getPrice()));
            else
                sym->setRPBidPx(max(sym->getRPBidPx(), ord->getPrice()));
        } else {
            if (ord->getClientType()() == ClientType_t::INVESTOR) 
                sym->setINAskPx(min(sym->getINAskPx(), ord->getPrice()));
            else
                sym->setRPAskPx(min(sym->getRPAskPx(), ord->getPrice()));
        }
    }
}

OrderEvent * createOrderEvent(context &ctx, int32_t lord, OrderEventType_t evt) 
{
    auto &db = ctx.imdb.getTable<OrderEvent>();
    auto ord = ctx.imdb.getTable<OrderLookup>().getObject(lord);
    auto evtobj = db.createObject();
    evtobj->setOrdIdx(lord);
    evtobj->setTradePrice(0);
    evtobj->setTradeQty(0);
    evtobj->setPostFillCumQty(ord->getCumQty());
    evtobj->setPostFillLeavesQty(ord->getLeavesQty());
    evtobj->setAvgPrice(ord->getAvgPx());
    evtobj->setExecId(ctx.getNextExecId());
    evtobj->setOrdStatus(ord->getOrdStatus());
    evtobj->setEventType(evt);
    return evtobj;
}

OrderEvent * createOrderFill(context &ctx, int32_t lord, int64_t price, int32_t qty) 
{
    auto ord = ctx.imdb.getTable<OrderLookup>().getObject(lord);
    int32_t cum = ord->getCumQty() + qty;
    int64_t avgpx = (ord->getCumQty() * ord->getAvgPx() + price * qty)/cum;
    int32_t leaves = ord->getOrderQty() - cum;
    ord->setCumQty(cum);
    ord->setLeavesQty(leaves);
    ord->setAvgPx(avgpx);
    ord->setOrdStatus(OrdStatus_t::PARTIAL);
    auto evttype = OrderEventType_t::PARTIAL_FILL;
    if (leaves == 0) evttype = OrderEventType_t::FILL;
    if (leaves == 0) ord->setOrdStatus(OrdStatus_t::FILLED);
    auto evt = createOrderEvent(ctx, lord, evttype);
    evt->setTradePrice(price);
    evt->setTradeQty(qty);
    return evt;
}

FirmLookup * createSubID(context &ctx, const char * cname, int32_t mpid, int32_t firmid) 
{
    auto &db = ctx.imdb.getTable<FirmLookup>();
    auto firm = ctx.createFirmIntraday();
    firm->setFirmRecordType(FirmRecordType_t::SUBID);
    firm->setName(cname);
    firm->setParentMPID(db.getObject(mpid)->getFirmId());
    firm->setParentFirm(db.getObject(firmid)->getFirmId());
    return firm;
}

//if replace = false, will only create if not present
void writeConfigRec(context &ctx, ConfigName_t config, int64_t value, int32_t firm, int32_t symbol, bool replace = true) 
{
    auto &db = ctx.imdb.getTable<ConfigLookup>();
    ConfigLookup key;
    key.setConfigName(config);
    key.setSymIdx(symbol);
    key.setFirmIdx(firm);
    key.setFirmId(0);
    key.setSymbol("");
    auto obj = db.findByPrimaryKey(&key);
    if (obj == nullptr) 
    {
        obj = db.copyObject(&key);
        if (firm > 0) 
            obj->setFirmId(ctx.imdb.getTable<FirmLookup>().getObject(firm)->getFirmId());
        if (symbol > 0)
            obj->setSymbol(ctx.imdb.getTable<SymbolLookup>().getObject(firm)->getName());
        obj->setConfigValue(value);
    }
    if (replace) obj->setConfigValue(value);
}

void saveNewUserConfig(context &ctx, ConfigName_t config, int32_t firm, int32_t symbol, int64_t value) 
{
    writeConfigRec(ctx, config, value, firm, symbol, false);
}
void saveUserConfig(context &ctx, ConfigName_t config, int32_t firm, int32_t symbol, int64_t value) 
{
    writeConfigRec(ctx, config, value, firm, symbol);
}

void saveSymbolConfig(context &ctx, int32_t symbol, ConfigName_t config, int64_t value)
{
    writeConfigRec(ctx, config, value, 0, symbol);
}

void saveGblConfig(context &ctx, ConfigName_t config, int64_t value) 
{
    writeConfigRec(ctx, config, value, 0, 0);
}


int64_t readUserConfig(context &ctx, ConfigName_t config, int32_t firm, int32_t symbol = 0) 
{
    auto &db = ctx.imdb.getTable<ConfigLookup>();
    ConfigLookup key;
    key.setConfigName(config);
    key.setFirmIdx(firm);
    key.setSymIdx(symbol);
    auto obj = db.findByPrimaryKey(&key);
    if (obj != nullptr) return obj->getConfigValue();
    if (symbol > 0) {
        key.setSymIdx(0);
        obj = db.findByPrimaryKey(&key);
        if (obj != nullptr) 
            return obj->getConfigValue();
    }
    key.setFirmIdx(0);
    obj = db.findByPrimaryKey(&key);
    if (obj != nullptr) 
        return obj->getConfigValue();
    return -1; // user configs can be missing and indicated by -1
}

int64_t readSystemConfig(context &ctx, ConfigName_t config, int32_t symbol = 0) 
{
    auto &db = ctx.imdb.getTable<ConfigLookup>();
    ConfigLookup key;
    key.setConfigName(config);
    key.setFirmIdx(0);
    key.setSymIdx(symbol);
    auto obj = db.findByPrimaryKey(&key);
    if (obj != nullptr) return obj->getConfigValue(); 
    key.setSymIdx(0);
    obj = db.findByPrimaryKey(&key);
    if (obj != nullptr) return obj->getConfigValue();
    return 0; // System configs unlike user configs are not expected to be missing
}

#endif

