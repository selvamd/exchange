#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include "DataTypes.hpp"
#include "DomainDatabase.hh"
#include "DomainDBListeners.hh"
#include "Enums.hh"
#include "MatcherDomainObjects.hh"
#include "ExchangeApi.hpp"
#include <cstring> 
#include "ux_selector.hh"

class context 
{
  public:

    int64_t getNextFirmId() { return 0; }
    int64_t getNextOrderId() { return 0; }
    int64_t getNextExecId() { return 0; }
    int64_t getInviteId() { return 0; }

    void printdb() 
    {
      std::cout << "Order count        : " << imdb.getRecordCount<OrderLookup>() << std::endl;
      std::cout << "Symbol count       : " << imdb.getRecordCount<SymbolLookup>() << std::endl;
      std::cout << "Firm count         : " << imdb.getRecordCount<FirmLookup>() << std::endl;
      std::cout << "Config count       : " << imdb.getRecordCount<ConfigLookup>() << std::endl;
      std::cout << "TimerEvent count   : " << imdb.getRecordCount<TimerEvent>() << std::endl;
      std::cout << "OrderEvent count   : " << imdb.getRecordCount<OrderEvent>() << std::endl;
    }

    void initdb() 
    {
      imdb.getTable<FirmLookup>(nullptr,1500*sizeof(FirmLookup));
      imdb.getTable<SymbolLookup>(nullptr,10000*sizeof(SymbolLookup));
      imdb.getTable<OrderLookup>(nullptr,1000000*sizeof(OrderLookup));
      imdb.getTable<ConfigLookup>(nullptr,100000*sizeof(ConfigLookup));
      imdb.getTable<OrderEvent>(nullptr,10000*sizeof(OrderEvent));
      imdb.getTable<TimerEvent>(nullptr,10000*sizeof(TimerEvent));

      //loads the snapshot data
      imdb.load<FirmLookup>(dbfile);
      imdb.load<SymbolLookup>(dbfile);
      imdb.load<OrderLookup>(dbfile);
      imdb.load<ConfigLookup>(dbfile);
      imdb.load<OrderEvent>(dbfile);
      imdb.load<TimerEvent>(dbfile);

      {
          //process db log to roll forward
          DBFileSerializer dbreader(dblog);
          dbreader.setBuffer(bufSerializer,1500);
          dbreader.unpackLog(imdb);
      }
      imdb.getLogger(bufLogger,8192);
      dbwriter.setBuffer(bufSerializer,1500);
      imdb.addDBChangeListener(&dbwriter);
    }

    void closedb() {
      //removes any old bakups first
      std::string cmd = "rm " + dbfile;
      system(cmd.c_str());
      //stores process state to disk
      imdb.store<OrderLookup>(dbfile);
      imdb.store<FirmLookup>(dbfile);
      imdb.store<SymbolLookup>(dbfile);
      imdb.store<ConfigLookup>(dbfile);
      imdb.store<TimerEvent>(dbfile);
      imdb.store<OrderEvent>(dbfile);
    }

    DomainDB &imdb = DomainDB::instance(1);
    exchange_api::ExchangeApiUnion * request() { return &m_request; }
    exchange_api::ExchangeApiUnion * response() { return &m_response; }

  private:
    exchange_api::ExchangeApiUnion m_request;
    exchange_api::ExchangeApiUnion m_response;
    const std::string dbfile = "../data/matcher.dat";
    const std::string dblog = "../data/matcher.log";
    DBFileSerializer dbwriter = DBFileSerializer(dblog);
    char bufSerializer[1500];
    char bufLogger[8192];
};

#endif
