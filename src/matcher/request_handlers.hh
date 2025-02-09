#ifndef REQUEST_HANDLERS_H
#define REQUEST_HANDLERS_H
#include "DataTypes.hpp"
#include "DomainDatabase.hh"
#include "DomainDBListeners.hh"
#include "Enums.hh"
#include "MatcherDomainObjects.hh"
#include "ExchangeApi.hpp"
#include "context.hpp"
#include "msg_generator.hh"
#include "validator.hh"
#include <cstring> 

void workOrder(context &ctx, int32_t lord);

typedef void (*HandlerFunction)(context &ctx);
HandlerFunction handler[15];

bool dequeTimerMsg(context &ctx) {
    Timestamp currts;
    currts.set();
    auto &db = ctx.imdb.getTable<TimerEvent>();    
    DomainTable<TimerEvent>::IndexIterator itrS, itrE;
    if (db.begin(itrS,"PrimaryKey") && db.end(itrE,"PrimaryKey")) {
            if (itrS != itrE) {
                auto evt = *(itrS);
                if (currts.compareTo(evt->getEventTime()) < 0) 
                    return false;
                auto req = ctx.request();
                req->timer_msg.reset();
                req->timer_msg.setTimerEventType(evt->getEventType().toString());
                req->timer_msg.setOrderId(evt->getOrderID());
                req->timer_msg.setSymbolId(evt->getSymbolIdx());
                req->timer_msg.setSubId(evt->getSubIDIdx());
                req->timer_msg.setRiskProviderId(evt->getPrfMpidIdx());
                req->timer_msg.setSide(evt->getSide().toString());
                req->timer_msg.setInviteId(evt->getInviteID());
                return true;
            }
    }
    return false;
}

void sendResponses(context &ctx, ux_selector *server) 
{
    //server->Writeint iClientID, char * Msg, int MsgLen);
    if (ctx.request()->api_msg_type == exchange::ExchangeApiMsgType_t::gen_msg || 
        ctx.request()->api_msg_type == exchange::ExchangeApiMsgType_t::config_msg)
        return;

    auto &firmdb = ctx.imdb.getTable<FirmLookup>(); 
    if (ctx.response()->api_msg_type != exchange::ExchangeApiMsgType_t::gen_msg) {
        // generate response
        //auto firm = firmdb.getObject(ctx.request()->new_order_msg.getSenderCompId());
        return;
    }

    auto &evtdb = ctx.imdb.getTable<OrderEvent>();
    auto &orddb = ctx.imdb.getTable<OrderLookup>(); 
    DomainTable<OrderEvent>::IndexIterator itrS, itrE;
    if (evtdb.begin(itrS,"PrimaryKey") && evtdb.end(itrE,"PrimaryKey")) {
            while (itrS != itrE) {
                auto ordevt = *(itrS);
                auto ord = orddb.getObject(ordevt->getOrdIdx());
                auto firm = firmdb.getObject(ord->getSenderCompId());
                prepareExecReport(ctx, evtdb.getObjectID(ordevt));
                server->Write(firm->getClientID(), (char *)ctx.response(), sizeof(ctx.response()->exec_report_msg));
                ++itrS;
            }
    }
}

void processNewOrder(context &ctx) {
    OrderLookup ord;
    auto res = validate_new_order(ctx, ord);
    if (res != "") {
        createOrderReject(ctx, ord, res);
    }
    auto rec = ctx.imdb.getTable<OrderLookup>().copyObject(&ord);
    createOrderEvent(ctx, rec->d_row, OrderEventType_t::ACK);
    updatebbo(ctx, ord.getSymbolIdx(), rec->d_row);
    workOrder(ctx, rec->d_row);
    //std::cout << ClientId << "," << "processNewOrder" << std::endl;
}

void processGenMsg(context &ctx) {
    auto req = ctx.request()->gen_msg;
    auto firm = findFirmByName(ctx, FirmRecordType_t::GATEWAY, req.getSenderCompId());
    if (firm != nullptr) firm->setClientID(req.getClientId());
}

void processTimerMsg(context &ctx) {
    //std::cout << ClientId << "," << "genmsg" << std::endl;
}

void processNBBOMsg(context &ctx) {
    //std::cout << ClientId << "," << "genmsg" << std::endl;
}

void processReplaceOrder(context &ctx) {
    //std::cout << ClientId << "," << "genmsg" << std::endl;
}

void processCancelOrder(context &ctx) {
    //std::cout << ClientId << "," << "genmsg" << std::endl;
}

void processConfigMsg(context &ctx) {
    //std::cout << ClientId << "," << "genmsg" << std::endl;
}

void initHandler()
{
    //Various msg types: genmsg(login),config, fix, nbbo, timer  
    handler[(int)exchange::ExchangeApiMsgType_t::gen_msg]   = processGenMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::new_order] = processNewOrder;
    handler[(int)exchange::ExchangeApiMsgType_t::timer_msg] = processTimerMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::config_msg] = processConfigMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::nbbo_msg] = processNBBOMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::replace_order] = processReplaceOrder;
    handler[(int)exchange::ExchangeApiMsgType_t::cancel] = processCancelOrder;
}

void tradeInvestor(context &ctx, OrderLookup &ord, SymbolLookup &sym) 
{
    if (ord.getClientType()() == ClientType_t::INVESTOR) {
        // Do natural cross

    } else {
        // Do risk transfer
    }
}

void tradeRiskProvider(context &ctx, OrderLookup &ord, SymbolLookup &sym) 
{
    if (ord.getClientType()() == ClientType_t::INVESTOR) {
        // Do risk transfer
    } else {
        // Do risk offset
    }
}

void workOrder(context &ctx, int32_t lord) 
{
    auto ord = ctx.imdb.getTable<OrderLookup>().getObject(lord);
    auto sym = ctx.imdb.getTable<SymbolLookup>().getObject(ord->getSymbolIdx());
    int64_t midpx = (sym->getNBBOBidPx() + sym->getNBBOAskPx()) / 2;
    if (ord->getSide()() == Side_t::BUY) {
        if (ord->getPrice() < midpx) return;

        if (ord->getPrice() >= sym->getINAskPx()) 
            tradeInvestor(ctx, *ord, *sym); 

        if (ord->getLeavesQty() > 0 && 
            ord->getPrice() >= sym->getRPAskPx())
            tradeRiskProvider(ctx, *ord, *sym); 
    } else {
        if (ord->getPrice() > midpx) return;

        if (ord->getPrice() <= sym->getINBidPx())
            tradeInvestor(ctx, *ord, *sym);
        
        if (ord->getLeavesQty() > 0 &&
            ord->getPrice() <= sym->getRPBidPx())
            tradeRiskProvider(ctx, *ord, *sym);
    }
}
#endif
