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
#include "DataDictionary.hpp"
using namespace std;

class OrderLookup : public DomainObjectBase<OrderLookup>
{
    public:
    
        friend DomainTable<OrderLookup>;
        static int TableID() { return static_cast<int>(DomainObjects_t::ORDERLOOKUP); }
        static string TableName() { return "OrderLookup"; }
        OrderLookup(int i = -1, void * sBuff = NULL) : d_row(i), d_dbid(0) { clone(sBuff); }
    
        //Use DECLARE_INDEX if the data member is used to construct an index 

		DECLARE_INDEX(int32_t,SenderCompId,0)
		DECLARE_INDEX(FixedString<exchange::COMP_ID_LENGTH+1>, SenderCompIdStr, 1)
		DECLARE_INDEX(FixedString<exchange::COMP_ID_LENGTH+1>, DeliverToCompId, 2)
		DECLARE_INDEX(FixedString<exchange::COMP_ID_LENGTH+1>, DeliverToLocationId, 3)
		DECLARE_INDEX(int32_t, SymbolId, 4)
		DECLARE_INDEX(int64_t, ClOrdId, 5)
		DECLARE_INDEX(EnumData<Side_t>, Side, 6)
		DECLARE_INDEX(int64_t, Price, 7)
		DECLARE_INDEX(int64_t, RankTime, 8)
		DECLARE_INDEX(int32_t, OrderId, 9)
		DECLARE_INDEX(int32_t, PartyId, 10)
		DECLARE_INDEX(int32_t, PartySubId, 11)

		DECLARE_MEMBER(int32_t, Account, 12)
		DECLARE_MEMBER(int32_t, MatcherId, 13)
		DECLARE_MEMBER(EnumData<ExecInst_t>, ExecInst, 14)
		DECLARE_MEMBER(EnumData<OrdType_t>, OrdType, 15)
		DECLARE_MEMBER(EnumData<OpenClose_t>, OpenClose, 16)
		DECLARE_MEMBER(EnumData<TimeInForce_t>, TimeInForce, 17)
		DECLARE_MEMBER(EnumData<ClientType_t>, ClientType, 18)
		DECLARE_MEMBER(EnumData<OrdStatus_t>, OrdStatus, 19)
		DECLARE_MEMBER(EnumData<AutoCancel_t>, AutoCancel, 20)
		DECLARE_MEMBER(int64_t, CumQty, 21)
		DECLARE_MEMBER(int64_t, StopPx, 22)
		DECLARE_MEMBER(int64_t, AvgPx, 23)
		DECLARE_MEMBER(int64_t, OrderQty, 24)
		DECLARE_MEMBER(int64_t, LeavesQty, 25)
		DECLARE_MEMBER(int64_t, QuoteQty, 26)
		DECLARE_MEMBER(int64_t, MaxFloor, 27)
		DECLARE_MEMBER(int64_t, SendingTime, 28)
		DECLARE_MEMBER(int64_t, ExpireTime, 29)
		DECLARE_MEMBER(int32_t, NoTradingSessions, 30)
		DECLARE_MEMBER(FixedString<exchange::TEXT_LENGTH+1>, OptionalData, 31)
		DECLARE_MEMBER(FixedString<exchange::TRADE_SESSION_LENGTH+1>, TradingSessionId, 32)
		DECLARE_MEMBER(int64_t, InsertionTimestamp, 33)
		DECLARE_MEMBER(int64_t, ExecId, 34)

        static int maxFields()    { return 35; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET35()
        #undef SET
                    
        const int d_row;
        const uint32_t d_dbid;
    
    private:
        //Called during table construction to create indexes 
        static void createIndices(DomainTable<OrderLookup> &table)
        {
            table.addIndex("PrimaryKey", primaryKey);
            table.addIndex("SessionIndex", sessionIndex);
            table.addIndex("OwnerIndex", ownerIndex);
            table.addIndex("ClordidIndex", clOrdIdIndex);
            table.addIndex("BookIndex", bookIndex);
        };

		static bool primaryKey(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getOrderId() != b->getOrderId())
				return a->getOrderId() < b->getOrderId();
			return a->d_row < b->d_row;
		}

