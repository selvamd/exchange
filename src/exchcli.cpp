#include <fstream>
#include <ostream>
#include <iostream>
#include <deque>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include "ux_selector.hh"
#include "api_SocketStreamer.hh"

int main(int , char ** )
{
    ux_selector * client = new ux_selector(0);
    int iClientFD;
    int iRC = connectTCP("127.0.0.1", 65000, iClientFD);
    SocketStreamerBase * streamer = new api_SocketStreamer(iClientFD, 4096);

    char sBuff[4096];
    memset(sBuff,0,4096);
    exchange_api::ExchangeApiUnion * inmsg = (exchange_api::ExchangeApiUnion *)sBuff;

    int iClientID = client->AddClient(streamer, sizeof(inmsg), 4, false);
    for (int i=0;i<20;i++) {
        inmsg->gen_msg.reset();
        std::cout << inmsg->gen_msg << std::endl;
        inmsg->hton();
        client->Write(iClientID, sBuff, sizeof(exchange_api::GenMsg));
        client->flush();
    }
    while (true) 
    {
        iRC = client->Read(iClientID, sBuff);
        //sleep(1);
        //std::cout << iRC << std::endl;
        if ( iRC != ux_selector::SUCCESS ) continue;    
        inmsg = (exchange_api::ExchangeApiUnion *) sBuff;
        inmsg->ntoh();
        cout << "received " << inmsg->gen_msg << std::endl;
    } 

    //sleep(10);
}
