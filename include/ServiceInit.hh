#pragma once

#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <assert.h>

using namespace std;

#include "DataTypes.hpp"
#include "DomainDatabase.hh"
#include "DomainDBListeners.hh"
#include "Enums.hh"
#include "MatcherDomainObjects.hh"

const std::string DATA_DIR = "../data/"; 
DomainDB &database = DomainDB::instance(1);
DBFileSerializer dbwriter("matcher.db");
char bufSerializer[1500];
char bufLogger[8192];

template<Service_t> void printdb() {}
template<Service_t> void initdb() {}
template<Service_t> void closedb() {}

template<> void printdb<Service_t::MATCHER>()
{
    std::cout << "Order count        : " << database.getRecordCount<OrderLookup>() << std::endl;
    std::cout << "Symbol count       : " << database.getRecordCount<SymbolLookup>() << std::endl;
    std::cout << "Firm count         : " << database.getRecordCount<FirmLookup>() << std::endl;
}

//Initialize memory and load the database
template<> void initdb<Service_t::MATCHER>()
{
    std::string dbfile = DATA_DIR;
    dbfile.append("matcher.dat");
    database.getTable<FirmLookup>(nullptr,1500*sizeof(FirmLookup));
    database.getTable<SymbolLookup>(nullptr,10000*sizeof(SymbolLookup));
    database.getTable<OrderLookup>(nullptr,1000000*sizeof(OrderLookup));
    database.load<FirmLookup>(dbfile);
    database.load<SymbolLookup>(dbfile);
    database.load<OrderLookup>(dbfile);

    {
        //reads and reloads the data
        DBFileSerializer dbreader("accounts.db");
        dbreader.setBuffer(bufSerializer,1500);
        dbreader.unpackLog(database);
    }

    database.getLogger(bufLogger,8192);
    dbwriter.setBuffer(bufSerializer,1500);
    database.addDBChangeListener(&dbwriter);

    printdb<Service_t::MATCHER>();
}

//Bkup the database
template<> void closedb<Service_t::MATCHER>()
{
    std::string dbfile = DATA_DIR;
    dbfile.append("account.dat");
    std::string cmd = "rm " + dbfile;
    system(cmd.c_str());

    database.store<OrderLookup>(dbfile);
    database.store<FirmLookup>(dbfile);
    database.store<SymbolLookup>(dbfile);

    printdb<Service_t::MATCHER>();
}