		static bool clOrdIdIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			int i = a->getDeliverToCompId().compareForIndex(b->getDeliverToCompId());
			if ( i != 0 ) return (i < 0);
			if (a->getClOrdId() != b->getClOrdId())
				if ( (a->getClOrdId() != -1) && (b->getClOrdId() != -1))
					return a->getClOrdId() < b->getClOrdId();
			return a->d_row < b->d_row;
		}

		static bool bookIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolId() != b->getSymbolId())
				return a->getSymbolId() < b->getSymbolId();

            int i = a->getSide().compareForIndex(b->getSide());
            if (i != 0) return i < 0;

			if (a->getPrice() != b->getPrice()) 
                return (a->getSide().compareForIndex(Side_t::SELL) == 0)? 
                    a->getPrice() < b->getPrice() : a->getPrice() > b->getPrice();

            //OrderId is internal, unique and monotonic and used as proxy for time rank
			if (a->getRankTime() != b->getRankTime())
				return a->getRankTime() < b->getRankTime();

			return a->d_row < b->d_row;
		}

		static bool ownerIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getPartyId() != b->getPartyId())
				if ( (a->getPartyId() != -1) && (b->getPartyId() != -1))
					return a->getPartyId() < b->getPartyId();
			if (a->getPartySubId() != b->getPartySubId())
				if ( (a->getPartySubId() != -1) && (b->getPartySubId() != -1))
					return a->getPartySubId() < b->getPartySubId();
			if (a->getOrderId() != b->getOrderId())
				if ( (a->getOrderId() != -1) && (b->getOrderId() != -1))
					return a->getOrderId() < b->getOrderId();
			return a->d_row < b->d_row;
		}

		static bool sessionIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getMatcherId() != b->getMatcherId())
				if ( (a->getMatcherId() != -1) && (b->getMatcherId() != -1))
					return a->getMatcherId() < b->getMatcherId();
			if (a->getSenderCompId() != b->getSenderCompId())
				if ( (a->getSenderCompId() != -1) && (b->getSenderCompId() != -1))
					return a->getSenderCompId() < b->getSenderCompId();
			int i = a->getDeliverToCompId().compareForIndex(b->getDeliverToCompId());
			if ( i != 0 ) return (i < 0);
			return a->d_row < b->d_row;
		}

};

class SymbolLookup : public DomainObjectBase<SymbolLookup>
{
    public:
    
        friend DomainTable<SymbolLookup>;
        static int TableID() { return static_cast<int>(DomainObjects_t::SYMBOLLOOKUP); }
        static string TableName() { return "SymbolLookup"; }
        SymbolLookup(int i = -1, void * sBuff = NULL) : d_row(i), d_dbid(0) { clone(sBuff); }
    
        //Use DECLARE_INDEX if the data member is used to construct an index 
        DECLARE_INDEX(int64_t, SymbolId,            0)
        DECLARE_INDEX(FixedString<20>, Name,        1)
        DECLARE_MEMBER(EnumData<EntityStatus_t>, EntityStatus, 2)

        static int maxFields()    { return 3; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET3()
        #undef SET
                    
        const int d_row;
        const uint32_t d_dbid;
    
    private:

        //Called during table construction to create indexes 
        static void createIndices(DomainTable<SymbolLookup> &table)
        {
            table.addIndex("PrimaryKey", primarykey);
            table.addIndex("TickerKey", tickerKey);
        };

        static bool primarykey(const SymbolLookup * a,  const SymbolLookup * b)
        {
            if ( a->getSymbolId() != b->getSymbolId() )
                if ( a->getSymbolId() >= 0 && b->getSymbolId() >= 0 )
                    return ( a->getSymbolId() < b->getSymbolId() );
            return a->d_row < b->d_row;
        }

        static bool tickerKey(const SymbolLookup * a,  const SymbolLookup * b)
        {
            int i = a->getName().compareForIndex(b->getName());
            return (i == 0 )? a->d_row < b->d_row : i < 0;
        }
};

class FirmLookup : public DomainObjectBase<FirmLookup>
{
    public:
    
        friend DomainTable<FirmLookup>;
        static int TableID() { return static_cast<int>(DomainObjects_t::FIRMLOOKUP); }
        static string TableName() { return "FirmLookup"; }
        FirmLookup(int i = -1, void * sBuff = NULL) : d_row(i), d_dbid(0) { clone(sBuff); }
    
        //Use DECLARE_INDEX if the data member is used to construct an index 
        DECLARE_INDEX(int64_t, FirmId, 0)
        DECLARE_INDEX(FixedString<20>, Name, 1)
        DECLARE_MEMBER(EnumData<ClientType_t>, ClientType, 2)
        DECLARE_MEMBER(EnumData<EntityStatus_t>, EntityStatus, 3)

        static int maxFields()    { return 4; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET4()
        #undef SET
                    
        const int d_row;
        const uint32_t d_dbid;
    
    private:

        //Called during table construction to create indexes 
        static void createIndices(DomainTable<FirmLookup> &table)
        {
            table.addIndex("PrimaryKey", primarykey);
            table.addIndex("NameKey", firmKey);
        };

        static bool primarykey(const FirmLookup * a,  const FirmLookup * b)
        {
            if ( a->getFirmId() != b->getFirmId() )
                if (a->getFirmId() >= 0 && b->getFirmId() >= 0) 
                    return ( a->getFirmId() < b->getFirmId() );
            return a->d_row < b->d_row;
        }

        static bool firmKey(const FirmLookup * a,  const FirmLookup * b)
        {
            int i = a->getName().compareForIndex(b->getName());
            return (i == 0 )? a->d_row < b->d_row : i < 0;
        }

};
