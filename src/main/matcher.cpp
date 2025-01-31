#include <fstream>
#include <ostream>
#include <iostream>
#include <deque>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
//#include "ServiceDBInit.hh"
#include "context.hpp"
#include "ux_selector.hh"
#include "api_SocketStreamer.hh"
#include <csignal>
#include <cstring>

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);  
}

ux_selector *server = nullptr;
typedef void (*HandlerFunction)(const context &ctx);
HandlerFunction handler[15];

void processNewOrder(const context &ctx) {
    //std::cout << ClientId << "," << "processNewOrder" << std::endl;
}

// void processGenMsg(const int &ClientId, exchange_api::ExchangeApiUnion *req, exchange_api::ExchangeApiUnion *res) {
//     std::cout << ClientId << "," << "genmsg" << std::endl;
// }

void initHandler()
{
    //handler[(int)exchange::ExchangeApiMsgType_t::gen_msg]   = processGenMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::new_order] = processNewOrder;
}

int processRequests(context& ctx)
{
    int iRC, iServerPort, iClientFD, iClientID;
    auto inmsg = ctx.request();
    auto outmsg = ctx.response();
    server->PollForSocketEvent();
    while (true)
    {
        iRC = server->Accept(iServerPort, iClientFD);
        if ( iRC == ux_selector::END_OF_SOCK_LIST) break;
        if ( iRC == ux_selector::WOULD_BLOCK) continue;
        SocketStreamerBase * streamer = new api_SocketStreamer(iClientFD, 4096);
        iClientID = server->AddClient(streamer, 4096, 4, false);
        std::cout << "ExchSvc accepted a connection " << iClientID << std::endl;
    }
    
    while (true)
    {
        memset((void *)inmsg, 0, sizeof(inmsg));
        memset((void *)outmsg, 0, sizeof(exchange_api::ExchangeApiUnion));
        iRC = server->Read(iClientID, (char *)inmsg);
        if ( iRC == ux_selector::SUCCESS )
        {
            inmsg->ntoh();
            //Read and process the msg
            auto evtreq = inmsg->api_msg_type;
            handler[(int)evtreq](ctx);
        }
        else
        {
            return (iRC == ux_selector::END_OF_SOCK_LIST)? 0:-1;
        }
        ctx.imdb.commit(); 
        ctx.imdb.getTable<OrderEvent>().reset();
    }
    return 0;
}

int main(int , char ** )
{
    signal(SIGINT, signalHandler);  

    initHandler();
    context ctx = context();
    ctx.initdb();
    //initdb<Service_t::MATCHER>(ctx);

    server = new ux_selector(20);
    server->AddServer(65000);

    while (true)
    {
        int rc = processRequests(ctx);
        if (rc == -2) break;
    }

    ctx.closedb();
    //closedb<Service_t::MATCHER>();
}
