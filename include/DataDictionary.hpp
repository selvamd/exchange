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
 *  Define flags
 */
// Permission Flags:
//static const uint32_t TRADING_USER_FLAG   = 1 << 0;
//static const uint32_t MANAGING_USER_FLAG  = 1 << 1;
//static const uint32_t OPERATING_USER_FLAG = 1 << 2;

//Error messages
//static const std::string INVALID_UNDERLYING_OR_MATURITY_DATE_ERROR("Invalid underlying or maturity date.");

/*
//Bond defaults
static const float DEFAULT_RECOVERY_RATE = 0.40;
static const uint32_t DEFAULT_BOND_FACE_VALUE = 1000;
*/

/*
 *  Define enumerations needed for API messages
 */

 enum class ExchangeApiMsgType_t
 {
    gen_msg = 0,
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

enum class CpodSymbolType_t
{
    NONE,
    SINGLE_ENTITY,
    INDEX,
    INDEX_TRANCHE,
    RECOVERY_RATE,
    NON_STANDARD
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

enum class SystemStatus_t
{
    Active,
    Unitialized,
    Error,
    Unknown
};

enum class AccountStatus_t
{
    OPEN,
    CLOSED,
    SUSPEND,
    LIQUIDATE,
    DELETED
};

enum class AccountStatusCode_t
{
    GREEN,
    YELLOW,
    RED
};

enum class CpodxUserStatus_t
{
    UNVERIFIED,
    VERIFIED,
    DELETED
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

enum class EventType_t
{
    BANKRUPTCY,
    OBLIGATION_ACCELERATION,
    OBLIGATION_DEFAULT,
    FAILURE_TO_PAY,
    REPUDIATION_MORATORIUM,
    RESTRUCTURING
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

enum class SecurityIdSource_t
{
    CUSIP,
    SEDOL,
    ISIN,
    EXCHANGE,
    SIAC_CQS_CTS
};

enum class SecurityRequestResult_t
{
    VALID,
    INVALID,
    NO_MATCH,
    NOT_AUTHORIZED,
    NOT_AVAILABLE,
    NOT_SUPPORTED
};

enum class SecurityAssetType_t
{
    STOCK,
    BOND,
    SOVEREIGN_DEBT
};

enum class SecurityTradingStatus_t
{
    UNKNOWN,
    HALTED,
    OPEN,
    CLOSED,
    PREOPEN,
    PRECLOSE,
    REQUEST_REJECT
};

enum class MonthCode_t
{
    JAN,
    FEB,
    MAR,
    APR,
    MAY,
    JUNE,
    JULY,
    AUG,
    SEPT,
    OCT,
    NOV,
    DEC
};

enum class ArchiveResendType_t
{
    RESEND_SINGLE_MESSAGE,
    RESET_SEQUENCE_NUMBER
};

static const std::string TRADER_ID_ASCENDING_SORT_BY_STR("TRADER_ID_ASCENDING");
static const std::string FIRM_NAME_ASCENDING_SORT_BY_STR("FIRM_NAME_ASCENDING");
static const std::string TRADER_ID_DESCENDING_SORT_BY_STR("TRADER_ID_DESCENDING");
static const std::string FIRM_NAME_DESCENDING_SORT_BY_STR("FIRM_NAME_DESCENDING");

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
                           "D-new_order",
                           "G-replace_order",
                           "F-cancel",
                           "8-exec_report",
                           "9-cancel_reject",
        };
    return vec;
}


static std::vector<std::string> & getEnumVector(ExchangeApiMsgSubType_t val)
{
    static std::vector<std::string> vec  {"0-CREATE", "1-MODIFY", "2-DELETE"};
    return vec;
}

static std::vector<std::string> & getEnumVector(ClientType_t val)
{
    static std::vector<std::string> vec  {"0-CUSTOMER","1-FIRM","2-BROKER","3-DEALER"};
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

static std::vector<std::string> & getEnumVector(EventType_t val)
{
    static std::vector<std::string> vec {"1-BANKRUPTCY","2-OBLIGATION_ACCELERATION","3-OBLIGATION_DEFAULT",
                                    "4-FAILURE_TO_PAY","5-REPUDIATION_MORATORIUM","6-RESTRUCTURING"};
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

static std::vector<std::string> & getEnumVector(TradeRejReason_t val)
{
    static std::vector<std::string> vec { "0-NONE", "1-DUPLICATE", "2-BAD_SYMBOL","3-BAD_BUYER","4-BAD_SELLER", "5-BAD_TRADE" };
    return vec;
}

static std::vector<std::string> & getEnumVector(OrdStatus_t val)
{
    static std::vector<std::string> vec {"0-NEW","1-PARTIAL","2-FILLED","3-DONE_TODAY","4-CANCELED",
                                         "5-UROUT", "6-CANCEL_PENDING","7-STOPPED","8-REJECTED",
                                         "A-NEW_PENDING", "C-EXPIRED","E-REPLACE_PENDING"};
    return vec;
}

static std::vector<std::string> & getEnumVector(TradeStatus_t val)
{
    static std::vector<std::string> vec
    {
        "x-Filler",
        "x-" + PENDING_TRADE_STATUS_STRING,
        "x-" + REJECTED_TRADE_STATUS_STRING,
        "x-" + OPENED_TRADE_STATUS_STRING,
        "x-" + CLOSED_TRADE_STATUS_STRING,
        "x-" + BUSTED_TRADE_STATUS_STRING,
        "x-" + CANCELED_TRADE_STATUS_STRING,
        "x-" + EXPIRED_TRADE_STATUS_STRING
    };
    return vec;
}


static std::vector<std::string> & getEnumVector(OrdType_t val)
{
    static std::vector<std::string> vec {"1-MARKET","2-LIMIT","3-STOP","4-STOPLIMIT"};
    return vec;
}

static std::vector<std::string> & getEnumVector(PossResend_t val)
{
    static std::vector<std::string> vec {"Y-YES","N-NO"};
    return vec;
}

static std::vector<std::string> & getEnumVector(AutoCancel_t val)
{
    static std::vector<std::string> vec {"Y-AUTO_CANCEL_ON","N-AUTO_CANCEL_OFF"};
    return vec;
}

static std::vector<std::string> & getEnumVector(SecurityIdSource_t val)
{
    static std::vector<std::string> vec {"1-CUSIP","2-SEDOL","4-ISIN","8-EXCHANGE","9-SIAC_CQS_CTS"};
    return vec;
}

static std::vector<std::string> & getEnumVector(SecurityRequestResult_t val)
{
    static std::vector<std::string> vec  {"0-VALID","1-INVALID","2-NO_MATCH","3-NOT_AUTHORIZED",
                                                            "4-NOT_AVAILABLE","5-NOT_SUPPORTED"};
    return vec;
}

static std::vector<std::string> & getEnumVector(SecurityAssetType_t val)
{
    static std::vector<std::string> vec {"S-STOCK", "B-BOND", "U-SOVEREIGN_DEBT"};
    return vec;
}

static std::vector<std::string> & getEnumVector(SecurityTradingStatus_t val)
{
    static std::vector<std::string> vec  {"0-UNKNOWN","1-HALTED","2-OPEN","3-CLOSED","4-PREOPEN",
                                                            "5-PRECLOSE","6-REQUEST_REJECT"};
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

static std::vector<std::string> & getEnumVector(FirmType_t val)
{
    static std::vector<std::string> vec {
                "0-CLEARING_FIRM", "1-TRADING_FIRM", "2-TRADING_USER",
                "3-TRADING_ACCOUNT", "4-GATEWAY_CLIENT", "5-GATEWAY", "6-TRADING_PERMIT", "A-MATCHER" };
    return vec;
}

static std::vector<std::string> & getEnumVector(SystemStatus_t val)
{
    static std::vector<std::string> vec {  "0-ACTIVE", "1-UNINITIALIZED", "2-ERROR", "3-UNKNOWN" };
    return vec;
}

static std::vector<std::string> & getEnumVector(AccountStatus_t val)
{
    static std::vector<std::string> vec {  "0-OPEN", "1-CLOSE", "2-SUSPEND", "3-LIQUIDATE", "4-DELETED" };
    return vec;
}

static std::vector<std::string> & getEnumVector(AccountStatusCode_t val)
{
    static std::vector<std::string> vec {  "G-GREEN", "Y-YELLOW", "R-RED" };
    return vec;
}

static std::vector<std::string> & getEnumVector(CpodxUserStatus_t val)
{
    static std::vector<std::string> vec {  "0-UNVERIFIED", "1-VERIFIED", "2-DELETED" };
    return vec;
}

static std::vector<std::string> & getEnumVector(FirmTradingStatus_t val)
{
    static std::vector<std::string> vec { "0-ACTIVE","1-DISABLE","2-BLOCK"};
    return vec;
}

static std::vector<std::string> & getEnumVector(CancelType_t val)
{
    static std::vector<std::string> vec { "0-AUTO_CANCEL_ONLY","1-CANCEL_ALL_ORDERS"};
    return vec;
}

static std::vector<std::string> & getEnumVector(CpodSymbolType_t val)
{
    static std::vector<std::string> vec = { "N-NONE", "S-SINGLE_ENTITY", "I-INDEX", "T-INDEX_TRANCHE",
                                            "R-RECOVERY_RATE", "X-NON_STANDARD" };
    return vec;
}



static std::vector<std::string> & getEnumVector(AssetCategory_t val)
{
    static std::vector<std::string> vec = { "N-NONE", "S-SINGLE_ENTITY", "I-INDEX", "T-INDEX_TRANCHE",
        "R-RECOVERY_RATE", "X-NON_STANDARD" };
    return vec;
}

static std::vector<std::string> & getEnumVector(MonthCode_t val)
{
    static std::vector<std::string> vec { "F-JAN",
                                          "G-FEB",
                                          "H-MAR",
                                          "J-APR",
                                          "K-MAY",
                                          "M-JUNE",
                                          "N-JULY",
                                          "Q-AUG",
                                          "U-SEPT",
                                          "V-OCT",
                                          "X-NOV",
                                          "Z-DEC"};
    return vec;
}

static std::vector<std::string> & getEnumVector(ArchiveResendType_t val)
{
    static std::vector<std::string> vec { "S-RESEND_SINGLE_MESSAGE",
                                          "Q-RESET_SEQUENCE_NUMBER"};
    return vec;
}

/*
 *  Define exchange maps
static std::map<UserPermissions_t, uint32_t> UserPermissions_m
{
    {UserPermissions_t::NEW_USER, 0},
    {UserPermissions_t::TRADING_USER, TRADING_USER_FLAG},
    {UserPermissions_t::MANAGING_USER, MANAGING_USER_FLAG},
    {UserPermissions_t::OPERATING_USER, OPERATING_USER_FLAG}
};
 */

/*
static std::set<ExchangeApiMsgType_t> OfflineAllowedMsgTypes_s
{
    ExchangeApiMsgType_t::logon,
    ExchangeApiMsgType_t::ops_process_status_request,
    ExchangeApiMsgType_t::ops_process_status_response,
    ExchangeApiMsgType_t::ops_server_status_request,
    ExchangeApiMsgType_t::ops_server_status_response,
    ExchangeApiMsgType_t::ops_command_request,
    ExchangeApiMsgType_t::ops_command_response,
    ExchangeApiMsgType_t::mng_view_underlying_request,
    ExchangeApiMsgType_t::mng_view_underlying_response,
    ExchangeApiMsgType_t::ops_system_status_request,
    ExchangeApiMsgType_t::ops_system_status_response,
    ExchangeApiMsgType_t::bond_trade_update,
    ExchangeApiMsgType_t::bond_trade_update_ack,
};

static std::set<ExchangeApiMsgType_t> OpenAllowedMsgTypes_s
{
    ExchangeApiMsgType_t::logon,
    ExchangeApiMsgType_t::gen_msg,
    ExchangeApiMsgType_t::sod_request,
    ExchangeApiMsgType_t::sod_response,
    ExchangeApiMsgType_t::restart_request,
    ExchangeApiMsgType_t::restart_response,
    ExchangeApiMsgType_t::mobile_logon_request,
    ExchangeApiMsgType_t::mobile_logon_response,
    ExchangeApiMsgType_t::heartbeat_request,
    ExchangeApiMsgType_t::heartbeat_response,
    ExchangeApiMsgType_t::archive_data_msg,
    ExchangeApiMsgType_t::archive_ok,
    ExchangeApiMsgType_t::archive_resend,
    ExchangeApiMsgType_t::bond_trade_update,
    ExchangeApiMsgType_t::bond_trade_update_ack,
    ExchangeApiMsgType_t::get_watch_list_request,
    ExchangeApiMsgType_t::get_watch_list_response,
    ExchangeApiMsgType_t::get_symbol_search_request,
    ExchangeApiMsgType_t::get_symbol_search_response,
    ExchangeApiMsgType_t::update_watch_list_request,
    ExchangeApiMsgType_t::update_watch_list_response,
    ExchangeApiMsgType_t::view_fmv_summary_by_underlying_request,
    ExchangeApiMsgType_t::view_fmv_summary_by_underlying_response,
    ExchangeApiMsgType_t::view_fmv_price_by_series_request,
    ExchangeApiMsgType_t::view_fmv_price_by_series_response,
    ExchangeApiMsgType_t::view_fmv_tradingstat_by_series_request,
    ExchangeApiMsgType_t::view_fmv_tradingstat_by_series_response,
    ExchangeApiMsgType_t::view_fmv_rating_by_series_request,
    ExchangeApiMsgType_t::view_fmv_rating_by_series_response,
    ExchangeApiMsgType_t::view_fmv_history_by_series_request,
    ExchangeApiMsgType_t::view_fmv_history_by_series_response,
    ExchangeApiMsgType_t::view_biv_history_by_series_request,
    ExchangeApiMsgType_t::view_biv_history_by_series_response,
    ExchangeApiMsgType_t::view_series_details_by_underlying_request,
    ExchangeApiMsgType_t::view_series_details_by_underlying_response,
    ExchangeApiMsgType_t::get_treasury_rates_request,
    ExchangeApiMsgType_t::get_treasury_rates_response,
    ExchangeApiMsgType_t::get_top_gainers_request,
    ExchangeApiMsgType_t::get_top_gainers_response,
    ExchangeApiMsgType_t::get_top_losers_request,
    ExchangeApiMsgType_t::get_top_losers_response,
    ExchangeApiMsgType_t::get_system_stats_request,
    ExchangeApiMsgType_t::get_system_stats_response,
    ExchangeApiMsgType_t::ops_process_status_request,
    ExchangeApiMsgType_t::ops_process_status_response,
    ExchangeApiMsgType_t::ops_server_status_request,
    ExchangeApiMsgType_t::ops_server_status_response,
    ExchangeApiMsgType_t::ops_command_request,
    ExchangeApiMsgType_t::ops_command_response,
    ExchangeApiMsgType_t::calculate_biv_price_request,
    ExchangeApiMsgType_t::calculate_biv_price_response,
    ExchangeApiMsgType_t::error_msg,
    ExchangeApiMsgType_t::fmv_data_update,
    ExchangeApiMsgType_t::get_bonds_by_underlying_request,
    ExchangeApiMsgType_t::get_bonds_by_underlying_response,
    ExchangeApiMsgType_t::mng_view_underlying_request,
    ExchangeApiMsgType_t::mng_view_underlying_response,
    ExchangeApiMsgType_t::trade_request,
    ExchangeApiMsgType_t::trade_response,
    ExchangeApiMsgType_t::get_trades_request,
    ExchangeApiMsgType_t::get_trades_response,
    ExchangeApiMsgType_t::get_accounts_request,
    ExchangeApiMsgType_t::get_accounts_response,
    ExchangeApiMsgType_t::get_firm_summary_request,
    ExchangeApiMsgType_t::get_firm_summary_response,
    ExchangeApiMsgType_t::get_public_trader_info_request,
    ExchangeApiMsgType_t::get_public_trader_info_response,
    ExchangeApiMsgType_t::update_password_request,
    ExchangeApiMsgType_t::update_password_response,
    ExchangeApiMsgType_t::reset_password_request,
    ExchangeApiMsgType_t::reset_password_response,
    ExchangeApiMsgType_t::reset_forgotten_password_request,
    ExchangeApiMsgType_t::reset_forgotten_password_response,
    ExchangeApiMsgType_t::get_challenge_questions_request,
    ExchangeApiMsgType_t::get_challenge_questions_response,
    ExchangeApiMsgType_t::update_challenge_answers_request,
    ExchangeApiMsgType_t::update_challenge_answers_response,
    ExchangeApiMsgType_t::get_validation_challenge_question_request,
    ExchangeApiMsgType_t::get_validation_challenge_question_response,
    ExchangeApiMsgType_t::check_challenge_answer_request,
    ExchangeApiMsgType_t::check_challenge_answer_response,
    ExchangeApiMsgType_t::debug_reset_user_status_request,
    ExchangeApiMsgType_t::debug_reset_user_status_response,
    ExchangeApiMsgType_t::web_logon_request,
    ExchangeApiMsgType_t::web_logon_response,
    ExchangeApiMsgType_t::web_logoff_request,
    ExchangeApiMsgType_t::web_logoff_response,
    ExchangeApiMsgType_t::create_account_request,
    ExchangeApiMsgType_t::create_account_response,
    ExchangeApiMsgType_t::update_ios_device_token_request,
    ExchangeApiMsgType_t::update_ios_device_token_request,
    ExchangeApiMsgType_t::update_push_notifications_settings_request,
    ExchangeApiMsgType_t::get_push_notifications_settings_request,
    ExchangeApiMsgType_t::view_index_composition_request,
    ExchangeApiMsgType_t::get_current_maturities_request,
    ExchangeApiMsgType_t::get_current_maturities_response,
    ExchangeApiMsgType_t::get_public_symbol_search_request,
    ExchangeApiMsgType_t::get_public_symbol_search_response,
    ExchangeApiMsgType_t::view_public_series_details_by_underlying_request,
    ExchangeApiMsgType_t::view_public_series_details_by_underlying_response,
    ExchangeApiMsgType_t::get_zcb_issuance_data_request,
    ExchangeApiMsgType_t::get_zcb_issuance_data_response,
    ExchangeApiMsgType_t::get_zcb_market_info_request,
    ExchangeApiMsgType_t::get_zcb_market_info_response,
    ExchangeApiMsgType_t::get_zcb_issuance_params_request,
    ExchangeApiMsgType_t::get_zcb_issuance_params_response,
    ExchangeApiMsgType_t::get_bonds_request,
    ExchangeApiMsgType_t::get_bonds_response,
    ExchangeApiMsgType_t::get_protection_quote_request,
    ExchangeApiMsgType_t::get_protection_quote_response,
    ExchangeApiMsgType_t::bond_price_data_update,
    ExchangeApiMsgType_t::ops_system_status_request,
    ExchangeApiMsgType_t::ops_system_status_response,
    ExchangeApiMsgType_t::get_bonds_sector_request,
    ExchangeApiMsgType_t::get_bonds_sector_response,
    ExchangeApiMsgType_t::get_bonds_industry_request,
    ExchangeApiMsgType_t::get_bonds_industry_response,
    ExchangeApiMsgType_t::update_bonds_watch_list_request,
    ExchangeApiMsgType_t::update_bonds_watch_list_response,
    ExchangeApiMsgType_t::get_bonds_data_limits_request,
    ExchangeApiMsgType_t::get_bonds_data_limits_response,
    ExchangeApiMsgType_t::get_bonds_rating_values_request,
    ExchangeApiMsgType_t::get_bonds_rating_values_response,
    ExchangeApiMsgType_t::update_watch_list_custom_input_request,
    ExchangeApiMsgType_t::update_watch_list_custom_input_response,
    ExchangeApiMsgType_t::delete_watch_list_custom_input_request,
    ExchangeApiMsgType_t::delete_watch_list_custom_input_response,
    ExchangeApiMsgType_t::get_bonds_index_request,
    ExchangeApiMsgType_t::get_bonds_index_response,
    ExchangeApiMsgType_t::calculate_customized_bond_data_request,
    ExchangeApiMsgType_t::calculate_customized_bond_data_response,
    ExchangeApiMsgType_t::get_hypothetical_bonds_request,
    ExchangeApiMsgType_t::get_hypothetical_bonds_response,
    ExchangeApiMsgType_t::update_hypothetical_bond_request,
    ExchangeApiMsgType_t::update_hypothetical_bond_response,
    ExchangeApiMsgType_t::delete_hypothetical_bond_request,
    ExchangeApiMsgType_t::delete_hypothetical_bond_response,
};

static std::set<ExchangeApiMsgType_t> ClosedAllowedMsgTypes_s
{
    ExchangeApiMsgType_t::logon,
    ExchangeApiMsgType_t::gen_msg,
    ExchangeApiMsgType_t::sod_request,
    ExchangeApiMsgType_t::sod_response,
    ExchangeApiMsgType_t::restart_request,
    ExchangeApiMsgType_t::restart_response,
    ExchangeApiMsgType_t::mobile_logon_request,
    ExchangeApiMsgType_t::mobile_logon_response,
    ExchangeApiMsgType_t::heartbeat_request,
    ExchangeApiMsgType_t::heartbeat_response,
    ExchangeApiMsgType_t::archive_data_msg,
    ExchangeApiMsgType_t::archive_ok,
    ExchangeApiMsgType_t::archive_resend,
    ExchangeApiMsgType_t::bond_trade_update,
    ExchangeApiMsgType_t::bond_trade_update_ack,
    ExchangeApiMsgType_t::get_watch_list_request,
    ExchangeApiMsgType_t::get_watch_list_response,
    ExchangeApiMsgType_t::get_symbol_search_request,
    ExchangeApiMsgType_t::get_symbol_search_response,
    ExchangeApiMsgType_t::update_watch_list_request,
    ExchangeApiMsgType_t::update_watch_list_response,
    ExchangeApiMsgType_t::view_fmv_summary_by_underlying_request,
    ExchangeApiMsgType_t::view_fmv_summary_by_underlying_response,
    ExchangeApiMsgType_t::view_fmv_price_by_series_request,
    ExchangeApiMsgType_t::view_fmv_price_by_series_response,
    ExchangeApiMsgType_t::view_fmv_tradingstat_by_series_request,
    ExchangeApiMsgType_t::view_fmv_tradingstat_by_series_response,
    ExchangeApiMsgType_t::view_fmv_rating_by_series_request,
    ExchangeApiMsgType_t::view_fmv_rating_by_series_response,
    ExchangeApiMsgType_t::view_fmv_history_by_series_request,
    ExchangeApiMsgType_t::view_fmv_history_by_series_response,
    ExchangeApiMsgType_t::view_biv_history_by_series_request,
    ExchangeApiMsgType_t::view_biv_history_by_series_response,
    ExchangeApiMsgType_t::view_series_details_by_underlying_request,
    ExchangeApiMsgType_t::view_series_details_by_underlying_response,
    ExchangeApiMsgType_t::get_treasury_rates_request,
    ExchangeApiMsgType_t::get_treasury_rates_response,
    ExchangeApiMsgType_t::get_top_gainers_request,
    ExchangeApiMsgType_t::get_top_gainers_response,
    ExchangeApiMsgType_t::get_top_losers_request,
    ExchangeApiMsgType_t::get_top_losers_response,
    ExchangeApiMsgType_t::get_system_stats_request,
    ExchangeApiMsgType_t::get_system_stats_response,
    ExchangeApiMsgType_t::ops_process_status_request,
    ExchangeApiMsgType_t::ops_process_status_response,
    ExchangeApiMsgType_t::ops_server_status_request,
    ExchangeApiMsgType_t::ops_server_status_response,
    ExchangeApiMsgType_t::ops_command_request,
    ExchangeApiMsgType_t::ops_command_response,
    ExchangeApiMsgType_t::calculate_biv_price_request,
    ExchangeApiMsgType_t::calculate_biv_price_response,
    ExchangeApiMsgType_t::error_msg,
    ExchangeApiMsgType_t::fmv_data_update,
    ExchangeApiMsgType_t::get_bonds_by_underlying_request,
    ExchangeApiMsgType_t::get_bonds_by_underlying_response,
    ExchangeApiMsgType_t::mng_view_underlying_request,
    ExchangeApiMsgType_t::mng_view_underlying_response,
    ExchangeApiMsgType_t::get_trades_request,
    ExchangeApiMsgType_t::get_trades_response,
    ExchangeApiMsgType_t::get_accounts_request,
    ExchangeApiMsgType_t::get_accounts_response,
    ExchangeApiMsgType_t::get_firm_summary_request,
    ExchangeApiMsgType_t::get_firm_summary_response,
    ExchangeApiMsgType_t::get_public_trader_info_request,
    ExchangeApiMsgType_t::get_public_trader_info_response,
    ExchangeApiMsgType_t::update_password_request,
    ExchangeApiMsgType_t::update_password_response,
    ExchangeApiMsgType_t::reset_password_request,
    ExchangeApiMsgType_t::reset_password_response,
    ExchangeApiMsgType_t::reset_forgotten_password_request,
    ExchangeApiMsgType_t::reset_forgotten_password_response,
    ExchangeApiMsgType_t::get_challenge_questions_request,
    ExchangeApiMsgType_t::get_challenge_questions_response,
    ExchangeApiMsgType_t::update_challenge_answers_request,
    ExchangeApiMsgType_t::update_challenge_answers_response,
    ExchangeApiMsgType_t::get_validation_challenge_question_request,
    ExchangeApiMsgType_t::get_validation_challenge_question_response,
    ExchangeApiMsgType_t::check_challenge_answer_request,
    ExchangeApiMsgType_t::check_challenge_answer_response,
    ExchangeApiMsgType_t::debug_reset_user_status_request,
    ExchangeApiMsgType_t::debug_reset_user_status_response,
    ExchangeApiMsgType_t::web_logon_request,
    ExchangeApiMsgType_t::web_logon_response,
    ExchangeApiMsgType_t::web_logoff_request,
    ExchangeApiMsgType_t::web_logoff_response,
    ExchangeApiMsgType_t::create_account_request,
    ExchangeApiMsgType_t::create_account_response,
    ExchangeApiMsgType_t::update_ios_device_token_request,
    ExchangeApiMsgType_t::update_push_notifications_settings_request,
    ExchangeApiMsgType_t::get_push_notifications_settings_request,
    ExchangeApiMsgType_t::view_index_composition_request,
    ExchangeApiMsgType_t::get_current_maturities_request,
    ExchangeApiMsgType_t::get_current_maturities_response,
    ExchangeApiMsgType_t::get_public_symbol_search_request,
    ExchangeApiMsgType_t::get_public_symbol_search_response,
    ExchangeApiMsgType_t::view_public_series_details_by_underlying_request,
    ExchangeApiMsgType_t::view_public_series_details_by_underlying_response,
    ExchangeApiMsgType_t::get_zcb_issuance_data_request,
    ExchangeApiMsgType_t::get_zcb_issuance_data_response,
    ExchangeApiMsgType_t::get_zcb_market_info_request,
    ExchangeApiMsgType_t::get_zcb_market_info_response,
    ExchangeApiMsgType_t::get_zcb_issuance_params_request,
    ExchangeApiMsgType_t::get_zcb_issuance_params_response,
    ExchangeApiMsgType_t::get_bonds_request,
    ExchangeApiMsgType_t::get_bonds_response,
    ExchangeApiMsgType_t::get_protection_quote_request,
    ExchangeApiMsgType_t::get_protection_quote_response,
    ExchangeApiMsgType_t::bond_price_data_update,
    ExchangeApiMsgType_t::ops_system_status_request,
    ExchangeApiMsgType_t::ops_system_status_response,
    ExchangeApiMsgType_t::get_bonds_sector_request,
    ExchangeApiMsgType_t::get_bonds_sector_response,
    ExchangeApiMsgType_t::get_bonds_industry_request,
    ExchangeApiMsgType_t::get_bonds_industry_response,
    ExchangeApiMsgType_t::update_bonds_watch_list_request,
    ExchangeApiMsgType_t::update_bonds_watch_list_response,
    ExchangeApiMsgType_t::get_bonds_data_limits_request,
    ExchangeApiMsgType_t::get_bonds_data_limits_response,
    ExchangeApiMsgType_t::get_bonds_rating_values_request,
    ExchangeApiMsgType_t::get_bonds_rating_values_response,
    ExchangeApiMsgType_t::update_watch_list_custom_input_request,
    ExchangeApiMsgType_t::update_watch_list_custom_input_response,
    ExchangeApiMsgType_t::delete_watch_list_custom_input_request,
    ExchangeApiMsgType_t::delete_watch_list_custom_input_response,
    ExchangeApiMsgType_t::get_bonds_index_request,
    ExchangeApiMsgType_t::get_bonds_index_response,
    ExchangeApiMsgType_t::calculate_customized_bond_data_request,
    ExchangeApiMsgType_t::calculate_customized_bond_data_response,
    ExchangeApiMsgType_t::get_hypothetical_bonds_request,
    ExchangeApiMsgType_t::get_hypothetical_bonds_response,
    ExchangeApiMsgType_t::update_hypothetical_bond_request,
    ExchangeApiMsgType_t::update_hypothetical_bond_response,
    ExchangeApiMsgType_t::delete_hypothetical_bond_request,
    ExchangeApiMsgType_t::delete_hypothetical_bond_response,
};

static std::set<ExchangeApiMsgType_t> EodClosedAllowedMsgTypes_s
{
    ExchangeApiMsgType_t::logon,
    ExchangeApiMsgType_t::gen_msg,
    ExchangeApiMsgType_t::sod_request,
    ExchangeApiMsgType_t::sod_response,
    ExchangeApiMsgType_t::restart_request,
    ExchangeApiMsgType_t::restart_response,
    ExchangeApiMsgType_t::mobile_logon_request,
    ExchangeApiMsgType_t::mobile_logon_response,
    ExchangeApiMsgType_t::heartbeat_request,
    ExchangeApiMsgType_t::heartbeat_response,
    ExchangeApiMsgType_t::archive_data_msg,
    ExchangeApiMsgType_t::archive_ok,
    ExchangeApiMsgType_t::archive_resend,
    ExchangeApiMsgType_t::get_watch_list_request,
    ExchangeApiMsgType_t::get_watch_list_response,
    ExchangeApiMsgType_t::get_symbol_search_request,
    ExchangeApiMsgType_t::get_symbol_search_response,
    ExchangeApiMsgType_t::update_watch_list_request,
    ExchangeApiMsgType_t::update_watch_list_response,
    ExchangeApiMsgType_t::view_fmv_summary_by_underlying_request,
    ExchangeApiMsgType_t::view_fmv_summary_by_underlying_response,
    ExchangeApiMsgType_t::view_fmv_price_by_series_request,
    ExchangeApiMsgType_t::view_fmv_price_by_series_response,
    ExchangeApiMsgType_t::view_fmv_tradingstat_by_series_request,
    ExchangeApiMsgType_t::view_fmv_tradingstat_by_series_response,
    ExchangeApiMsgType_t::view_fmv_rating_by_series_request,
    ExchangeApiMsgType_t::view_fmv_rating_by_series_response,
    ExchangeApiMsgType_t::view_fmv_history_by_series_request,
    ExchangeApiMsgType_t::view_fmv_history_by_series_response,
    ExchangeApiMsgType_t::view_biv_history_by_series_request,
    ExchangeApiMsgType_t::view_biv_history_by_series_response,
    ExchangeApiMsgType_t::view_series_details_by_underlying_request,
    ExchangeApiMsgType_t::view_series_details_by_underlying_response,
    ExchangeApiMsgType_t::get_treasury_rates_request,
    ExchangeApiMsgType_t::get_treasury_rates_response,
    ExchangeApiMsgType_t::get_top_gainers_request,
    ExchangeApiMsgType_t::get_top_gainers_response,
    ExchangeApiMsgType_t::get_top_losers_request,
    ExchangeApiMsgType_t::get_top_losers_response,
    ExchangeApiMsgType_t::get_system_stats_request,
    ExchangeApiMsgType_t::get_system_stats_response,
    ExchangeApiMsgType_t::ops_process_status_request,
    ExchangeApiMsgType_t::ops_process_status_response,
    ExchangeApiMsgType_t::ops_server_status_request,
    ExchangeApiMsgType_t::ops_server_status_response,
    ExchangeApiMsgType_t::ops_command_request,
    ExchangeApiMsgType_t::ops_command_response,
    ExchangeApiMsgType_t::calculate_biv_price_request,
    ExchangeApiMsgType_t::calculate_biv_price_response,
    ExchangeApiMsgType_t::error_msg,
    ExchangeApiMsgType_t::get_bonds_by_underlying_request,
    ExchangeApiMsgType_t::get_bonds_by_underlying_response,
    ExchangeApiMsgType_t::mng_view_underlying_request,
    ExchangeApiMsgType_t::mng_view_underlying_response,
    ExchangeApiMsgType_t::get_trades_request,
    ExchangeApiMsgType_t::get_trades_response,
    ExchangeApiMsgType_t::get_accounts_request,
    ExchangeApiMsgType_t::get_accounts_response,
    ExchangeApiMsgType_t::get_firm_summary_request,
    ExchangeApiMsgType_t::get_firm_summary_response,
    ExchangeApiMsgType_t::get_public_trader_info_request,
    ExchangeApiMsgType_t::get_public_trader_info_response,
    ExchangeApiMsgType_t::update_password_request,
    ExchangeApiMsgType_t::update_password_response,
    ExchangeApiMsgType_t::reset_password_request,
    ExchangeApiMsgType_t::reset_password_response,
    ExchangeApiMsgType_t::reset_forgotten_password_request,
    ExchangeApiMsgType_t::reset_forgotten_password_response,
    ExchangeApiMsgType_t::get_challenge_questions_request,
    ExchangeApiMsgType_t::get_challenge_questions_response,
    ExchangeApiMsgType_t::update_challenge_answers_request,
    ExchangeApiMsgType_t::update_challenge_answers_response,
    ExchangeApiMsgType_t::get_validation_challenge_question_request,
    ExchangeApiMsgType_t::get_validation_challenge_question_response,
    ExchangeApiMsgType_t::check_challenge_answer_request,
    ExchangeApiMsgType_t::check_challenge_answer_response,
    ExchangeApiMsgType_t::web_logon_request,
    ExchangeApiMsgType_t::web_logon_response,
    ExchangeApiMsgType_t::web_logoff_request,
    ExchangeApiMsgType_t::web_logoff_response,
    ExchangeApiMsgType_t::create_account_request,
    ExchangeApiMsgType_t::create_account_response,
    ExchangeApiMsgType_t::update_ios_device_token_request,
    ExchangeApiMsgType_t::update_push_notifications_settings_request,
    ExchangeApiMsgType_t::get_push_notifications_settings_request,
    ExchangeApiMsgType_t::view_index_composition_request,
    ExchangeApiMsgType_t::get_current_maturities_request,
    ExchangeApiMsgType_t::get_current_maturities_response,
    ExchangeApiMsgType_t::get_public_symbol_search_request,
    ExchangeApiMsgType_t::get_public_symbol_search_response,
    ExchangeApiMsgType_t::view_public_series_details_by_underlying_request,
    ExchangeApiMsgType_t::view_public_series_details_by_underlying_response,
    ExchangeApiMsgType_t::get_zcb_issuance_data_request,
    ExchangeApiMsgType_t::get_zcb_issuance_data_response,
    ExchangeApiMsgType_t::get_zcb_market_info_request,
    ExchangeApiMsgType_t::get_zcb_market_info_response,
    ExchangeApiMsgType_t::get_zcb_issuance_params_request,
    ExchangeApiMsgType_t::get_zcb_issuance_params_response,
    ExchangeApiMsgType_t::get_bonds_request,
    ExchangeApiMsgType_t::get_bonds_response,
    ExchangeApiMsgType_t::get_protection_quote_request,
    ExchangeApiMsgType_t::get_protection_quote_response,
    ExchangeApiMsgType_t::bond_price_data_update,
    ExchangeApiMsgType_t::ops_system_status_request,
    ExchangeApiMsgType_t::ops_system_status_response,
    ExchangeApiMsgType_t::get_bonds_sector_request,
    ExchangeApiMsgType_t::get_bonds_sector_response,
    ExchangeApiMsgType_t::get_bonds_industry_request,
    ExchangeApiMsgType_t::get_bonds_industry_response,
    ExchangeApiMsgType_t::update_bonds_watch_list_request,
    ExchangeApiMsgType_t::update_bonds_watch_list_response,
    ExchangeApiMsgType_t::get_bonds_data_limits_request,
    ExchangeApiMsgType_t::get_bonds_data_limits_response,
    ExchangeApiMsgType_t::get_bonds_rating_values_request,
    ExchangeApiMsgType_t::get_bonds_rating_values_response,
    ExchangeApiMsgType_t::update_watch_list_custom_input_request,
    ExchangeApiMsgType_t::update_watch_list_custom_input_response,
    ExchangeApiMsgType_t::delete_watch_list_custom_input_request,
    ExchangeApiMsgType_t::delete_watch_list_custom_input_response,
    ExchangeApiMsgType_t::get_bonds_index_request,
    ExchangeApiMsgType_t::get_bonds_index_response,
    ExchangeApiMsgType_t::calculate_customized_bond_data_request,
    ExchangeApiMsgType_t::calculate_customized_bond_data_response,
    ExchangeApiMsgType_t::get_hypothetical_bonds_request,
    ExchangeApiMsgType_t::get_hypothetical_bonds_response,
    ExchangeApiMsgType_t::update_hypothetical_bond_request,
    ExchangeApiMsgType_t::update_hypothetical_bond_response,
    ExchangeApiMsgType_t::delete_hypothetical_bond_request,
    ExchangeApiMsgType_t::delete_hypothetical_bond_response,
};

static std::map<SystemState_t, std::set<ExchangeApiMsgType_t>> AllowedStateMsgTypes_m
{
    {SystemState_t::OFFLINE, OfflineAllowedMsgTypes_s},
    {SystemState_t::OPEN, OpenAllowedMsgTypes_s},
    {SystemState_t::CLOSED, ClosedAllowedMsgTypes_s},
    {SystemState_t::CLOSED_EOD, EodClosedAllowedMsgTypes_s}
};
*/

#pragma GCC diagnostic pop

//inline std::ostream& operator<< (std::ostream &out, exchange::ExchangeApiMsgSubType_t val)  { return print<exchange::ExchangeApiMsgSubType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::ClientType_t val)             { return print<exchange::ClientType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::CancelType_t val)             { return print<exchange::CancelType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::CxlRejReason_t val)           { return print<exchange::CxlRejReason_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::CxlRejResponseTo_t val)       { return print<exchange::CxlRejResponseTo_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::EventType_t val)              { return print<exchange::EventType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::ExecInst_t val)               { return print<exchange::ExecInst_t>(out,val);  }
//inline std::ostream& operator<< (std::ostream &out, exchange::ExecType_t val)               { return print<exchange::ExecType_t>(out,val);  }
//inline std::ostream& operator<< (std::ostream &out, exchange::OpenClose_t val)              { return print<exchange::OpenClose_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::OrdRejReason_t val)           { return print<exchange::OrdRejReason_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::TradeRejReason_t val)         { return print<exchange::TradeRejReason_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::OrdStatus_t val)              { return print<exchange::OrdStatus_t>(out,val);    }
//inline std::ostream& operator<< (std::ostream &out, exchange::TradeStatus_t val)            { return print<exchange::TradeStatus_t>(out,val);  }
//inline std::ostream& operator<< (std::ostream &out, exchange::OrdType_t val)                { return print<exchange::OrdType_t>(out,val);      }
//inline std::ostream& operator<< (std::ostream &out, exchange::PossResend_t val)             { return print<exchange::PossResend_t>(out,val);   }
//inline std::ostream& operator<< (std::ostream &out, exchange::AutoCancel_t val)             { return print<exchange::AutoCancel_t>(out,val);   }
//inline std::ostream& operator<< (std::ostream &out, exchange::SecurityIdSource_t val)       { return print<exchange::SecurityIdSource_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::SecurityRequestResult_t val)  { return print<exchange::SecurityRequestResult_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::SecurityAssetType_t val)      { return print<exchange::SecurityAssetType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::SecurityTradingStatus_t val)  { return print<exchange::SecurityTradingStatus_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::Side_t val)                   { return print<exchange::Side_t>(out,val);        }
//inline std::ostream& operator<< (std::ostream &out, exchange::TimeInForce_t val)            { return print<exchange::TimeInForce_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::FirmType_t val)               { return print<exchange::FirmType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::SystemStatus_t val)           { return print<exchange::SystemStatus_t>(out,val);}
//inline std::ostream& operator<< (std::ostream &out, exchange::AccountStatus_t val)          { return print<exchange::AccountStatus_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::AccountStatusCode_t val)      { return print<exchange::AccountStatusCode_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::CpodxUserStatus_t val)        { return print<exchange::CpodxUserStatus_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::CpodSymbolType_t val)         { return print<exchange::CpodSymbolType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::AssetCategory_t val)          { return print<exchange::AssetCategory_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::FirmTradingStatus_t val)      { return print<exchange::FirmTradingStatus_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::ViewSortBy_t val)             { return print<exchange::ViewSortBy_t>(out,val);  }
//inline std::ostream& operator<< (std::ostream &out, exchange::ViewFilterBy_t val)           { return print<exchange::ViewFilterBy_t>(out,val);}
//inline std::ostream& operator<< (std::ostream &out, exchange::ViewDirection_t val)          { return print<exchange::ViewDirection_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::SessionType_t val)            { return print<exchange::SessionType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::MonthCode_t val)              { return print<exchange::MonthCode_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::ArchiveResendType_t val)      { return print<exchange::ArchiveResendType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::TimeScale_t val)              { return print<exchange::TimeScale_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::FmvUnderlyingSortBy_t val)    { return print<exchange::FmvUnderlyingSortBy_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::FmvUnderlyingFilterBy_t val)  { return print<exchange::FmvUnderlyingFilterBy_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::FmvGainerLoserSortBy_t val)   { return print<exchange::FmvGainerLoserSortBy_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::OpsCommandType_t val)         { return print<exchange::OpsCommandType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::OpsCommandStatus_t val)       { return print<exchange::OpsCommandStatus_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::DataField_t val)              { return print<exchange::DataField_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::BondCouponFrequency_t val)    { return print<exchange::BondCouponFrequency_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::BondCouponType_t val)         { return print<exchange::BondCouponType_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::BondDayCountConvention_t val) { return print<exchange::BondDayCountConvention_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::UserPermissions_t val)        { return print<exchange::UserPermissions_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::TradeEvent_t val)             { return print<exchange::TradeEvent_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::SystemState_t val)            { return print<exchange::SystemState_t>(out,val); }
//inline std::ostream& operator<< (std::ostream &out, exchange::TraderInfoSortBy_t val)       { return print<exchange::TraderInfoSortBy_t>(out,val);  }
}

#endif
