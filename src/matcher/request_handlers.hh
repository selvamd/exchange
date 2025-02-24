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
        // TODO: generate response
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
    auto &orddb = ctx.imdb.getTable<OrderLookup>();
    auto res = validate_new_order(ctx, ord);
    if (res != "") {
        createOrderReject(ctx, ord, res);
        return;
    }
    if (ord.getOrdTypeExt()() == OrdTypeExt_t::FIRMUP) 
    {
        auto cord = findConditionalOrder(ctx, ord.getInviteId(), ord.getSide()());
        if (cord == nullptr || cord->getOrdTypeExt()() != OrdTypeExt_t::CONDITIONAL) {
            createOrderReject(ctx, ord, "No conditional order");
            return;
        }
        orddb.removeObject(cord->d_row);
    }
    auto rec = orddb.copyObject(&ord);
    createOrderEvent(ctx, rec->d_row, OrderEventType_t::ACK);
    if (ord.getOrderState()() == OrderState_t::MRI_WAIT) {
        createTimerEvent(ctx, TimerEventType_t::MRI_WAIT, rec->d_row, 0);
        return;
    } else if (ord.getOrdTypeExt()() == OrdTypeExt_t::FIRMUP) {
        auto contra = (ord.getSide()() == Side_t::BUY) ? Side_t::SELL : Side_t::BUY;
        auto cord = findConditionalOrder(ctx, ord.getInviteId(), contra);
        if (cord != nullptr) {
            if (cord->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL) 
                rec->setOrderState(OrderState_t::CO_WAIT);
            else
                cord->setOrderState(OrderState_t::ACTIVE);
        }
    } 
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
    auto req = ctx.request()->timer_msg;
    if (req.getTimerEventType() == exchange::TimerEventType_t::MRI_WAIT) {
        auto ord = findOrder(ctx,req.getOrderId());
        if (ord == nullptr) return;
        ord->setOrderState(OrderState_t::ACTIVE);
        updatebbo(ctx, ord->getSymbolIdx(), ord->d_row); 
        workOrder(ctx, ord->d_row);
    } 
    else if (req.getTimerEventType() == exchange::TimerEventType_t::CO_WAIT) 
    {
        auto bord = findConditionalOrder(ctx, req.getInviteId(), Side_t::BUY);
        if (bord != nullptr && bord->getOrdTypeExt()() != OrdTypeExt_t::FIRM) {
            //one last sweep b4 cxl
            workOrder(ctx, bord->d_row); 
            if (bord->getLeavesQty() > 0) {
                bord->setOrdStatus(OrdStatus_t::CANCELED);
                createOrderEvent(ctx, bord->d_row, OrderEventType_t::UROUT);
            }
        }
        auto sord = findConditionalOrder(ctx, req.getInviteId(), Side_t::SELL);
        if (sord != nullptr && bord->getOrdTypeExt()() != OrdTypeExt_t::FIRM) {
            //one last sweep b4 cxl
            workOrder(ctx, sord->d_row);
            if (sord->getLeavesQty() > 0) {
                sord->setOrdStatus(OrdStatus_t::CANCELED);
                createOrderEvent(ctx, sord->d_row, OrderEventType_t::UROUT);
            }
        }
    } 
    else if (req.getTimerEventType() == exchange::TimerEventType_t::PRF_TIMER)
    {
        //Process PRF Timeout
        auto symidx = req.getSymbolId();
        auto subidx = req.getSubId();
        auto prfid  = req.getInviteId();
        if (req.getSide() == exchange::Side_t::Buy) {
            auto id = readUserConfig(ctx, ConfigName_t::SID_PRF_BID_ID, subidx, symidx);
            if (id != prfid) return;
            saveUserConfig(ctx, ConfigName_t::SID_PRF_BID_RP, subidx, symidx, 0);    
        } else {
            auto id = readUserConfig(ctx, ConfigName_t::SID_PRF_ASK_ID, subidx, symidx);
            if (id != prfid) return;
            saveUserConfig(ctx, ConfigName_t::SID_PRF_ASK_RP, subidx, symidx, 0);    
        }
    }
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

