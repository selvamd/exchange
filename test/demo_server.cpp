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

ux_selector *server = nullptr;

int processRequests()
{
    int iRC, iServerPort, iClientFD, iClientID;
    static char request[4096];
    memset(request,0,4096);
    server->PollForSocketEvent();
    while (true)
    {
        iRC = server->Accept(iServerPort, iClientFD);
        if ( iRC == ux_selector::END_OF_SOCK_LIST) break;
        if ( iRC == ux_selector::WOULD_BLOCK) continue;
        SocketStreamerBase * streamer = new SocketStreamerBase(iClientFD, 4096);
        iClientID = server->AddClient(streamer, 4096, 0, false);
    }
    
    while (true)
    {
        iRC = server->Read(iClientID, request);
        if ( iRC == ux_selector::SUCCESS )
        {
            std::string response = request;
            response.append("\n");
            server->Write(iClientID, (char *)response.c_str(), response.length());
            server->flush();
        }
        else return (iRC == ux_selector::END_OF_SOCK_LIST)? 0:-1;
    }
    return 0;
}

int main(int , char ** )
{
    server = new ux_selector(20);
    server->AddServer(63700);

    while (true)
    {
        int rc = processRequests();
        if (rc == -2) break;
    }
}
