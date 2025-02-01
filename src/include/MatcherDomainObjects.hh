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
        DECLARE_INDEX(int32_t,SenderCompId,0) //Gateway
        //clordid index
		DECLARE_INDEX(int32_t,DeliverToCompId,1) //Client Session
        DECLARE_INDEX(FixedString<exchange::CLORD_ID_LENGTH+1>, ClOrdId, 2)
		DECLARE_INDEX(int32_t, SymbolIdx, 3)
		DECLARE_INDEX(EnumData<Side_t>, Side, 4)
        //conditional index=symbol,side,inviteid
		DECLARE_INDEX(int32_t, InviteId, 5)
        //ownerindex = firmid + bookindex
		DECLARE_INDEX(int32_t, FirmId, 6) //FIRMID
        //bookindex = usertype,[urgency,contracategory,ranktime],[ranktier,orderqty,ranktime]
		DECLARE_INDEX(EnumData<ClientType_t>, ClientType, 7)
		DECLARE_INDEX(EnumData<OrderUrgency_t>, OrderUrgency, 8)
		DECLARE_INDEX(EnumData<ContraCategory_t>, ContraCategory, 9)
		DECLARE_INDEX(EnumData<RiskTier_t>, RiskTier, 10)
		DECLARE_INDEX(int32_t, OrderQty, 11)
		DECLARE_INDEX(Timestamp, RankTime, 12)
        //PRIMARYKEY
		DECLARE_INDEX(int64_t, OrderId, 13)

		DECLARE_MEMBER(int32_t, PartyId, 14) //MPID
		DECLARE_MEMBER(int32_t, PartySubId, 15) //SUBID
		DECLARE_MEMBER(int64_t, Price, 16)
		DECLARE_MEMBER(int64_t, ArrivalTime, 17)
		DECLARE_MEMBER(EnumData<TimeInForce_t>, TimeInForce, 18)
		DECLARE_MEMBER(EnumData<OrdType_t>, OrdType, 19)
		DECLARE_MEMBER(EnumData<OrdStatus_t>, OrdStatus, 20)
		DECLARE_MEMBER(EnumData<OrderLife_t>, OrderLife, 21)
		DECLARE_MEMBER(EnumData<MinQtyInst_t>, MinQtyInst, 22)
		DECLARE_MEMBER(EnumData<ConditionalEligible_t>, ConditionalEligible, 23)
		DECLARE_MEMBER(EnumData<SelfTradeInst_t>, SelfTradeInst, 24)
		DECLARE_MEMBER(EnumData<AutoCancel_t>, AutoCancel, 25)
		DECLARE_MEMBER(int32_t, CumQty, 26)
		DECLARE_MEMBER(int32_t, AvgPx, 27)
		DECLARE_MEMBER(int32_t, MinQty, 28)
		DECLARE_MEMBER(int32_t, LeavesQty, 29)
		DECLARE_MEMBER(EnumData<OrdTypeExt_t>, OrdTypeExt, 30)

		//DECLARE_INDEX(FixedString<exchange::COMP_ID_LENGTH+1>, SenderCompIdStr, 1)
		//DECLARE_MEMBER(EnumData<ExecInst_t>, ExecInst, 14)
		//DECLARE_MEMBER(EnumData<OpenClose_t>, OpenClose, 16)
		//DECLARE_MEMBER(EnumData<ClientType_t>, ClientType, 18)
		// DECLARE_MEMBER(int64_t, SendingTime, 28)
		// DECLARE_MEMBER(int64_t, ExpireTime, 29)
		// DECLARE_MEMBER(int32_t, NoTradingSessions, 30)
		// DECLARE_MEMBER(FixedString<exchange::TEXT_LENGTH+1>, OptionalData, 31)
		// DECLARE_MEMBER(FixedString<exchange::TRADE_SESSION_LENGTH+1>, TradingSessionId, 32)
		// DECLARE_MEMBER(int64_t, InsertionTimestamp, 33)

        static int maxFields()    { return 31; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET31()
        #undef SET
                    
        const int d_row;
        const uint32_t d_dbid;
    
    private:
        //Called during table construction to create indexes 
        static void createIndices(DomainTable<OrderLookup> &table)
        {
            table.addIndex("PrimaryKey", primaryKey);
            table.addIndex("BookIndex", bookIndex);
            table.addIndex("OwnerBookIndex", ownerBookIndex);
            table.addIndex("ClordidIndex", clOrdIdIndex);
            table.addIndex("ConditionalIndex", conditionalIndex);
            table.addIndex("CancelIndex", cancelIndex);
            table.addIndex("RiskOffsetIndex", timeIndex);
        };

		static bool primaryKey(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getOrderId() != b->getOrderId())
                if (min(a->getOrderId(),b->getOrderId()) >= 0)
    				return a->getOrderId() < b->getOrderId();
			return a->d_row < b->d_row;
		};

		static bool bookIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolIdx() != b->getSymbolIdx())
                if (min(a->getSymbolIdx(),b->getSymbolIdx()) >= 0)
    				return a->getSymbolIdx() < b->getSymbolIdx();

            int i = a->getSide().compareForIndex(b->getSide());
            if (i != 0) return i < 0;

            i = a->getClientType().compareForIndex(b->getClientType());
            if (i != 0) return i < 0;

			if (a->getClientType().compareForIndex(ClientType_t::INVESTOR) == 0) {
                i = a->getOrderUrgency().compareForIndex(b->getOrderUrgency());
                if (i != 0) return i > 0;
                i = a->getContraCategory().compareForIndex(b->getContraCategory());
                if (i != 0) return i > 0;
            } else {
                i = a->getRiskTier().compareForIndex(b->getRiskTier());
                if (i != 0) return i > 0;
                if (a->getOrderQty() != b->getOrderQty())
                    if (min(a->getOrderQty(),b->getOrderQty()) >= 0)
                        return b->getOrderQty() > a->getOrderQty();
            }
            i = a->getRankTime().compareForIndex(b->getRankTime());
            if (i != 0) return i < 0;

			return a->d_row < b->d_row;
		};

        //Index used for lookup during self-trade and previous riskfill 
		static bool ownerBookIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolIdx() != b->getSymbolIdx())
                if (min(a->getSymbolIdx(),b->getSymbolIdx()) >= 0)
    				return a->getSymbolIdx() < b->getSymbolIdx();

            int i = a->getSide().compareForIndex(b->getSide());
            if (i != 0) return i < 0;

            i = a->getClientType().compareForIndex(b->getClientType());
            if (i != 0) return i < 0;

			if (a->getFirmId() != b->getFirmId())
                if (min(a->getFirmId(),b->getFirmId()) >= 0)
				    return a->getFirmId() < b->getFirmId();

			if (a->getClientType().compareForIndex(ClientType_t::INVESTOR) == 0) {
                i = a->getOrderUrgency().compareForIndex(b->getOrderUrgency());
                if (i != 0) return i > 0;
                i = a->getContraCategory().compareForIndex(b->getContraCategory());
                if (i != 0) return i > 0;
            } else {
                i = a->getRiskTier().compareForIndex(b->getRiskTier());
                if (i != 0) return i > 0;
                if (a->getOrderQty() != b->getOrderQty())
                    if (min(a->getOrderQty(),b->getOrderQty()) >= 0)
                        return b->getOrderQty() > a->getOrderQty();
            }
            i = a->getRankTime().compareForIndex(b->getRankTime());
            if (i != 0) return i < 0;

			return a->d_row < b->d_row;
		};

        //Index used for lookup during self-trade and previous riskfill 
		static bool timeIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolIdx() != b->getSymbolIdx())
                if (min(a->getSymbolIdx(),b->getSymbolIdx()) >= 0)
    				return a->getSymbolIdx() < b->getSymbolIdx();

            int i = a->getSide().compareForIndex(b->getSide());
            if (i != 0) return i < 0;

            //Get RiskProvider before Investor for this index
            i = a->getClientType().compareForIndex(b->getClientType());
            if (i != 0) return i > 0;

            i = a->getRankTime().compareForIndex(b->getRankTime());
            if (i != 0) return i < 0;

			return a->d_row < b->d_row;
		};

		static bool cancelIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolIdx() != b->getSymbolIdx())
                if (min(a->getSymbolIdx(),b->getSymbolIdx()) >= 0)
    				return a->getSymbolIdx() < b->getSymbolIdx();

			if (a->getDeliverToCompId() != b->getDeliverToCompId())
                if (min(a->getDeliverToCompId(),b->getDeliverToCompId()) >= 0)
					return a->getDeliverToCompId() < b->getDeliverToCompId();

			return a->d_row < b->d_row;
		};

		static bool clOrdIdIndex(const OrderLookup* a,  const OrderLookup* b)
		{
            if (a->getDeliverToCompId() != b->getDeliverToCompId())
                if (min(a->getDeliverToCompId(),b->getDeliverToCompId()) >= 0)
                    return a->getDeliverToCompId() < b->getDeliverToCompId();
			int i = a->getClOrdId().compareForIndex(b->getClOrdId());
			if ( i != 0 ) return (i < 0);
			return a->d_row < b->d_row;
		};

		static bool conditionalIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolIdx() != b->getSymbolIdx())
                if (min(a->getSymbolIdx(),b->getSymbolIdx()) >= 0)
    				return a->getSymbolIdx() < b->getSymbolIdx();

			if (a->getInviteId() != b->getInviteId())
                if (min(a->getInviteId(),b->getInviteId()) >= 0)
				    return a->getInviteId() < b->getInviteId();

            int i = a->getSide().compareForIndex(b->getSide());
            if (i != 0) return i < 0;

			return a->d_row < b->d_row;
		};
};

