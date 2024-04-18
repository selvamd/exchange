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

int processRequests()
{
    int iRC, iServerPort, iClientFD, iClientID;
    exchange_api::ExchangeApiUnion * inmsg = nullptr;
    //int maxmsgsize = sizeof(exchange_api::ExchangeApiUnion);
    char request[4096];
    memset(request, 0, 4096);
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
        iRC = server->Read(iClientID, request);
        if ( iRC == ux_selector::SUCCESS )
        {
            inmsg = (exchange_api::ExchangeApiUnion *) request;
            inmsg->ntoh();
            //cout << "received " << inmsg->gen_msg << std::endl;
            inmsg->gen_msg.reset();
            inmsg->gen_msg.setSeqNum(100);
            inmsg->gen_msg.setSenderCompId("selvamd");
            inmsg->gen_msg.setTargetCompId("selvamd");
            //cout << "sent " << inmsg->gen_msg << std::endl;
            inmsg->hton();
            server->Write(iClientID, request, sizeof(exchange_api::GenMsg));
            server->flush();
        }
        else
        {
            return (iRC == ux_selector::END_OF_SOCK_LIST)? 0:-1;
        }
    }
    return 0;
}

int main(int , char ** )
{
    signal(SIGINT, signalHandler);  

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
