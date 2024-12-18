#include <fstream>
#include <ostream>
#include <iostream>
#include <deque>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include "BusinessSvcDomainObjects.hh"
#include "jsonxx.hpp"
#include "ux_selector.hh"
#include "loginsvcclient.hpp"

DomainDB &businessdb = DomainDB::instance(1);
std::string dbfile("business.dat");
typedef void (*HandlerFunction)(const int &, jsonxx::Object &);
HandlerFunction handler[9];
ux_selector *server = nullptr;
loginsvcclient *loginsvc = nullptr;

/*
<transactions>
START_BUSINESS
OPEN_ACCOUNT
SEND_TRASACTIONS
<queries>
*/


void processStartBusiness(const int &iClientID, jsonxx::Object &request)
{
    std::cout << iClientID << request.json() << std::endl;
}

void initHandler()
{
    handler[Events_t::START_BUSINESS] = processStartBusiness;
}

void printDb()
{
    std::cout << "BusinessLookup count  : " << businessdb.getRecordCount<BusinessLookup>() << std::endl;
    std::cout << "AccountLookup  count  : " << businessdb.getRecordCount<AccountLookup>() << std::endl;
    std::cout << "Transaction count     : " << businessdb.getRecordCount<Transaction>() << std::endl;
    std::cout << "BalanceSheet count    : " << businessdb.getRecordCount<BalanceSheet>() << std::endl;
}

//Initialize memory and load the database
void initDb()
{
    //BUSINESSLOOKUP,ACCOUNTLOOKUP,TRANSACTION,BALANCESHEET
    businessdb.getTable<BusinessLookup>(nullptr,50*sizeof(BusinessLookup));
    businessdb.getTable<AccountLookup>(nullptr,1000*sizeof(AccountLookup));
    businessdb.getTable<Transaction>(nullptr,1000*sizeof(Transaction));
    businessdb.getTable<BalanceSheet>(nullptr,1000*sizeof(BalanceSheet));
    businessdb.load<BusinessLookup>(dbfile);
    businessdb.load<AccountLookup>(dbfile);
    businessdb.load<Transaction>(dbfile);
    businessdb.load<BalanceSheet>(dbfile);
    printDb();
}

//Bkup the database
void closeDb()
{
    std::string cmd = "rm ";
    cmd.append(dbfile);
    system(cmd.c_str());
    businessdb.store<BusinessLookup>(dbfile);
    businessdb.store<AccountLookup>(dbfile);
    businessdb.store<Transaction>(dbfile);
    businessdb.store<BalanceSheet>(dbfile);
    printDb();
}


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
            jsonxx::Object requestobj;
            requestobj.parse(std::string(request));

            if (!requestobj.has<jsonxx::String>("Event")) continue;
            std::string event = requestobj.get<jsonxx::String>("Event");
            if (event == "EXIT") return -2;
            auto evtreq = EnumData<Events_t>(event);
            if (evtreq.isValid())
                handler[evtreq()](iClientID, requestobj);
            else
                requestobj << "error" << "Invalid event";
            std::string response = requestobj.json();
            strSearchReplace(response,"\n","");
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
    initDb();
    initHandler();

    server = new ux_selector(0);
    server->AddServer(63700);

    loginsvc = new loginsvcclient("127.0.0.1",63600);

    while (true)
    {
        //sync with login process
        loginsvc->process();
        
        int rc = processRequests();
        if (rc == -2) break;
    }

    closeDb();
}
