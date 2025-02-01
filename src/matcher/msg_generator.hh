#ifndef MSG_GENERATOR_H
#define MSG_GENERATOR_H
#include "DataTypes.hpp"
#include "DomainDatabase.hh"
#include "DomainDBListeners.hh"
#include "Enums.hh"
#include "MatcherDomainObjects.hh"
#include "ExchangeApi.hpp"
#include "context.hpp"
#include <cstring> 

void prepareExecReport(context &ctx, int32_t levt) 
{
    auto evt = ctx.imdb.getTable<OrderEvent>().getObject(levt);
    int32_t lord = evt->getOrdIdx();
    auto ord = ctx.imdb.getTable<OrderLookup>().getObject(lord);
    auto sym = ctx.imdb.getTable<SymbolLookup>().getObject(ord->getSymbolIdx());
    if (ord != nullptr && evt != nullptr && sym != nullptr) {
        auto rept = ctx.response()->exec_report_msg;
        rept.reset();
        rept.setClOrdId(ord->getClOrdId().c_str());
        rept.setExecId(evt->getExecId());
        rept.setOrderQty(ord->getOrderQty());
        rept.setOrdType(ord->getOrdType().toString());
        rept.setPrice(ord->getPrice());
        rept.setSide(ord->getSide().toString());
        rept.setSymbol(sym->getName().c_str());
        rept.setTimeInForce(ord->getTimeInForce().toString());
        rept.setCumQty(evt->getPostFillCumQty());
        rept.setLastPx(evt->getTradePrice());
        rept.setLastShares(evt->getTradeQty());
        //m_response.exec_report_msg.setLeavesQty(evt->getPostFillLeavesQty());
        //m_response.exec_report_msg.setExecType(ord->getExecType().toString());
        rept.setOrdStatus(evt->getOrdStatus().toString());
        // remove processed order from the book
        if (evt->getEventType()() == OrderEventType_t::FILL || 
            evt->getEventType()() == OrderEventType_t::UROUT || 
            evt->getEventType()() == OrderEventType_t::NOTHING_DONE)
            ctx.imdb.getTable<OrderLookup>().removeObject(lord);
    }
}


#endif