class SymbolLookup : public DomainObjectBase<SymbolLookup>
{
    public:
    
        friend DomainTable<SymbolLookup>;
        static int TableID() { return static_cast<int>(DomainObjects_t::SYMBOLLOOKUP); }
        static string TableName() { return "SymbolLookup"; }
        SymbolLookup(int i = -1, void * sBuff = NULL) : d_row(i), d_dbid(0) { clone(sBuff); }
    
        //Use DECLARE_INDEX if the data member is used to construct an index 
        DECLARE_INDEX(FixedString<20>, Name, 0)
        DECLARE_MEMBER(EnumData<SymbolStatus_t>, SymbolStatus, 1)
        DECLARE_MEMBER(int64_t, TransactionNumber, 2)
        DECLARE_MEMBER(int32_t, NBBOBidPx, 3)
        DECLARE_MEMBER(int32_t, NBBOAskPx, 4)

        static int maxFields()    { return 5; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET5()
        #undef SET
                    
        const int d_row;
        const uint32_t d_dbid;
    
    private:

        //Called during table construction to create indexes 
        static void createIndices(DomainTable<SymbolLookup> &table)
        {
            table.addIndex("PrimaryKey", primarykey);
        };

        static bool primarykey(const SymbolLookup * a,  const SymbolLookup * b)
        {
            int i = a->getName().compareForIndex(b->getName());
            return (i == 0 )? a->d_row < b->d_row : i < 0;
        };

};

class OrderEvent : public DomainObjectBase<OrderEvent>
{
    public:
    