inline void executeTrade(context &ctx, int32_t lord1, int32_t lord2) {
    auto &orddb = ctx.imdb.getTable<OrderLookup>();
    auto ord1 = orddb.getObject(lord1);
    auto ord2 = orddb.getObject(lord1);
    auto trdqty = min(ord2->getLeavesQty(),ord2->getLeavesQty());
    if (trdqty <= 0) return;
    auto trdpx = getTradePrice(ctx,ord1->getSymbolIdx());
    if (ord1->getSide()() == Side_t::BUY) {
        if (!is_match_eligible(ctx,ord1->d_row,ord2->d_row))
            return;
    } else {
        if (!is_match_eligible(ctx,ord2->d_row,ord1->d_row))
            return;
    }
    if (ord1->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL || 
        ord2->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL) 
    {
        auto inviteid = ctx.getInviteId();
        ord1->setInviteId(inviteid);
        ord2->setInviteId(inviteid);
        ord1->setOrderState(OrderState_t::CO_WAIT);
        ord2->setOrderState(OrderState_t::CO_WAIT);
        if (ord1->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL)
            createOrderEvent(ctx, ord1->d_row, OrderEventType_t::INVITE);
        if (ord2->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL)
            createOrderEvent(ctx, ord2->d_row, OrderEventType_t::INVITE);
        createTimerEvent(ctx, TimerEventType_t::CO_WAIT, ord1->d_row, inviteid); 
    } else {
        createOrderFill(ctx, ord1->d_row, trdpx, trdqty);
        createOrderFill(ctx, ord2->d_row, trdpx, trdqty);
        //save previous risk fill data and create Timer event
        if (ord1->getClientType()() != ord2->getClientType()()) {
            auto evtid = ctx.getNextExecId();
            if (ord1->getClientType()() == ClientType_t::INVESTOR) {
                auto cfgid = (ord1->getSide()() == Side_t::BUY)? ConfigName_t::SID_PRF_BID_ID : ConfigName_t::SID_PRF_ASK_ID;
                auto cfgrp = (ord1->getSide()() == Side_t::BUY)? ConfigName_t::SID_PRF_BID_RP : ConfigName_t::SID_PRF_ASK_RP;
                saveUserConfig(ctx, cfgid, ord1->getPartySubId(), ord1->getSymbolIdx(), evtid);
                saveUserConfig(ctx, cfgrp, ord1->getPartySubId(), ord1->getSymbolIdx(), ord2->getPartyId());
                createTimerEvent(ctx, TimerEventType_t::PRF_TIMER, ord1->d_row, evtid);
            } else {
                auto cfgid = (ord2->getSide()() == Side_t::BUY)? ConfigName_t::SID_PRF_BID_ID : ConfigName_t::SID_PRF_ASK_ID;
                auto cfgrp = (ord2->getSide()() == Side_t::BUY)? ConfigName_t::SID_PRF_BID_RP : ConfigName_t::SID_PRF_ASK_RP;
                saveUserConfig(ctx, cfgid, ord2->getPartySubId(), ord2->getSymbolIdx(), evtid);
                saveUserConfig(ctx, cfgrp, ord2->getPartySubId(), ord2->getSymbolIdx(), ord1->getPartyId());
                createTimerEvent(ctx, TimerEventType_t::PRF_TIMER, ord2->d_row, evtid);
            }
        }
    }
}

inline void tradeInvestorUrgency(context &ctx, OrderLookup &ord, 
    OrderUrgency_t urg, vector<int32_t> &vec) {
    if (ord.getLeavesQty() == 0) return;
    auto &orddb = ctx.imdb.getTable<OrderLookup>();
    //broker priority
    for (auto it = vec.begin(); it != vec.end();) {
        auto ordbk = orddb.getObject(*it);
        if (ordbk->getOrderUrgency()() != urg && ord.getLeavesQty() > 0 &&  
            findParentFirm(ctx,ord.getPartyId()) == findParentFirm(ctx,ordbk->getPartyId())) 
        {
            executeTrade(ctx,ord.d_row,ordbk->d_row);
            it = vec.erase(it);
        } else ++it;
    }
    //same urgency non-brokers
    for (auto it = vec.begin(); it != vec.end();) {
        auto ordbk = orddb.getObject(*it);
        if (ordbk->getOrderUrgency()() != urg && ord.getLeavesQty() > 0) 
        {
            executeTrade(ctx,ord.d_row,ordbk->d_row);
            it = vec.erase(it);
        } else ++it;
    }
} 

