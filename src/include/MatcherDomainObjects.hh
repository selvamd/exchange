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
		DECLARE_INDEX(int32_t, SymbolId, 3)
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
		DECLARE_INDEX(int64_t, RankTime, 12)
        //PRIMARYKEY
		DECLARE_INDEX(int64_t, OrderId, 13)

		DECLARE_MEMBER(int32_t, PartyId, 14) //MPID
		DECLARE_MEMBER(int32_t, PartySubId, 15) //SUBID
		DECLARE_MEMBER(int64_t, ExecId, 16)
		DECLARE_MEMBER(int64_t, Price, 17)
		DECLARE_MEMBER(int64_t, ArrivalTime, 18)
		DECLARE_MEMBER(EnumData<TimeInForce_t>, TimeInForce, 19)
		DECLARE_MEMBER(EnumData<OrdType_t>, OrdType, 20)
		DECLARE_MEMBER(EnumData<OrdStatus_t>, OrdStatus, 21)
		DECLARE_MEMBER(EnumData<OrderLife_t>, OrderLife, 22)
		DECLARE_MEMBER(EnumData<MinQtyInst_t>, MinQtyInst, 23)
		DECLARE_MEMBER(EnumData<ConditionalEligible_t>, ConditionalEligible, 24)
		DECLARE_MEMBER(EnumData<SelfTradeInst_t>, SelfTradeInst, 25)
		DECLARE_MEMBER(EnumData<AutoCancel_t>, AutoCancel, 26)
		DECLARE_MEMBER(int32_t, CumQty, 27)
		DECLARE_MEMBER(int32_t, AvgPx, 28)
		DECLARE_MEMBER(int32_t, MinQty, 29)
		DECLARE_MEMBER(int32_t, LeavesQty, 30)

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
        };

		static bool primaryKey(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getOrderId() != b->getOrderId())
                if (a->getOrderId() >= 0 && b->getOrderId() >= 0)
    				return a->getOrderId() < b->getOrderId();
			return a->d_row < b->d_row;
		};

		static bool bookIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolId() != b->getSymbolId())
                if (a->getSymbolId() >= 0 && b->getSymbolId() >= 0)
    				return a->getSymbolId() < b->getSymbolId();

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
                if (a->getOrderQty() != b->getOrderQty())
				    if ( (a->getOrderQty() >= 0) && (b->getOrderQty() >= 0))
                        return b->getOrderQty() > a->getOrderQty();
            }
			if (a->getRankTime() != b->getRankTime())
				if ( (a->getRankTime() >= 0) && (b->getRankTime() >= 0))
				    return a->getRankTime() < b->getRankTime();

			return a->d_row < b->d_row;
		};

        //Index used for lookup during self-trade and previous riskfill 
		static bool ownerBookIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolId() != b->getSymbolId())
                if (a->getSymbolId() >= 0 && b->getSymbolId() >= 0)
    				return a->getSymbolId() < b->getSymbolId();

            int i = a->getSide().compareForIndex(b->getSide());
            if (i != 0) return i < 0;

            i = a->getClientType().compareForIndex(b->getClientType());
            if (i != 0) return i < 0;

			if (a->getFirmId() != b->getFirmId())
				if ( (a->getFirmId() >= 0) && (b->getFirmId() >= 0))
				    return a->getFirmId() < b->getFirmId();

			if (a->getClientType().compareForIndex(ClientType_t::INVESTOR) == 0) {
                i = a->getOrderUrgency().compareForIndex(b->getOrderUrgency());
                if (i != 0) return i > 0;
                i = a->getContraCategory().compareForIndex(b->getContraCategory());
                if (i != 0) return i > 0;
            } else {
                if (a->getOrderQty() != b->getOrderQty())
				    if ( (a->getOrderQty() >= 0) && (b->getOrderQty() >= 0))
                        return b->getOrderQty() > a->getOrderQty();
            }
			if (a->getRankTime() != b->getRankTime())
				if ( (a->getRankTime() >= 0) && (b->getRankTime() >= 0))
				    return a->getRankTime() < b->getRankTime();

			return a->d_row < b->d_row;
		};

		static bool cancelIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSenderCompId() != b->getSenderCompId())
				if ( (a->getSenderCompId() != -1) && (b->getSenderCompId() != -1))
					return a->getSenderCompId() < b->getSenderCompId();

			if (a->getDeliverToCompId() != b->getDeliverToCompId())
				if ( (a->getDeliverToCompId() != -1) && (b->getDeliverToCompId() != -1))
					return a->getDeliverToCompId() < b->getDeliverToCompId();

			return a->d_row < b->d_row;
		};

		static bool clOrdIdIndex(const OrderLookup* a,  const OrderLookup* b)
		{
            if (a->getDeliverToCompId() != b->getDeliverToCompId())
				if ( (a->getDeliverToCompId() != -1) && (b->getDeliverToCompId() != -1))
                    return a->getDeliverToCompId() < b->getDeliverToCompId();
			int i = a->getClOrdId().compareForIndex(b->getClOrdId());
			if ( i != 0 ) return (i < 0);
			return a->d_row < b->d_row;
		};

		static bool conditionalIndex(const OrderLookup* a,  const OrderLookup* b)
		{
			if (a->getSymbolId() != b->getSymbolId())
				if ( (a->getSymbolId() != -1) && (b->getSymbolId() != -1))
    				return a->getSymbolId() < b->getSymbolId();

            int i = a->getSide().compareForIndex(b->getSide());
            if (i != 0) return i < 0;

			if (a->getInviteId() != b->getInviteId())
				if ( (a->getInviteId() != -1) && (b->getInviteId() != -1))
				    return a->getInviteId() < b->getInviteId();

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
        };

        static bool tickerKey(const SymbolLookup * a,  const SymbolLookup * b)
        {
            int i = a->getName().compareForIndex(b->getName());
            return (i == 0 )? a->d_row < b->d_row : i < 0;
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

        static int maxFields()    { return 5; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET5()
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
                if (a->getParentFirm() >= 0 && b->getParentFirm() >= 0) 
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

        static int maxFields()    { return 4; }
        #define SET(FieldIndex) DECLARE_END(FieldIndex)
        SET4()
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
                if ( a->getSymbolId() >= 0 && b->getSymbolId() >= 0 )
                    return ( a->getSymbolId() < b->getSymbolId() );

            if ( a->getFirmId() != b->getFirmId() )
                if ( a->getFirmId() >= 0 && b->getFirmId() >= 0 )
                    return ( a->getFirmId() < b->getFirmId() );

            return a->d_row < b->d_row;
        };

        static bool firmkey(const ConfigLookup * a,  const ConfigLookup * b)
        {
            int i = a->getConfigName().compareForIndex(b->getConfigName());
            if (i != 0) return i < 0;

            if ( a->getFirmId() != b->getFirmId() )
                if ( a->getFirmId() >= 0 && b->getFirmId() >= 0 )
                    return ( a->getFirmId() < b->getFirmId() );

            if ( a->getSymbolId() != b->getSymbolId() )
                if ( a->getSymbolId() >= 0 && b->getSymbolId() >= 0 )
                    return ( a->getSymbolId() < b->getSymbolId() );

            return a->d_row < b->d_row;
        };
};
