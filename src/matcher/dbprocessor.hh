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

#include <iostream>
#include <cstdlib>
#include <ctime>

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

int64_t findParentFirm(context &ctx, int32_t idx) 
{
    auto &db = ctx.imdb.getTable<FirmLookup>();
    auto rec = db.getObject(idx);
    if (rec != nullptr) 
        return rec->getParentFirm();
    return -1;
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
    //New COs not yet executed cannot be looked up this way
    //There will be atmost 1 order per inviteid per side. 
    //It can be either conditional or Firmup or firm
    auto &db = ctx.imdb.getTable<OrderLookup>();    
    OrderLookup key;
    key.setInviteId(invite);
    key.setSide(side);
    return db.findByUniqueKey("ConditionalIndex", &key);
}

inline int64_t getTradePrice(context &ctx, const int32_t &symidx) {
    auto sym = ctx.imdb.getTable<SymbolLookup>().getObject(symidx);
    int64_t midpx = (sym->getNBBOBidPx() + sym->getNBBOAskPx()) / 2;
    return midpx;
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
    auto &orddb = ctx.imdb.getTable<OrderLookup>();
    auto ord = orddb.getObject(lord);
    auto sym = ctx.imdb.getTable<SymbolLookup>().getObject(ord->getSymbolIdx());
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
    //FILL,REPLACED,UROUT,NOTHING_DONE
    if (evt == OrderEventType_t::FILL || 
        evt == OrderEventType_t::INVITE || 
        evt == OrderEventType_t::REPLACED ||
        evt == OrderEventType_t::UROUT || 
        evt == OrderEventType_t::NOTHING_DONE) 
    {
        int64_t bbopx = 0;
        if (ord->getSide()() == Side_t::BUY) 
            bbopx = (ord->getClientType()() == ClientType_t::INVESTOR)?
                sym->getINBidPx():sym->getRPBidPx(); 
        else
            bbopx = (ord->getClientType()() == ClientType_t::INVESTOR)?
                sym->getINAskPx():sym->getRPAskPx(); 
        if (ord->getPrice() != bbopx) return evtobj;
        OrderLookup key;
        key.setSymbolIdx(ord->getSymbolIdx());
        key.setSide(ord->getSide()());
        key.setClientType(ord->getClientType());
        bbopx = (ord->getSide()() == Side_t::BUY)? exchange::MIN_PRICE:exchange::MAX_PRICE;
        DomainTable<OrderLookup>::IndexIterator itrS, itrE;
        if (  orddb.begin(itrS,"BookIndex",&key) && orddb.end(itrE,"BookIndex",&key))
            while (itrS != itrE) {
                auto ord2 = *(itrS);
                if (ord2->getOrderState()() != OrderState_t::ACTIVE) 
                    continue;
                if (ord2->getLeavesQty() <= 0) continue; 
                if (ord2->getSide()() == Side_t::BUY) 
                    bbopx = max(bbopx,ord2->getPrice());
                else
                    bbopx = min(bbopx,ord2->getPrice());
                ++itrS;
            }
        if (ord->getSide()() == Side_t::BUY && ord->getClientType()() == ClientType_t::INVESTOR)
            sym->setINBidPx(bbopx);
        if (ord->getSide()() == Side_t::BUY && ord->getClientType()() == ClientType_t::RISK_PROVIDER)
            sym->setRPBidPx(bbopx);
        if (ord->getSide()() == Side_t::SELL && ord->getClientType()() == ClientType_t::INVESTOR)
            sym->setINAskPx(bbopx);
        if (ord->getSide()() == Side_t::SELL && ord->getClientType()() == ClientType_t::RISK_PROVIDER)
            sym->setRPAskPx(bbopx);
    }
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

inline bool isPRFProvider(context &ctx, OrderLookup &ord, int32_t riskprovider) {
    int64_t prf = 0;
    if (ord.getSide()() != Side_t::SELL) 
        prf = readUserConfig(ctx, ConfigName_t::SID_PRF_ASK_RP, ord.getPartySubId(), ord.getSymbolIdx());
    else 
        prf = readUserConfig(ctx, ConfigName_t::SID_PRF_BID_RP, ord.getPartySubId(), ord.getSymbolIdx());
    auto &db = ctx.imdb.getTable<FirmLookup>();
    auto rec = db.getObject(riskprovider);
    if (rec == nullptr) return false;
    return rec->getFirmId() == prf;
}

TimerEvent * createTimerEvent(context &ctx, TimerEventType_t evt, int32_t lord, int64_t inviteid) 
{
    auto &db = ctx.imdb.getTable<TimerEvent>();
    auto &orddb = ctx.imdb.getTable<OrderLookup>();
    auto ord = orddb.getObject(lord);
    auto evtobj = db.createObject();
    evtobj->setEventType(evt);
    evtobj->setInviteID(inviteid);
    evtobj->setOrderID(ord->getOrderId());
    evtobj->setSymbolIdx(ord->getSymbolIdx());
    evtobj->setSubIDIdx(ord->getPartySubId());
    evtobj->setPrfMpidIdx(0);
    evtobj->setSide(ord->getSide());
    Timestamp ts;
    ts.set();
    int64_t wait = 0;
    if (evt == TimerEventType_t::CO_WAIT) {
        wait = readSystemConfig(ctx, ConfigName_t::GBL_CO_WAIT); 
    } else if (evt == TimerEventType_t::PRF_TIMER) {
        wait = readSystemConfig(ctx, ConfigName_t::SYM_PRF_WAIT, ord->getSymbolIdx()); 
    } else {
        std::srand(ts());
        wait = readSystemConfig(ctx, ConfigName_t::SYM_MRI_RANDOM_WAIT, ord->getSymbolIdx());
        wait = (wait * std::rand())/RAND_MAX;
        wait += readSystemConfig(ctx, ConfigName_t::SYM_MRI_FIXED_WAIT, ord->getSymbolIdx()); 
    }
    evtobj->setEventTime(Timestamp(ts()+wait));
    return evtobj;
}

#endif

