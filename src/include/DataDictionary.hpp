#ifndef _DATA_DICTIONARY_HPP_
#define _DATA_DICTIONARY_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <map>
#include <set>

#include "ExchangeTemplates.hpp"
//#include "ApiEnums_gen.hpp"
#include "DataDictionary.hpp"

namespace exchange {

/*
 *  Define field length for all API message fields
 */
static const int32_t APP_VERSION_LENGTH                 = 10;
static const int32_t COMP_ID_LENGTH                     = 10;
static const int32_t CLORD_ID_LENGTH                    = 30;
static const int32_t TRADE_ID_LENGTH                    = 20;
static const int32_t SYMBOL_LENGTH                      = 40;
static const int32_t FIRM_LENGTH                        = 10;
static const int32_t TEXT_LENGTH                        = 100;
static const int32_t RATING_LENGTH                      = 10;
static const int32_t SIDE_LENGTH                        = 5;
static const int32_t CXL_TYPE_LENGTH                    = 4;
static const int32_t SECURITY_TYPE_LEN                  = 4;
static const int32_t TRADE_SESSION_LENGTH               = 4;
static const int32_t MOBILE_USERNAME_LENGTH             = 10;
static const int32_t MOBILE_PASSWORD_LENGTH             = 10;
static const int32_t LOGON_STATUS_TEXT_LENGTH           = 10;
static const int32_t MAX_VIEW_ORDER_RECORDS             = 10;
static const int32_t MAX_VIEW_TRADE_RECORDS             = 20;
static const int32_t MAX_TRADING_ACCOUNTS_RECORDS       = 20;
static const int32_t MAX_UNDL_LIST_RECORDS              = 20;
static const int32_t MAX_SYMBOL_LIST_RECORDS            = 20;
static const int32_t MAX_VIEW_FMV_SUMMARY_BY_UNDERLYING = 20;
static const int32_t MAX_VIEW_FMV_RATING_BY_SERIES      = 20;
static const int32_t MAX_VIEW_FMV_PRICE_BY_SERIES       = 20;
static const int32_t MAX_VIEW_FMV_TRADINGSTAT_BY_SERIES = 20;
static const int32_t MAX_GET_FMV_SYMBOL_SEARCH          = 20;
static const int32_t MAX_GET_TOP_GAINERS                = 20;
static const int32_t MAX_GET_TOP_LOSERS                 = 20;
static const int32_t MAX_SYMBOL_DEFINITION_RECORDS      = 10;
static const int32_t MAX_GET_ACCOUNTS                   = 20;
static const int32_t ERROR_STR_LENGTH                   = 255;
static const int32_t DESCRIPTION_LENGTH                 = 100;
static const int32_t KEY_LENGTH                         = 50;
static const int32_t CREDIT_EVENT_LIST_LENGTH           = 150;
static const int32_t PROTECTION_AMOUNT_LIST_LENGTH      = 100;
static const int32_t NAME_LENGTH                        = 25;
static const int32_t ADDRESS_LENGTH                     = 75;
static const int32_t PHONE_NUMBER_LENGTH                = 20;
static const int32_t MAX_USER_WATCH_LIST_SYMBOLS        = 50;
static const int32_t MAX_WATCHLIST_SYMBOLS              = 10;
static const int32_t WATCHLIST_LENGTH                   = SYMBOL_LENGTH*MAX_WATCHLIST_SYMBOLS+MAX_WATCHLIST_SYMBOLS; // Symbol lengths + commas
static const int32_t WATCH_LIST_LENGTH                  = SYMBOL_LENGTH*MAX_USER_WATCH_LIST_SYMBOLS+MAX_USER_WATCH_LIST_SYMBOLS; // Symbol lengths + commas
static const int32_t MARGIN_TYPE_LENGTH                 = 40;
static const int32_t XML_LOCAL_MSG_BUFFER_SIZE          = 10240;
static const int32_t MAX_VIEW_PORTFOLIO_RECORDS         = 10;
static const int32_t MAX_VIEW_ALLOCATION_RECORDS        = 10;
static const int32_t MAX_VIEW_PERFORMANCE_RECORDS       = 12;
static const int32_t MAX_VIEW_NET_BALANCE_RECORDS       = 3;
static const int32_t MAX_INTERVAL_RECORDS               = 12;
static const int32_t TIME_PERIOD_LENGTH                 = 20;
static const int32_t BOND_SYMBOL_LENGTH                 = 200;
static const int32_t HYPOTHETICAL_BOND_SYMBOL_ID_LENGTH = 20;
static const int32_t BOND_DESCRIPTION_LENGTH            = 200;
static const int32_t MAX_FMV_RECORDS_LENGTH             = 10;
static const int32_t MAX_DATA_TIME_SLOT_RECORDS         = 20;
static const int32_t MAX_SERIES_DETAILS_RECORDS         = 20;
static const int32_t MAX_TREASURY_RATES_RECORDS         = 11;
static const int32_t FILTER_BY_LENGTH                   = 200;
static const int32_t MAX_BONDS_BY_UNDL_RECORDS          = 20;
static const int32_t MAX_GET_TRADES_RECORDS             = 20;
static const int32_t TRADE_STATUS_LENGTH                = 15;
static const int32_t TIME_LENGTH                        = 8;
static const int32_t TIME_WITH_MICROS_LENGTH            = 16;
static const int32_t FIRST_NAME_LENGTH                  = 20;
static const int32_t LAST_NAME_LENGTH                   = 20;
static const int32_t USER_TITLE_LENGTH                  = 20;
static const int32_t EMAIL_ADDRESS_LENGTH               = 100;
static const int32_t MAX_DAYS_OFF                       = 3;
static const int32_t SECURITY_CONTEXT_LENGTH            = 255;
static const int32_t EMAIL_PARAMETER_LENGTH             = 255;
static const int32_t CPOD_WEB_SESSION_LENGTH            = 25;
static const int32_t PUSH_NOTIFICATION_TEXT_LENGTH      = 255;
static const int32_t DEVICE_IDENTIFIER_LENGTH           = 255;
static const int32_t DEVICE_TOKEN_LENGTH                = 255;
static const int32_t PUSH_NOTIFICATION_ARRAY_LENGTH     = 5;
static const int32_t ISIN_CODE_LENGTH                   = 20;
static const int32_t BOND_COUPON_FREQUENCY_LENGTH       = 10;
static const int32_t MAX_VIEW_INDEX_COMPOSITION_RECORDS = 20;
static const int32_t MAX_GET_CURRENT_MATURITIES_RECORDS = 20;
static const int32_t MAX_GET_ZCB_ISSUANCE_DATA_RECORDS  = 20;
static const int32_t MAX_GET_ZCB_MARKET_INFO_RECORDS    = 20;
static const int32_t MAX_GET_ZCB_ISSUANCE_PARAMS_RECORDS = 20;
static const int32_t MAX_GET_BONDS_RECORDS              = 20;
static const int32_t MAX_GET_HYPOTHETICAL_BONDS_RECORDS = 20;
static const int32_t CPOD_PROVIDER_NAME_LENGTH          = 30;
static const int32_t CPOD_PROVIDER_DESCRIPTION_LENGTH   = 40;
static const int32_t TICKER_SYMBOL_LENGTH               = 25;
static const int32_t BOND_TYPE_LENGTH                   = 4;
static const int32_t SECTOR_LENGTH                      = 30;
static const int32_t INDUSTRY_LENGTH                    = 50;
static const int32_t DECIMAL_LENGTH                     = 20;

static const int32_t MAX_GET_BONDS_SECTOR_RECORDS        = 20;
static const int32_t MAX_GET_BONDS_INDUSTRY_RECORDS      = 150;
static const int32_t MAX_BOND_WATCH_LIST_SYMBOLS         = 50;
static const int32_t MAX_BOND_DATA_LIMIT_RECORDS         = 20;
static const int32_t MAX_GET_BONDS_RATING_VALUE_RECORDS  = 30;
static const int32_t MAX_GET_BONDS_INDEX_RECORDS         = 20;

static const int32_t MAX_MNG_VIEW_UNDERLYING_RECORDS = 20;
static const int32_t MAX_EXCHANGE_PROCESS_STATUS_RECORDS = 200;

static const int32_t NUMBER_OF_ACTIVE_MATURITY_DATES = 20;

//System limits
static const int32_t MAX_CONTRACT_QUANTITY_LIMIT = 9999;

//Price settings
static const int64_t PRICE_PRECISION = 3;
static const int64_t RATE_PRECISION  = 3;
static const int64_t SCORE_PRECISION = 1;

static const int64_t PRICE_SCALE = 1000000;
static const int64_t RATE_SCALE  = 1000000;
static const int64_t SCORE_SCALE  = 1000000;

static const int32_t MINIMUM_NUMBER_OF_CHALLENGE_QUESTIONS_TO_ANSWER = 3;

static const std::string BUY_SIDE_STRING  = "BUY";
static const std::string SELL_SIDE_STRING = "SELL";

//Trade status strings
static const std::string PENDING_TRADE_STATUS_STRING  = "PENDING";
static const std::string REJECTED_TRADE_STATUS_STRING = "REJECTED";
static const std::string OPENED_TRADE_STATUS_STRING   = "OPENED";
static const std::string CLOSED_TRADE_STATUS_STRING   = "CLOSED";
static const std::string BUSTED_TRADE_STATUS_STRING   = "BUSTED";
static const std::string CANCELED_TRADE_STATUS_STRING = "CANCELED";
static const std::string EXPIRED_TRADE_STATUS_STRING  = "EXPIRED";

/*
 *  Define enumerations needed for API messages defined with fix code mapping
 */
 enum class ExchangeApiMsgType_t
 {
    gen_msg = 0,
    nbbo_msg,
    timer_msg,
    new_order,
    replace_order,
    cancel,
    exec_report,
    cancel_reject
 };

enum class ExchangeApiMsgSubType_t
{
    CREATE,
    MODIFY,
    DELETE
};

enum class FirmType_t
{
    CLEARING_FIRM,
    TRADING_FIRM,
    TRADING_USER,
    TRADING_ACCOUNT,
    GATEWAY_CLIENT,
    GATEWAY,
    TRADING_PERMIT,
    MATCHER
};

enum class FirmTradingStatus_t
{
    ACTIVE,
    DISABLE,
    BLOCK
};

enum class CancelType_t
{
    AUTO_CANCEL_ONLY,
    CANCEL_ALL_ORDERS
};

enum class TimerEventType_t
{
    MRI_WAIT,
    CO_WAIT,
    PRF_TIMER    
};

enum class AssetCategory_t
{
    NONE,
    SINGLE_ENTITY,
    INDEX,
    INDEX_TRANCHE,
    RECOVERY_RATE,
    NON_STANDARD
};

enum class OrdStatus_t
{
    NEW,
    PARTIAL,
    FILLED,
    DONE_TODAY,
    CANCELED,
    UROUT,
    CANCEL_PENDING,
    STOPPED,
    REJECTED,
    NEW_PENDING,
    EXPIRED,
    REPLACE_PENDING
};

enum class TradeStatus_t
{
    PENDING = 1,
    REJECTED,
    OPENED,
    CLOSED,
    BUSTED,
    CANCELED,
    EXPIRED
};

enum class Side_t
{
    Buy,
    Sell,
};

enum class OrderType_t
{
    MARKET,
    LIMIT,
    STOP,
    STOPLIMIT
};

enum class TimeInForce_t
{
    DAY,
    GTC,
    IOC,
    FOK,
    GTX,
    GTD
};

enum class ClientType_t
{
    CUSTOMER,
    FIRM,
    BROKER,
    DEALER
};

enum class CxlRejReason_t
{
    TOOLATE,
    UNKNOWN_ORDER,
    CANCEL_PENDING_ALREADY,
    UNABLE_TO_PROCESS,
    DUPLICATE_REQUEST,
    OTHER
};

enum class CxlRejResponseTo_t
{
    CANCEL_REQUEST,
    REPLACE_REQUEST
};

enum class ExecInst_t
{
    NONE,
    RFQ,
    AON
};

enum class ExecType_t
{
    NONE,
    CANCEL,
    CORRECT
};

enum class OpenClose_t
{
    OPEN,
    CLOSE
};

enum class OrdRejReason_t
{
    NONE,
    BAD_SYMBOL_INFO,
    NOT_OPEN_FOR_TRADING,
    LIMIT_VIOLATION,
    DUPLICATE,
    BAD_USER_INFO
};

enum class TradeRejReason_t
{
    NONE,
    DUPLICATE,
    BAD_SYMBOL,
    BAD_BUYER,
    BAD_SELLER,
    BAD_TRADE
};

enum class OrdType_t
{
    MARKET,
    LIMIT,
    STOP,
    STOPLIMIT
};

enum class PossResend_t
{
    YES,
    NO
};

enum class AutoCancel_t
{
    AUTO_CANCEL_ON,
    AUTO_CANCEL_OFF
};

enum class ArchiveResendType_t
{
    RESEND_SINGLE_MESSAGE,
    RESET_SEQUENCE_NUMBER
};

// FIXME:
// Using prama to turn off this particular diagnostic arising due to getEnumVector implementation.
// Waring is that the val parameter is never used but is part of the required method overloading.
// Placeholder waiting for a different implementation.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static std::vector<std::string> & getEnumVector(ExchangeApiMsgType_t val)
{
    static std::vector<std::string> vec {  
                           "x-genmsg",
                           "N-nbbo_msg",
                           "T-timer_msg",
                           "D-new_order",
                           "G-replace_order",
                           "F-cancel",
                           "8-exec_report",
                           "9-cancel_reject",
        };
    return vec;
}

static std::vector<std::string> & getEnumVector(ClientType_t val)
{
    static std::vector<std::string> vec  {"0-CUSTOMER","1-FIRM","2-BROKER","3-DEALER"};
    return vec;
}

static std::vector<std::string> & getEnumVector(TimerEventType_t val)
{
    static std::vector<std::string> vec  {"0-MRI_WAIT","1-CO_WAIT","2-PRF_TIMER"};
    return vec;
}

static std::vector<std::string> & getEnumVector(CxlRejReason_t val)
{
    static std::vector<std::string> vec {  "0-TOOLATE","1-UNKNOWN_ORDER","3-CANCEL_PENDING_ALREADY",
                                            "4-UNABLE_TO_PROCESS","6-DUPLICATE_REQUEST","9-OTHER"};
    return vec;
}

static std::vector<std::string> & getEnumVector(CxlRejResponseTo_t val)
{
    static std::vector<std::string> vec   {"1-CANCEL_REQUEST","2-REPLACE_REQUEST"};
    return vec;
}

static std::vector<std::string> & getEnumVector(ExecInst_t val)
{
    static std::vector<std::string> vec {"0-NONE","1-RFQ","G-AON"};
    return vec;
}

static std::vector<std::string> & getEnumVector(ExecType_t val)
{
    static std::vector<std::string> vec {"0-NONE", "H-CANCEL", "G-CORRECT"};
    return vec;
}

static std::vector<std::string> & getEnumVector(OpenClose_t val)
{
    static std::vector<std::string> vec {"O-OPEN","C-CLOSE"};
    return vec;
}

static std::vector<std::string> & getEnumVector(OrdRejReason_t val)
{
    static std::vector<std::string> vec { "0-NONE", "1-BAD_SYMBOL_INFO","2-NOT_OPEN_FOR_TRADING","3-LIMIT_VIOLATION",
                                        "6-DUPLICATE","A-BAD_USER_INFO"};
    return vec;
}

// static std::vector<std::string> & getEnumVector(TradeRejReason_t val)
// {
//     static std::vector<std::string> vec { "0-NONE", "1-DUPLICATE", "2-BAD_SYMBOL","3-BAD_BUYER","4-BAD_SELLER", "5-BAD_TRADE" };
//     return vec;
// }

static std::vector<std::string> & getEnumVector(OrdStatus_t val)
{
    static std::vector<std::string> vec {"0-NEW","1-PARTIAL","2-FILLED","3-DONE_TODAY","4-CANCELED",
                                         "5-UROUT", "6-CANCEL_PENDING","7-STOPPED","8-REJECTED",
                                         "A-NEW_PENDING", "C-EXPIRED","E-REPLACE_PENDING"};
    return vec;
}

// static std::vector<std::string> & getEnumVector(TradeStatus_t val)
// {
//     static std::vector<std::string> vec
//     {
//         "x-Filler",
//         "x-" + PENDING_TRADE_STATUS_STRING,
//         "x-" + REJECTED_TRADE_STATUS_STRING,
//         "x-" + OPENED_TRADE_STATUS_STRING,
//         "x-" + CLOSED_TRADE_STATUS_STRING,
//         "x-" + BUSTED_TRADE_STATUS_STRING,
//         "x-" + CANCELED_TRADE_STATUS_STRING,
//         "x-" + EXPIRED_TRADE_STATUS_STRING
//     };
//     return vec;
// }

static std::vector<std::string> & getEnumVector(OrdType_t val)
{
    static std::vector<std::string> vec {"1-MARKET","2-LIMIT","3-STOP","4-STOPLIMIT"};
    return vec;
}

static std::vector<std::string> & getEnumVector(AutoCancel_t val)
{
    static std::vector<std::string> vec {"Y-AUTO_CANCEL_ON","N-AUTO_CANCEL_OFF"};
    return vec;
}

static std::vector<std::string> & getEnumVector(Side_t val)
{
    //static std::vector<std::string> vec {"1-BUY","2-SELL"};
    static std::vector<std::string> vec {"1-" + BUY_SIDE_STRING,"2-" + SELL_SIDE_STRING};
    return vec;
}

static std::vector<std::string> & getEnumVector(TimeInForce_t val)
{
    static std::vector<std::string> vec {"0-DAY","1-GTC","3-IOC","4-FOK","5-GTX","6-GTD"};
    return vec;

}

#pragma GCC diagnostic pop

}

#endif
