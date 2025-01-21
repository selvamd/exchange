#pragma once

// All enums used internally in DBTable (DomainObject) goes here and have no mapped fix code.  
//API Enums mapped with fix code are in DataDictionary.hpp with exchange namespace
DEFINE_ENUM_TYPES(Service_t,MATCHER,GATEWAY)
DEFINE_ENUM_TYPES(DomainObjects_t,ORDERLOOKUP,SYMBOLLOOKUP,FIRMLOOKUP,CONFIGLOOKUP)
DEFINE_ENUM_TYPES(MatcherEvents_t,NEW_ORDER,REPLACE_ORDER,CXL_ORDER)

DEFINE_ENUM_TYPES(FirmRecordType_t,MATCHER,GATEWAY,USER_SESSION,FIRMID,MPID,SUBID,RP_PLATINUM,RP_GOLD,RP_SILVER)
DEFINE_ENUM_TYPES(EntityStatus_t,ACTIVE,DISABLE,BLOCK)

DEFINE_ENUM_TYPES(Side_t,BUY,SELL)

DEFINE_ENUM_TYPES(OrdType_t,MARKET,LIMIT)
DEFINE_ENUM_TYPES(OpenClose_t,OPEN,CLOSE)
DEFINE_ENUM_TYPES(TimeInForce_t,DAY,IOC)
DEFINE_ENUM_TYPES(OrdStatus_t,NEW,PARTIAL,FILLED,DONE_TODAY,CANCELED,UROUT,CANCEL_PENDING,STOPPED,REJECTED,NEW_PENDING,EXPIRED,REPLACE_PENDING)
DEFINE_ENUM_TYPES(AutoCancel_t,AUTO_CANCEL_ON,AUTO_CANCEL_OFF)
DEFINE_ENUM_TYPES(ExecInst_t,NONE,RFQ,AON)
DEFINE_ENUM_TYPES(ConditionalEligible_t,CONDITIONAL_ELIBIGLE_ON,CONDITIONAL_ELIBIGLE_OFF)
DEFINE_ENUM_TYPES(OrderLife_t,FIRST,SECOND)
DEFINE_ENUM_TYPES(ClientType_t,INVESTOR,RISK_PROVIDER)
DEFINE_ENUM_TYPES(OrderUrgency_t,LOW,MEDIUM,HIGH)
DEFINE_ENUM_TYPES(ContraCategory_t,PARITY,PARITY_PLUS_ONE, PARITY_PLUS_TWO)
DEFINE_ENUM_TYPES(RiskTier_t,SILVER,GOLD,PLATINUM)
DEFINE_ENUM_TYPES(SelfTradeInst_t,ALLOW,STOP_MPID_SUBID)
DEFINE_ENUM_TYPES(MinQtyInst_t,TRADE_RESIDUE,CANCEL_RESIDUE)

DEFINE_ENUM_TYPES(ConfigName_t,RP_ORDLIFE_MIN,RP_RT_NOTIONAL, \
    RP_RT_ELIGIBLE,RP_RO_NOTIONAL,RP_RO_ELIGIBLE,SYM_RO2RT_MIN, \
    SYM_RO2RT_MAX,SID_PRF_TIME,SID_PRF_TIMEOUT,SID_PRF_PROVIDER, \
    GBL_MAX_ORD_QTY,GBL_MAX_NOTIONAL,SYM_MRI_TIME, \
    SYM_MRI_RANDOM_INTERVAL,URGENCY_LEVEL)
