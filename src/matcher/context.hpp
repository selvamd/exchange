#ifndef CONTEXT_H
#define CONTEXT_H
#include "DataTypes.hpp"
#include "DomainDatabase.hh"
#include "DomainDBListeners.hh"
#include "Enums.hh"
#include "MatcherDomainObjects.hh"
#include "ExchangeApi.hpp"
#include <cstring> 

class context 
{
  public:

    // void beginTransaction() {
    // }

    // bool hasResponse() 
    // {
    //     if (m_response.api_msg_type() != exchange::ExchangeApiMsgType_t::gen_msg)
    //       return true;
    //     auto &db = imdb.getTable<OrderEvent>();
    //     DomainTable<OrderEvent>::IndexIterator itrS, itrE;
    //     if (db.begin(itrS,"PrimaryKey") && db.end(itrE,"PrimaryKey")) {
    //             if (itrS != itrE) {
    //                 prepareExecReport(db.getObjectID(*(--itrE));
    //                 return true;
    //             }
    //     }
    //     return false;
    // }

    //Called at the end of msg processing
    // void endTransaction() 
    // {
    //     imdb.commit(); //commit with Timer & Order Events
    //     m_request.gen_msg.reset();
    //     m_response.get_msg.reset();
    // }

    void printdb() {
      std::cout << "Order count        : " << imdb.getRecordCount<OrderLookup>() << std::endl;
      std::cout << "Symbol count       : " << imdb.getRecordCount<SymbolLookup>() << std::endl;
      std::cout << "Firm count         : " << imdb.getRecordCount<FirmLookup>() << std::endl;
      std::cout << "Config count       : " << imdb.getRecordCount<ConfigLookup>() << std::endl;
      std::cout << "TimerEvent count   : " << imdb.getRecordCount<TimerEvent>() << std::endl;
      std::cout << "OrderEvent count   : " << imdb.getRecordCount<OrderEvent>() << std::endl;
    }

    void initdb() {
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

    void prepareExecReport(int32_t levt) { 
        auto evt = imdb.getTable<OrderEvent>().getObject(levt);
        int32_t lord = evt->getOrdIdx();
        auto ord = imdb.getTable<OrderLookup>().getObject(lord);
        auto sym = imdb.getTable<SymbolLookup>().getObject(ord->getSymbolIdx());
        if (ord != nullptr && evt != nullptr && sym != nullptr) {
            m_response.exec_report_msg.reset();
            m_response.exec_report_msg.setClOrdId(ord->getClOrdId().c_str());
            m_response.exec_report_msg.setExecId(evt->getExecId());
            m_response.exec_report_msg.setOrderQty(ord->getOrderQty());
            m_response.exec_report_msg.setOrdType(ord->getOrdType().toString());
            m_response.exec_report_msg.setPrice(ord->getPrice());
            m_response.exec_report_msg.setSide(ord->getSide().toString());
            m_response.exec_report_msg.setSymbol(sym->getName().c_str());
            m_response.exec_report_msg.setTimeInForce(ord->getTimeInForce().toString());
            m_response.exec_report_msg.setCumQty(evt->getPostFillCumQty());
            m_response.exec_report_msg.setLastPx(evt->getTradePrice());
            m_response.exec_report_msg.setLastShares(evt->getTradeQty());
            //m_response.exec_report_msg.setLeavesQty(evt->getPostFillLeavesQty());
            //m_response.exec_report_msg.setExecType(ord->getExecType().toString());
            m_response.exec_report_msg.setOrdStatus(evt->getOrdStatus().toString());
            // remove processed order from the book
            if (evt->getEventType()() == OrderEventType_t::FILL || 
              evt->getEventType()() == OrderEventType_t::UROUT || 
              evt->getEventType()() == OrderEventType_t::NOTHING_DONE)
                imdb.getTable<OrderLookup>().removeObject(lord);
        }
        if (evt != nullptr)
            imdb.getTable<OrderEvent>().removeObject(levt);
    }

    const std::string dbfile = "../data/matcher.dat";
    const std::string dblog = "../data/matcher.log";
    DBFileSerializer dbwriter = DBFileSerializer(dblog);
    char bufSerializer[1500];
    char bufLogger[8192];
  };

#endif
