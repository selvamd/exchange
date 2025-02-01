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
#include <cstring> 

typedef void (*HandlerFunction)(context &ctx);

HandlerFunction handler[15];

void sendResponses(context &ctx, ux_selector *server) 
{
    //server->Writeint iClientID, char * Msg, int MsgLen);
    if (ctx.request()->api_msg_type == exchange::ExchangeApiMsgType_t::gen_msg || 
        ctx.request()->api_msg_type == exchange::ExchangeApiMsgType_t::config_msg)
        return;
    auto &db = ctx.imdb.getTable<OrderEvent>();
    DomainTable<OrderEvent>::IndexIterator itrS, itrE;
    if (db.begin(itrS,"PrimaryKey") && db.end(itrE,"PrimaryKey")) {
            while (itrS != itrE) {
                prepareExecReport(ctx, db.getObjectID(*(itrS)));
                auto iClientID = 0; //m_response.exec_report_msg.getTargetCompId();
                server->Write(iClientID, (char *)ctx.response(), sizeof(ctx.response()->exec_report_msg));
                ++itrS;
            }
    }
}

void processNewOrder(context &ctx) {
    //std::cout << ClientId << "," << "processNewOrder" << std::endl;
}

void processGenMsg(context &ctx) {
    //std::cout << ClientId << "," << "genmsg" << std::endl;
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
    //Various msg types: genmsg(login,heartbeat),config, fix, nbbo, timer  
    handler[(int)exchange::ExchangeApiMsgType_t::gen_msg]   = processGenMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::new_order] = processNewOrder;
    handler[(int)exchange::ExchangeApiMsgType_t::timer_msg] = processTimerMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::config_msg] = processConfigMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::nbbo_msg] = processNBBOMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::replace_order] = processReplaceOrder;
    handler[(int)exchange::ExchangeApiMsgType_t::cancel] = processCancelOrder;
}


#endif