        friend DomainTable<OrderEvent>;
        static int TableID() { return static_cast<int>(DomainObjects_t::ORDEREVENT); }
        static string TableName() { return "OrderEvent"; }
        OrderEvent(int i = -1, void * sBuff = NULL) : d_row(i), d_dbid(0) { clone(sBuff); }
    
        //Use DECLARE_INDEX if the data member is used to construct an index 
        DECLARE_INDEX(int32_t, OrdIdx, 0)
        DECLARE_INDEX(EnumData<OrderEventType_t>, EventType, 1)
		DECLARE_MEMBER(int64_t, ExecId, 2)
		DECLARE_MEMBER(int64_t, TradePrice, 3)
		DECLARE_MEMBER(int32_t, TradeQty, 4)
		DECLARE_MEMBER(int32_t, PostFillCumQty, 5)
		DECLARE_MEMBER(int32_t, PostFillLeavesQty, 6)
		DECLARE_MEMBER(int64_t, AvgPrice, 7)
		DECLARE_MEMBER(EnumData<OrdStatus_t>, OrdStatus, 8)

        static int maxFields()    { return 9; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET9()
        #undef SET
                    
        const int d_row;
        const uint32_t d_dbid;
    
    private:
        //Called during table construction to create indexes 
        static void createIndices(DomainTable<OrderEvent> &table)
        {
            table.addIndex("PrimaryKey", primaryKey);
        };

		static bool primaryKey(const OrderEvent* a,  const OrderEvent* b)
		{
			if (a->getOrdIdx() != b->getOrdIdx())
                if (min(a->getOrdIdx(),b->getOrdIdx()) >= 0)
    				return a->getOrdIdx() < b->getOrdIdx();
            int i = a->getEventType().compareForIndex(b->getEventType());
            if (i != 0) return i < 0;
			return a->d_row < b->d_row;
		};
};

class TimerEvent : public DomainObjectBase<TimerEvent>
{
    public:
    
        friend DomainTable<TimerEvent>;
        static int TableID() { return static_cast<int>(DomainObjects_t::TIMEREVENT); }
        static string TableName() { return "TimerEvent"; }
        TimerEvent(int i = -1, void * sBuff = NULL) : d_row(i), d_dbid(0) { clone(sBuff); }
    
