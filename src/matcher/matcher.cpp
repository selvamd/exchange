#include <fstream>
#include <ostream>
#include <iostream>
#include <deque>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include "context.hpp"
#include "request_handlers.hh"
#include "msg_generator.hh"
#include "dbprocessor.hh"
#include "validator.hh"
#include "ux_selector.hh"
#include "api_SocketStreamer.hh"
#include <csignal>
#include <cstring>

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);  
}

ux_selector *server = nullptr;

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
        memset((void *)outmsg, 0, sizeof(outmsg));
        //process timer event if present
        if (dequeTimerMsg(ctx)) {
            auto evtreq = inmsg->api_msg_type;
            handler[(int)evtreq](ctx);
            sendResponses(ctx,server);
            memset((void *)inmsg, 0, sizeof(inmsg));
            memset((void *)outmsg, 0, sizeof(outmsg));
            ctx.imdb.commit(); 
            ctx.imdb.getTable<OrderEvent>().reset();
        }
        iRC = server->Read(iClientID, (char *)inmsg);
        if ( iRC == ux_selector::SUCCESS )
        {
            inmsg->ntoh();
            //Read and process the msg
            auto evtreq = inmsg->api_msg_type;
            if (evtreq == exchange::ExchangeApiMsgType_t::gen_msg)
                inmsg->gen_msg.setClientId(iClientID);
            handler[(int)evtreq](ctx);
            sendResponses(ctx,server);
            ctx.imdb.commit(); 
            ctx.imdb.getTable<OrderEvent>().reset();
        }
        else
        {
            return (iRC == ux_selector::END_OF_SOCK_LIST)? 0:-1;
        }
    }
    return 0;
}

/*
TODOs
    1) Handling msgs: NBBO,CFO,CXL,Config
    2) Handling Order,Cxl rejects
*/
int main(int , char ** )
{
    signal(SIGINT, signalHandler);  

    initHandler();
    context ctx = context();
    ctx.initdb();

    server = new ux_selector(20);
    server->AddServer(65000);

    while (true)
    {
        int rc = processRequests(ctx);
        if (rc == -2) break;
    }

    ctx.closedb();
    delete server;
}
