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

#endif