        //Use DECLARE_INDEX if the data member is used to construct an index 
        DECLARE_INDEX(Timestamp, EventTime, 0)
        DECLARE_MEMBER(EnumData<TimerEventType_t>, EventType, 1)
		DECLARE_MEMBER(int64_t, OrderID, 2)
		DECLARE_MEMBER(int32_t, SubIDIdx, 3)
		DECLARE_MEMBER(int32_t, PrfMpidIdx, 4)
		DECLARE_MEMBER(int32_t, SymbolIdx, 5)
        DECLARE_MEMBER(EnumData<Side_t>, Side, 6)
		DECLARE_MEMBER(int32_t, InviteID, 7)

        static int maxFields()    { return 8; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET8()
        #undef SET
                    
        const int d_row;
        const uint32_t d_dbid;
    
    private:
        //Called during table construction to create indexes 
        static void createIndices(DomainTable<TimerEvent> &table)
        {
            table.addIndex("PrimaryKey", primaryKey);
        };

		static bool primaryKey(const TimerEvent* a,  const TimerEvent* b)
		{
            int i = a->getEventTime().compareForIndex(b->getEventTime());
            if (i != 0) return i < 0;
			return a->d_row < b->d_row;
		};
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
        DECLARE_INDEX(EnumData<FirmRecordType_t>, FirmRecordType, 2)
        DECLARE_INDEX(int64_t, ParentFirm, 3)
        DECLARE_MEMBER(int64_t, ParentMPID, 4)
        DECLARE_MEMBER(int32_t, ClientID, 5)

        static int maxFields()    { return 6; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET6()
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
                if (min(a->getFirmId(),b->getFirmId()) >= 0)
                        return ( a->getFirmId() < b->getFirmId() );
            return a->d_row < b->d_row;
        };

        static bool firmKey(const FirmLookup * a,  const FirmLookup * b)
        {
            int i = a->getFirmRecordType().compareForIndex(b->getFirmRecordType());
            if (i != 0) return i < 0;

            //subid are flow identifiers within a firm (or mpid)
            //Cannot expect to be unique across firms 
            i = a->getName().compareForIndex(b->getName());
            if (i != 0 ) return i < 0;

            if (a->getParentFirm() != b->getParentFirm())
                if (min(a->getParentFirm(),b->getParentFirm()) >= 0)
                    return a->getParentFirm() > b->getParentFirm();

            return a->d_row < b->d_row;
        };

};

class ConfigLookup : public DomainObjectBase<ConfigLookup>
{
    public:
    
        friend DomainTable<ConfigLookup>;
        static int TableID() { return static_cast<int>(DomainObjects_t::CONFIGLOOKUP); }
        static string TableName() { return "ConfigLookup"; }
        ConfigLookup(int i = -1, void * sBuff = NULL) : d_row(i), d_dbid(0) { clone(sBuff); }
    
        //Use DECLARE_INDEX if the data member is used to construct an index 
        DECLARE_INDEX(EnumData<ConfigName_t>, ConfigName, 0)
        DECLARE_INDEX(int64_t, SymbolId,      1)
        DECLARE_INDEX(int64_t, FirmId,        2)
        DECLARE_MEMBER(int64_t, ConfigValue,  3)
        DECLARE_MEMBER(int32_t, ConfigScale,  4)

        static int maxFields()    { return 5; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET5()
        #undef SET
                    
        const int d_row;
        const uint32_t d_dbid;
    
    private:

        //Called during table construction to create indexes 
        static void createIndices(DomainTable<ConfigLookup> &table)
        {
            table.addIndex("PrimaryKey", primarykey);
            table.addIndex("FirmConfigKey", firmkey);
        };

        static bool primarykey(const ConfigLookup * a,  const ConfigLookup * b)
        {
            int i = a->getConfigName().compareForIndex(b->getConfigName());
            if (i != 0) return i < 0;

            if ( a->getSymbolId() != b->getSymbolId() )
                if (min(a->getSymbolId(),b->getSymbolId()) >= 0)
                    return ( a->getSymbolId() < b->getSymbolId() );

            if ( a->getFirmId() != b->getFirmId() )
                if (min(a->getFirmId(),b->getFirmId()) >= 0)
                    return ( a->getFirmId() < b->getFirmId() );

            return a->d_row < b->d_row;
        };

        static bool firmkey(const ConfigLookup * a,  const ConfigLookup * b)
        {
            int i = a->getConfigName().compareForIndex(b->getConfigName());
            if (i != 0) return i < 0;

            if ( a->getFirmId() != b->getFirmId() )
                if (min(a->getFirmId(),b->getFirmId()) >= 0)
                    return ( a->getFirmId() < b->getFirmId() );

            if ( a->getSymbolId() != b->getSymbolId() )
                if (min(a->getSymbolId(),b->getSymbolId()) >= 0)
                    return ( a->getSymbolId() < b->getSymbolId() );

            return a->d_row < b->d_row;
        };
};
