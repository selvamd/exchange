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

    int64_t getNextFirmId()  { return ++max_firm_id;   }
    int64_t getNextOrderId() { return ++max_order_id;  }
    int64_t getNextExecId()  { return ++max_exec_id;   }
    int64_t getInviteId()    { return ++max_invite_id; }

    /*
    * Allows non-subid firms to be created thru SOD Files and API
    * In either case, user is expected to assign/supply unique FirmIds
    * For subid firms created intraday, unique firmid is assigned by the system
    */
    FirmLookup * createFirmIntraday(int64_t firmid = 0) 
    {
        auto &firmdb = imdb.getTable<FirmLookup>();
        if (firmid == 0) firmid = getNextFirmId();
        else if (firmid <= max_firm_id) {
            FirmLookup key;
            key.setFirmId(firmid);
            auto obj = firmdb.findByPrimaryKey(&key);
            if (obj != nullptr) return nullptr;
        } 
        else max_firm_id = firmid;
        
        auto res = firmdb.createObject();
        res->setFirmId(firmid);
        return res;
    }
    
    void printdb() 
    {
      std::cout << "Order count        : " << imdb.getRecordCount<OrderLookup>() << std::endl;
      std::cout << "Symbol count       : " << imdb.getRecordCount<SymbolLookup>() << std::endl;
      std::cout << "Firm count         : " << imdb.getRecordCount<FirmLookup>() << std::endl;
      std::cout << "Config count       : " << imdb.getRecordCount<ConfigLookup>() << std::endl;
      std::cout << "TimerEvent count   : " << imdb.getRecordCount<TimerEvent>() << std::endl;
      std::cout << "OrderEvent count   : " << imdb.getRecordCount<OrderEvent>() << std::endl;
    //   std::cout << *imdb.getTable<FirmLookup>().getObject(0) << std::endl;
    //   std::cout << *imdb.getTable<SymbolLookup>().getObject(0) << std::endl;
    //   std::cout << *imdb.getTable<ConfigLookup>().getObject(0) << std::endl;
    }

    void initdb() 
    {
      imdb.getTable<FirmLookup>(nullptr,1500*sizeof(FirmLookup));
      imdb.getTable<SymbolLookup>(nullptr,20000*sizeof(SymbolLookup));
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

      auto &firmdb = imdb.getTable<FirmLookup>();
      for (int i = 0; i < firmdb.capacity(); i++) {
        auto firm = firmdb.getObject(i);
        if (firm == nullptr) continue;
        if (firm->getFirmId() > max_firm_id) 
          max_firm_id = firm->getFirmId();
      }

      auto &orddb = imdb.getTable<OrderLookup>();
      for (int i = 0; i < orddb.capacity(); i++) 
      {
        auto ord = orddb.getObject(i);
        if (ord == nullptr) continue;
        if (ord->getOrderId() > max_order_id) 
          max_order_id = ord->getOrderId();
        if (ord->getInviteId() > max_invite_id)
          max_invite_id = ord->getInviteId();
      }

      imdb.getLogger(bufLogger,8192);
      dbwriter->setBuffer(bufSerializer,1500);
      imdb.addDBChangeListener(dbwriter);
      printdb();
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

    DomainDB &imdb = DomainDB::instance(0);
    exchange_api::ExchangeApiUnion * request() { return &m_request; }
    exchange_api::ExchangeApiUnion * response() { return &m_response; }


  private:

    int64_t max_firm_id   = 1000000;
    int64_t max_order_id  = 1000000;
    int64_t max_exec_id   = 1000000;
    int64_t max_invite_id = 1000000;

    exchange_api::ExchangeApiUnion m_request;
    exchange_api::ExchangeApiUnion m_response;
    const std::string dbfile = "../data/matcher.dat";
    const std::string dblog = "../data/matcher.log";
    char bufSerializer[1500];
    char bufLogger[8192];
    DBFileSerializer *dbwriter = new DBFileSerializer(dblog);
  };

#endif