inline void tradeRiskTier(context &ctx, OrderLookup &ord, 
    RiskTier_t tier, vector<int32_t> &vec) {
    if (ord.getLeavesQty() == 0) return;
    auto &orddb = ctx.imdb.getTable<OrderLookup>();

    //broker priority
    for (auto it = vec.begin(); it != vec.end();) {
        auto ordbk = orddb.getObject(*it);
        if (ordbk->getRiskTier()() == tier && ord.getLeavesQty() > 0 && 
            findParentFirm(ctx,ord.getPartyId()) == findParentFirm(ctx,ordbk->getPartyId())) 
        {
            executeTrade(ctx,ord.d_row,ordbk->d_row);
            it = vec.erase(it);
        } else ++it;
    }
    //Previous Risk Fill
    for (auto it = vec.begin(); it != vec.end();) {
        auto ordbk = orddb.getObject(*it);
        if (ordbk->getRiskTier()() == tier && ord.getLeavesQty() > 0 && 
            isPRFProvider(ctx,ord,ordbk->getPartyId())) 
        {
            executeTrade(ctx,ord.d_row,ordbk->d_row);
            it = vec.erase(it);
        } else ++it;
    }
    //same urgency non-brokers
    for (auto it = vec.begin(); it != vec.end();) {
        auto ordbk = orddb.getObject(*it);
        if (ordbk->getRiskTier()() == tier && ord.getLeavesQty() > 0) 
        {
            executeTrade(ctx,ord.d_row,ordbk->d_row);
            it = vec.erase(it);
        } else ++it;
    }
 } 

void tradeInvestor(context &ctx, OrderLookup &ord, SymbolLookup &sym) 
{
    OrderLookup key;
    key.setSymbolIdx(ord.getSymbolIdx());
    key.setSide((ord.getSide()() == Side_t::BUY)? Side_t::SELL:Side_t::BUY);
    key.setClientType(ClientType_t::INVESTOR);
    key.defOrderUrgency();
    key.defContraCategory();
    key.defRiskTier();
    key.defOrderQty();
    key.defRankTime();
    std::vector<int32_t> vec;
    auto &orddb = ctx.imdb.getTable<OrderLookup>();
    DomainTable<OrderLookup>::IndexIterator itrS, itrE;
    if (  orddb.begin(itrS,"BookIndex",&key) && orddb.end(itrE,"BookIndex",&key))
        while (itrS != itrE) {
            vec.push_back((*(itrS))->d_row);
            ++itrS;
        }
    tradeInvestorUrgency(ctx,ord,OrderUrgency_t::HIGH,vec);
    tradeInvestorUrgency(ctx,ord,OrderUrgency_t::MEDIUM,vec);
    tradeInvestorUrgency(ctx,ord,OrderUrgency_t::LOW,vec);
}

void tradeRiskProvider(context &ctx, OrderLookup &ord, SymbolLookup &sym) 
{
    OrderLookup key;
    key.setSymbolIdx(ord.getSymbolIdx());
    key.setSide((ord.getSide()() == Side_t::BUY)? Side_t::SELL:Side_t::BUY);
    key.setClientType(ClientType_t::RISK_PROVIDER);
    key.defOrderUrgency();
    key.defContraCategory();
    key.defRiskTier();
    key.defOrderQty();
    key.defRankTime();
    std::vector<int32_t> vec;
    auto &orddb = ctx.imdb.getTable<OrderLookup>();
    DomainTable<OrderLookup>::IndexIterator itrS, itrE;
    if (  orddb.begin(itrS,"BookIndex",&key) && orddb.end(itrE,"BookIndex",&key))
        while (itrS != itrE) {
            vec.push_back((*(itrS))->d_row);
            ++itrS;
        }
    if (ord.getClientType()() == ClientType_t::INVESTOR) {
        // Do risk transfer with Broker Pref and PRF
        tradeRiskTier(ctx,ord,RiskTier_t::PLATINUM,vec);
        tradeRiskTier(ctx,ord,RiskTier_t::GOLD,vec);
        tradeRiskTier(ctx,ord,RiskTier_t::SILVER,vec);
    } else {
        // Do risk offset
        for (auto rec : vec) {
            auto ordbk = orddb.getObject(rec);
            executeTrade(ctx,ord.d_row,ordbk->d_row);
        }   
    }
}

void workOrder(context &ctx, int32_t lord) 
{
    auto ord = ctx.imdb.getTable<OrderLookup>().getObject(lord);
    auto sym = ctx.imdb.getTable<SymbolLookup>().getObject(ord->getSymbolIdx());
    auto midpx = getTradePrice(ctx, ord->getSymbolIdx());
    if (ord->getSide()() == Side_t::BUY) {
        if (ord->getPrice() < midpx) return;

        if (sym->getINAskPx() <= midpx) 
            tradeInvestor(ctx, *ord, *sym); 

        if (ord->getLeavesQty() > 0 && 
            sym->getRPAskPx() <= midpx)
            tradeRiskProvider(ctx, *ord, *sym); 
    } else {
        if (ord->getPrice() > midpx) return;

        if (sym->getINBidPx() >= midpx)
            tradeInvestor(ctx, *ord, *sym);
        
        if (ord->getLeavesQty() > 0 &&
            sym->getRPBidPx() >= midpx)
            tradeRiskProvider(ctx, *ord, *sym);
    }
}
#endif
