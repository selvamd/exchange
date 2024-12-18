#include <fstream>
#include <ostream>
#include <iostream>
#include <deque>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include "ServiceInit.hh"
#include "ux_selector.hh"
#include "api_SocketStreamer.hh"
#include <csignal>

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);  
}

ux_selector *server = nullptr;
typedef void (*HandlerFunction)(const int &, exchange_api::ExchangeApiUnion *, exchange_api::ExchangeApiUnion *);
HandlerFunction handler[15];

void processNewOrder(const int &ClientId, exchange_api::ExchangeApiUnion *req, exchange_api::ExchangeApiUnion *res) {
    std::cout << ClientId << "," << "processNewOrder" << std::endl;
}

void processGenMsg(const int &ClientId, exchange_api::ExchangeApiUnion *req, exchange_api::ExchangeApiUnion *res) {
    std::cout << ClientId << "," << "genmsg" << std::endl;
}

void initHandler()
{
    handler[(int)exchange::ExchangeApiMsgType_t::gen_msg]   = processGenMsg;
    handler[(int)exchange::ExchangeApiMsgType_t::new_order] = processNewOrder;
}

int processRequests()
{
    int iRC, iServerPort, iClientFD, iClientID;
    exchange_api::ExchangeApiUnion * inmsg = nullptr;
    exchange_api::ExchangeApiUnion * outmsg = nullptr;
    //int maxmsgsize = sizeof(exchange_api::ExchangeApiUnion);
    char request[4096];
    char response[4096];
    inmsg = (exchange_api::ExchangeApiUnion *) request;
    outmsg = (exchange_api::ExchangeApiUnion *) response;

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
        memset(request, 0, 4096);
        memset(response, 0, 4096);
        iRC = server->Read(iClientID, request);
        if ( iRC == ux_selector::SUCCESS )
        {
            inmsg->ntoh();
            //Read and process the msg
            auto evtreq = inmsg->api_msg_type;
            handler[(int)evtreq](iClientID, inmsg, outmsg);

            // inmsg->gen_msg.reset();
            // inmsg->gen_msg.setSeqNum(100);
            // inmsg->gen_msg.setSenderCompId("selvamd");
            // inmsg->gen_msg.setTargetCompId("selvamd");

            // inmsg->hton();
            // server->Write(iClientID, request, sizeof(exchange_api::GenMsg));
            // server->flush();
        }
        else
        {
            return (iRC == ux_selector::END_OF_SOCK_LIST)? 0:-1;
        }
        database.commit();
    }
    return 0;
}

int main(int , char ** )
{
    signal(SIGINT, signalHandler);  

    initHandler();
    initdb<Service_t::MATCHER>();

    server = new ux_selector(20);
    server->AddServer(65000);

    while (true)
    {
        int rc = processRequests();
        if (rc == -2) break;
    }

    closedb<Service_t::MATCHER>();
}
