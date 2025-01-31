#ifndef REQUEST_HANDLERS_H
#define REQUEST_HANDLERS_H
#include "DataTypes.hpp"
#include "DomainDatabase.hh"
#include "DomainDBListeners.hh"
#include "Enums.hh"
#include "MatcherDomainObjects.hh"
#include "ExchangeApi.hpp"
#include "context.hpp"
#include <cstring> 

typedef void (*HandlerFunction)(const context &);

HandlerFunction handler[15];

void processNewOrder(const context &ctx) {
    //std::cout << ClientId << "," << "processNewOrder" << std::endl;
}

void processGenMsg(const context &ctx) {
    //std::cout << ClientId << "," << "genmsg" << std::endl;
}

void initHandler()
{
    //Various msg types: genmsg(login,heartbeat),config, fix, nbbo, timer  
    handler[(int)exchange::ExchangeApiMsgType_t::gen_msg]   = processGenMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::new_order] = processNewOrder;
}


#endif
