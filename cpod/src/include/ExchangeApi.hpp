#ifndef _EXCHANGE_API_HPP_
#define _EXCHANGE_API_HPP_

/**
 * \brief Internal format messages.
 * \detail All components of the exchange will use these messages
 *         to communicate with each other.
 *
 */

#include <iostream>
#include <algorithm>
#include <string.h>
#include <arpa/inet.h>

#include "ExchangeTemplates.hpp"

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

namespace exchange_api
{

struct NewOrderMsg
{
    DECLARE_ENUM_FIELD(msg_type, MsgType, exchange::ExchangeApiMsgType_t)
    DECLARE_NUM_FIELD(msg_size, MsgSize, int32_t)
    DECLARE_NUM_FIELD(seq_num, SeqNum, int64_t)
    DECLARE_STRING_FIELD(sender_comp_id, SenderCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(target_comp_id, TargetCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_comp_id, OnBehalfOfCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_comp_id, DeliverToCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_location_id, OnBehalfOfLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_location_id, DeliverToLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_id, PartyId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_sub_id, PartySubId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(account, Account, exchange::COMP_ID_LENGTH)
    DECLARE_NUM_FIELD(cl_ord_id, ClOrdId, int64_t)
    DECLARE_ENUM_FIELD(exec_inst, ExecInst, exchange::ExecInst_t)
    DECLARE_NUM_FIELD(order_qty, OrderQty, int64_t)
    DECLARE_ENUM_FIELD(ord_type, OrdType, exchange::OrdType_t)
    DECLARE_PRICE_FIELD(price, Price, int64_t, exchange::PRICE_SCALE)
    DECLARE_TIME_FIELD(sending_time, SendingTime, int64_t)
    DECLARE_ENUM_FIELD(side, Side, exchange::Side_t)
    DECLARE_STRING_FIELD(symbol, Symbol, exchange::SYMBOL_LENGTH)
    DECLARE_ENUM_FIELD(time_in_force, TimeInForce, exchange::TimeInForce_t)
    DECLARE_NUM_FIELD(poss_resend, PossResend, int32_t)
    DECLARE_ENUM_FIELD(auto_cancel, AutoCancel, exchange::AutoCancel_t)
    DECLARE_PRICE_FIELD(stop_px, StopPx, int64_t, exchange::PRICE_SCALE)
    DECLARE_NUM_FIELD(max_floor, MaxFloor, int64_t)
    DECLARE_TIME_FIELD(expire_time, ExpireTime, int64_t)
    DECLARE_ENUM_FIELD(client_type, ClientType, exchange::ClientType_t)
    DECLARE_STRING_FIELD(trading_session_id, TradingSessionId, exchange::TRADE_SESSION_LENGTH)
    DECLARE_NUM_FIELD(no_trading_sessions, NoTradingSessions, int32_t)

    void reset()
    {
        memset((void *)this, 0, sizeof(*this));
        msg_type = exchange::ExchangeApiMsgType_t::new_order;
        msg_size = sizeof(*this);
    }

    void ntoh()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) ntohl((int32_t)msg_type);
        msg_size = ntohl(msg_size);
        seq_num = ntoh64(seq_num);
        cl_ord_id = ntoh64(cl_ord_id);
        exec_inst = (exchange::ExecInst_t) ntohl((int32_t)exec_inst);
        order_qty = ntoh64(order_qty);
        ord_type = (exchange::OrdType_t) ntohl((int32_t)ord_type);
        price = ntoh64(price);
        sending_time = ntoh64(sending_time);
        side = (exchange::Side_t) ntohl((int32_t)side);
        time_in_force = (exchange::TimeInForce_t) ntohl((int32_t)time_in_force);
        poss_resend = ntohl(poss_resend);
        auto_cancel = (exchange::AutoCancel_t) ntohl((int32_t)auto_cancel);
        stop_px = ntoh64(stop_px);
        max_floor = ntoh64(max_floor);
        expire_time = ntoh64(expire_time);
        client_type = (exchange::ClientType_t) ntohl((int32_t)client_type);
        no_trading_sessions = ntohl(no_trading_sessions);
    }

    void hton()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) htonl((int32_t)msg_type);
        msg_size = htonl(msg_size);
        seq_num = hton64(seq_num);
        cl_ord_id = hton64(cl_ord_id);
        exec_inst = (exchange::ExecInst_t) htonl((int32_t)exec_inst);
        order_qty = hton64(order_qty);
        ord_type = (exchange::OrdType_t) htonl((int32_t)ord_type);
        price = hton64(price);
        sending_time = hton64(sending_time);
        side = (exchange::Side_t) htonl((int32_t)side);
        time_in_force = (exchange::TimeInForce_t) htonl((int32_t)time_in_force);
        poss_resend = htonl(poss_resend);
        auto_cancel = (exchange::AutoCancel_t) htonl((int32_t)auto_cancel);
        stop_px = hton64(stop_px);
        max_floor = hton64(max_floor);
        expire_time = hton64(expire_time);
        client_type = (exchange::ClientType_t) htonl((int32_t)client_type);
        no_trading_sessions = htonl(no_trading_sessions);
    }

    friend std::ostream & operator<<(std::ostream & out, const NewOrderMsg& msg)
    {
        out << "NewOrderMsg";
        msg.printMsgType(out);
        msg.printMsgSize(out);
        msg.printSeqNum(out);
        msg.printSenderCompId(out);
        msg.printTargetCompId(out);
        msg.printOnBehalfOfCompId(out);
        msg.printDeliverToCompId(out);
        msg.printOnBehalfOfLocationId(out);
        msg.printDeliverToLocationId(out);
        msg.printPartyId(out);
        msg.printPartySubId(out);
        msg.printAccount(out);
        msg.printClOrdId(out);
        msg.printExecInst(out);
        msg.printOrderQty(out);
        msg.printOrdType(out);
        msg.printPrice(out);
        msg.printSendingTime(out);
        msg.printSide(out);
        msg.printSymbol(out);
        msg.printTimeInForce(out);
        msg.printPossResend(out);
        msg.printAutoCancel(out);
        msg.printStopPx(out);
        msg.printMaxFloor(out);
        msg.printExpireTime(out);
        msg.printClientType(out);
        msg.printTradingSessionId(out);
        msg.printNoTradingSessions(out);
        return out;
    }
};

struct ReplaceOrderMsg
{
    DECLARE_ENUM_FIELD(msg_type, MsgType, exchange::ExchangeApiMsgType_t)
    DECLARE_NUM_FIELD(msg_size, MsgSize, int32_t)
    DECLARE_NUM_FIELD(seq_num, SeqNum, int64_t)
    DECLARE_STRING_FIELD(sender_comp_id, SenderCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(target_comp_id, TargetCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_comp_id, OnBehalfOfCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_comp_id, DeliverToCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_location_id, OnBehalfOfLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_location_id, DeliverToLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_id, PartyId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_sub_id, PartySubId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(account, Account, exchange::COMP_ID_LENGTH)
    DECLARE_NUM_FIELD(cl_ord_id, ClOrdId, int64_t)
    DECLARE_ENUM_FIELD(exec_inst, ExecInst, exchange::ExecInst_t)
    DECLARE_NUM_FIELD(order_qty, OrderQty, int64_t)
    DECLARE_ENUM_FIELD(ord_type, OrdType, exchange::OrdType_t)
    DECLARE_PRICE_FIELD(price, Price, int64_t, exchange::PRICE_SCALE)
    DECLARE_TIME_FIELD(sending_time, SendingTime, int64_t)
    DECLARE_ENUM_FIELD(side, Side, exchange::Side_t)
    DECLARE_STRING_FIELD(symbol, Symbol, exchange::SYMBOL_LENGTH)
    DECLARE_ENUM_FIELD(time_in_force, TimeInForce, exchange::TimeInForce_t)
    DECLARE_NUM_FIELD(poss_resend, PossResend, int32_t)
    DECLARE_ENUM_FIELD(auto_cancel, AutoCancel, exchange::AutoCancel_t)
    DECLARE_PRICE_FIELD(stop_px, StopPx, int64_t, exchange::PRICE_SCALE)
    DECLARE_NUM_FIELD(max_floor, MaxFloor, int64_t)
    DECLARE_TIME_FIELD(expire_time, ExpireTime, int64_t)
    DECLARE_ENUM_FIELD(client_type, ClientType, exchange::ClientType_t)
    DECLARE_STRING_FIELD(trading_session_id, TradingSessionId, exchange::TRADE_SESSION_LENGTH)
    DECLARE_NUM_FIELD(no_trading_sessions, NoTradingSessions, int32_t)
    DECLARE_NUM_FIELD(order_id, OrderId, int64_t)
    DECLARE_NUM_FIELD(orig_cl_ord_id, OrigClOrdId, int64_t)

    void reset()
    {
        memset((void *)this, 0, sizeof(*this));
        msg_type = exchange::ExchangeApiMsgType_t::replace_order;
        msg_size = sizeof(*this);
    }

    void ntoh()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) ntohl((int32_t)msg_type);
        msg_size = ntohl(msg_size);
        seq_num = ntoh64(seq_num);
        cl_ord_id = ntoh64(cl_ord_id);
        exec_inst = (exchange::ExecInst_t) ntohl((int32_t)exec_inst);
        order_qty = ntoh64(order_qty);
        ord_type = (exchange::OrdType_t) ntohl((int32_t)ord_type);
        price = ntoh64(price);
        sending_time = ntoh64(sending_time);
        side = (exchange::Side_t) ntohl((int32_t)side);
        time_in_force = (exchange::TimeInForce_t) ntohl((int32_t)time_in_force);
        poss_resend = ntohl(poss_resend);
        auto_cancel = (exchange::AutoCancel_t) ntohl((int32_t)auto_cancel);
        stop_px = ntoh64(stop_px);
        max_floor = ntoh64(max_floor);
        expire_time = ntoh64(expire_time);
        client_type = (exchange::ClientType_t) ntohl((int32_t)client_type);
        no_trading_sessions = ntohl(no_trading_sessions);
        order_id = ntoh64(order_id);
        orig_cl_ord_id = ntoh64(orig_cl_ord_id);
    }

    void hton()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) htonl((int32_t)msg_type);
        msg_size = htonl(msg_size);
        seq_num = hton64(seq_num);
        cl_ord_id = hton64(cl_ord_id);
        exec_inst = (exchange::ExecInst_t) htonl((int32_t)exec_inst);
        order_qty = hton64(order_qty);
        ord_type = (exchange::OrdType_t) htonl((int32_t)ord_type);
        price = hton64(price);
        sending_time = hton64(sending_time);
        side = (exchange::Side_t) htonl((int32_t)side);
        time_in_force = (exchange::TimeInForce_t) htonl((int32_t)time_in_force);
        poss_resend = htonl(poss_resend);
        auto_cancel = (exchange::AutoCancel_t) htonl((int32_t)auto_cancel);
        stop_px = hton64(stop_px);
        max_floor = hton64(max_floor);
        expire_time = hton64(expire_time);
        client_type = (exchange::ClientType_t) htonl((int32_t)client_type);
        no_trading_sessions = htonl(no_trading_sessions);
        order_id = hton64(order_id);
        orig_cl_ord_id = hton64(orig_cl_ord_id);
    }

    friend std::ostream & operator<<(std::ostream & out, const ReplaceOrderMsg& msg)
    {
        out << "ReplaceOrderMsg";
        msg.printMsgType(out);
        msg.printMsgSize(out);
        msg.printSeqNum(out);
        msg.printSenderCompId(out);
        msg.printTargetCompId(out);
        msg.printOnBehalfOfCompId(out);
        msg.printDeliverToCompId(out);
        msg.printOnBehalfOfLocationId(out);
        msg.printDeliverToLocationId(out);
        msg.printPartyId(out);
        msg.printPartySubId(out);
        msg.printAccount(out);
        msg.printClOrdId(out);
        msg.printExecInst(out);
        msg.printOrderQty(out);
        msg.printOrdType(out);
        msg.printPrice(out);
        msg.printSendingTime(out);
        msg.printSide(out);
        msg.printSymbol(out);
        msg.printTimeInForce(out);
        msg.printPossResend(out);
        msg.printAutoCancel(out);
        msg.printStopPx(out);
        msg.printMaxFloor(out);
        msg.printExpireTime(out);
        msg.printClientType(out);
        msg.printTradingSessionId(out);
        msg.printNoTradingSessions(out);
        msg.printOrderId(out);
        msg.printOrigClOrdId(out);
        return out;
    }
};

struct CancelMsg
{
    DECLARE_ENUM_FIELD(msg_type, MsgType, exchange::ExchangeApiMsgType_t)
    DECLARE_NUM_FIELD(msg_size, MsgSize, int32_t)
    DECLARE_NUM_FIELD(seq_num, SeqNum, int64_t)
    DECLARE_STRING_FIELD(sender_comp_id, SenderCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(target_comp_id, TargetCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_comp_id, OnBehalfOfCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_comp_id, DeliverToCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_location_id, OnBehalfOfLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_location_id, DeliverToLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_id, PartyId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_sub_id, PartySubId, exchange::COMP_ID_LENGTH)
    DECLARE_NUM_FIELD(cl_ord_id, ClOrdId, int64_t)
    DECLARE_NUM_FIELD(sending_time, SendingTime, int64_t)
    DECLARE_STRING_FIELD(symbol, Symbol, exchange::SYMBOL_LENGTH)
    DECLARE_NUM_FIELD(poss_resend, PossResend, int32_t)
    DECLARE_NUM_FIELD(order_id, OrderId, int64_t)
    DECLARE_NUM_FIELD(orig_cl_ord_id, OrigClOrdId, int64_t)

    void reset()
    {
        memset((void *)this, 0, sizeof(*this));
        msg_type = exchange::ExchangeApiMsgType_t::cancel;
        msg_size = sizeof(*this);
    }

    void ntoh()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) ntohl((int32_t)msg_type);
        msg_size = ntohl(msg_size);
        seq_num = ntoh64(seq_num);
        cl_ord_id = ntoh64(cl_ord_id);
        sending_time = ntoh64(sending_time);
        poss_resend = ntohl(poss_resend);
        order_id = ntoh64(order_id);
        orig_cl_ord_id = ntoh64(orig_cl_ord_id);
    }

    void hton()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) htonl((int32_t)msg_type);
        msg_size = htonl(msg_size);
        seq_num = hton64(seq_num);
        cl_ord_id = hton64(cl_ord_id);
        sending_time = hton64(sending_time);
        poss_resend = htonl(poss_resend);
        order_id = hton64(order_id);
        orig_cl_ord_id = hton64(orig_cl_ord_id);
    }

    friend std::ostream & operator<<(std::ostream & out, const CancelMsg& msg)
    {
        out << "CancelMsg";
        msg.printMsgType(out);
        msg.printMsgSize(out);
        msg.printSeqNum(out);
        msg.printSenderCompId(out);
        msg.printTargetCompId(out);
        msg.printOnBehalfOfCompId(out);
        msg.printDeliverToCompId(out);
        msg.printOnBehalfOfLocationId(out);
        msg.printDeliverToLocationId(out);
        msg.printPartyId(out);
        msg.printPartySubId(out);
        msg.printClOrdId(out);
        msg.printSendingTime(out);
        msg.printSymbol(out);
        msg.printPossResend(out);
        msg.printOrderId(out);
        msg.printOrigClOrdId(out);
        return out;
    }
};

struct ExecReportMsg
{
    DECLARE_ENUM_FIELD(msg_type, MsgType, exchange::ExchangeApiMsgType_t)
    DECLARE_NUM_FIELD(msg_size, MsgSize, int32_t)
    DECLARE_NUM_FIELD(seq_num, SeqNum, int64_t)
    DECLARE_STRING_FIELD(sender_comp_id, SenderCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(target_comp_id, TargetCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_comp_id, OnBehalfOfCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_comp_id, DeliverToCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_location_id, OnBehalfOfLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_location_id, DeliverToLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_id, PartyId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_sub_id, PartySubId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(account, Account, exchange::COMP_ID_LENGTH)
    DECLARE_NUM_FIELD(cl_ord_id, ClOrdId, int64_t)
    DECLARE_ENUM_FIELD(exec_inst, ExecInst, exchange::ExecInst_t)
    DECLARE_NUM_FIELD(order_qty, OrderQty, int64_t)
    DECLARE_ENUM_FIELD(ord_type, OrdType, exchange::OrdType_t)
    DECLARE_PRICE_FIELD(price, Price, int64_t, exchange::PRICE_SCALE)
    DECLARE_TIME_FIELD(sending_time, SendingTime, int64_t)
    DECLARE_ENUM_FIELD(side, Side, exchange::Side_t)
    DECLARE_STRING_FIELD(symbol, Symbol, exchange::SYMBOL_LENGTH)
    DECLARE_ENUM_FIELD(time_in_force, TimeInForce, exchange::TimeInForce_t)
    DECLARE_NUM_FIELD(poss_resend, PossResend, int32_t)
    DECLARE_ENUM_FIELD(auto_cancel, AutoCancel, exchange::AutoCancel_t)
    DECLARE_PRICE_FIELD(stop_px, StopPx, int64_t, exchange::PRICE_SCALE)
    DECLARE_NUM_FIELD(max_floor, MaxFloor, int64_t)
    DECLARE_TIME_FIELD(expire_time, ExpireTime, int64_t)
    DECLARE_ENUM_FIELD(client_type, ClientType, exchange::ClientType_t)
    DECLARE_STRING_FIELD(trading_session_id, TradingSessionId, exchange::TRADE_SESSION_LENGTH)
    DECLARE_NUM_FIELD(no_trading_sessions, NoTradingSessions, int32_t)
    DECLARE_PRICE_FIELD(avg_px, AvgPx, int64_t, exchange::PRICE_SCALE)
    DECLARE_NUM_FIELD(cum_qty, CumQty, int64_t)
    DECLARE_NUM_FIELD(exec_id, ExecId, int64_t)
    DECLARE_NUM_FIELD(exec_ref_id, ExecRefId, int64_t)
    DECLARE_ENUM_FIELD(exec_type, ExecType, exchange::ExecType_t)
    DECLARE_PRICE_FIELD(last_px, LastPx, int64_t, exchange::PRICE_SCALE)
    DECLARE_NUM_FIELD(last_shares, LastShares, int64_t)
    DECLARE_NUM_FIELD(order_id, OrderId, int64_t)
    DECLARE_NUM_FIELD(orig_cl_ord_id, OrigClOrdId, int64_t)
    DECLARE_NUM_FIELD(trade_id, TradeId, int64_t)
    DECLARE_ENUM_FIELD(ord_status, OrdStatus, exchange::OrdStatus_t)
    DECLARE_STRING_FIELD(security_id, SecurityId, exchange::SYMBOL_LENGTH)
    DECLARE_STRING_FIELD(text, Text, exchange::TEXT_LENGTH)
    DECLARE_TIME_FIELD(transact_time, TransactTime, int64_t)
    DECLARE_ENUM_FIELD(open_close, OpenClose, exchange::OpenClose_t)
    DECLARE_ENUM_FIELD(ord_rej_reason, OrdRejReason, exchange::OrdRejReason_t)

    void reset()
    {
        memset((void *)this, 0, sizeof(*this));
        msg_type = exchange::ExchangeApiMsgType_t::exec_report;
        msg_size = sizeof(*this);
    }

    void ntoh()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) ntohl((int32_t)msg_type);
        msg_size = ntohl(msg_size);
        seq_num = ntoh64(seq_num);
        cl_ord_id = ntoh64(cl_ord_id);
        exec_inst = (exchange::ExecInst_t) ntohl((int32_t)exec_inst);
        order_qty = ntoh64(order_qty);
        ord_type = (exchange::OrdType_t) ntohl((int32_t)ord_type);
        price = ntoh64(price);
        sending_time = ntoh64(sending_time);
        side = (exchange::Side_t) ntohl((int32_t)side);
        time_in_force = (exchange::TimeInForce_t) ntohl((int32_t)time_in_force);
        poss_resend = ntohl(poss_resend);
        auto_cancel = (exchange::AutoCancel_t) ntohl((int32_t)auto_cancel);
        stop_px = ntoh64(stop_px);
        max_floor = ntoh64(max_floor);
        expire_time = ntoh64(expire_time);
        client_type = (exchange::ClientType_t) ntohl((int32_t)client_type);
        no_trading_sessions = ntohl(no_trading_sessions);
        avg_px = ntoh64(avg_px);
        cum_qty = ntoh64(cum_qty);
        exec_id = ntoh64(exec_id);
        exec_ref_id = ntoh64(exec_ref_id);
        exec_type = (exchange::ExecType_t) ntohl((int32_t)exec_type);
        last_px = ntoh64(last_px);
        last_shares = ntoh64(last_shares);
        order_id = ntoh64(order_id);
        orig_cl_ord_id = ntoh64(orig_cl_ord_id);
        trade_id = ntoh64(trade_id);
        ord_status = (exchange::OrdStatus_t) ntohl((int32_t)ord_status);
        transact_time = ntoh64(transact_time);
        open_close = (exchange::OpenClose_t) ntohl((int32_t)open_close);
        ord_rej_reason = (exchange::OrdRejReason_t) ntohl((int32_t)ord_rej_reason);
    }

    void hton()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) htonl((int32_t)msg_type);
        msg_size = htonl(msg_size);
        seq_num = hton64(seq_num);
        cl_ord_id = hton64(cl_ord_id);
        exec_inst = (exchange::ExecInst_t) htonl((int32_t)exec_inst);
        order_qty = hton64(order_qty);
        ord_type = (exchange::OrdType_t) htonl((int32_t)ord_type);
        price = hton64(price);
        sending_time = hton64(sending_time);
        side = (exchange::Side_t) htonl((int32_t)side);
        time_in_force = (exchange::TimeInForce_t) htonl((int32_t)time_in_force);
        poss_resend = htonl(poss_resend);
        auto_cancel = (exchange::AutoCancel_t) htonl((int32_t)auto_cancel);
        stop_px = hton64(stop_px);
        max_floor = hton64(max_floor);
        expire_time = hton64(expire_time);
        client_type = (exchange::ClientType_t) htonl((int32_t)client_type);
        no_trading_sessions = htonl(no_trading_sessions);
        avg_px = hton64(avg_px);
        cum_qty = hton64(cum_qty);
        exec_id = hton64(exec_id);
        exec_ref_id = hton64(exec_ref_id);
        exec_type = (exchange::ExecType_t) htonl((int32_t)exec_type);
        last_px = hton64(last_px);
        last_shares = hton64(last_shares);
        order_id = hton64(order_id);
        orig_cl_ord_id = hton64(orig_cl_ord_id);
        trade_id = hton64(trade_id);
        ord_status = (exchange::OrdStatus_t) htonl((int32_t)ord_status);
        transact_time = hton64(transact_time);
        open_close = (exchange::OpenClose_t) htonl((int32_t)open_close);
        ord_rej_reason = (exchange::OrdRejReason_t) htonl((int32_t)ord_rej_reason);
    }

    friend std::ostream & operator<<(std::ostream & out, const ExecReportMsg& msg)
    {
        out << "ExecReportMsg";
        msg.printMsgType(out);
        msg.printMsgSize(out);
        msg.printSeqNum(out);
        msg.printSenderCompId(out);
        msg.printTargetCompId(out);
        msg.printOnBehalfOfCompId(out);
        msg.printDeliverToCompId(out);
        msg.printOnBehalfOfLocationId(out);
        msg.printDeliverToLocationId(out);
        msg.printPartyId(out);
        msg.printPartySubId(out);
        msg.printAccount(out);
        msg.printClOrdId(out);
        msg.printExecInst(out);
        msg.printOrderQty(out);
        msg.printOrdType(out);
        msg.printPrice(out);
        msg.printSendingTime(out);
        msg.printSide(out);
        msg.printSymbol(out);
        msg.printTimeInForce(out);
        msg.printPossResend(out);
        msg.printStopPx(out);
        msg.printMaxFloor(out);
        msg.printExpireTime(out);
        msg.printClientType(out);
        msg.printTradingSessionId(out);
        msg.printNoTradingSessions(out);
        msg.printAvgPx(out);
        msg.printCumQty(out);
        msg.printExecId(out);
        msg.printExecRefId(out);
        msg.printExecType(out);
        msg.printLastPx(out);
        msg.printLastShares(out);
        msg.printOrderId(out);
        msg.printOrigClOrdId(out);
        msg.printTradeId(out);
        msg.printOrdStatus(out);
        msg.printSecurityId(out);
        msg.printText(out);
        msg.printTransactTime(out);
        msg.printOpenClose(out);
        msg.printOrdRejReason(out);
        return out;
    }
};

struct CancelRejectMsg
{
    DECLARE_ENUM_FIELD(msg_type, MsgType, exchange::ExchangeApiMsgType_t)
    DECLARE_NUM_FIELD(msg_size, MsgSize, int32_t)
    DECLARE_NUM_FIELD(seq_num, SeqNum, int64_t)
    DECLARE_STRING_FIELD(sender_comp_id, SenderCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(target_comp_id, TargetCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_comp_id, OnBehalfOfCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_comp_id, DeliverToCompId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(on_behalf_of_location_id, OnBehalfOfLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(deliver_to_location_id, DeliverToLocationId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_id, PartyId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(party_sub_id, PartySubId, exchange::COMP_ID_LENGTH)
    DECLARE_STRING_FIELD(account, Account, exchange::COMP_ID_LENGTH)
    DECLARE_NUM_FIELD(cl_ord_id, ClOrdId, int64_t)
    DECLARE_ENUM_FIELD(ord_status, OrdStatus, exchange::OrdStatus_t)
    DECLARE_NUM_FIELD(orig_cl_ord_id, OrigClOrdId, int64_t)
    DECLARE_STRING_FIELD(symbol, Symbol, exchange::SYMBOL_LENGTH)
    DECLARE_NUM_FIELD(order_id, OrderId, int64_t)
    DECLARE_STRING_FIELD(text, Text, exchange::TEXT_LENGTH)
    DECLARE_ENUM_FIELD(cxl_rej_reason, CxlRejReason, exchange::CxlRejReason_t)
    DECLARE_ENUM_FIELD(cxl_rej_response_to, CxlRejResponseTo, exchange::CxlRejResponseTo_t)

    void reset()
    {
        memset((void *)this, 0, sizeof(*this));
        msg_type = exchange::ExchangeApiMsgType_t::cancel_reject;
        msg_size = sizeof(*this);
    }

    void ntoh()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) ntohl((int32_t)msg_type);
        msg_size = ntohl(msg_size);
        seq_num = ntoh64(seq_num);
        cl_ord_id = ntoh64(cl_ord_id);
        ord_status = (exchange::OrdStatus_t) ntohl((int32_t)ord_status);
        orig_cl_ord_id = ntoh64(orig_cl_ord_id);
        order_id = ntoh64(order_id);
        cxl_rej_reason = (exchange::CxlRejReason_t) ntohl((int32_t)cxl_rej_reason);
        cxl_rej_response_to = (exchange::CxlRejResponseTo_t) ntohl((int32_t)cxl_rej_response_to);
    }

    void hton()
    {
        msg_type = (exchange::ExchangeApiMsgType_t) htonl((int32_t)msg_type);
        msg_size = htonl(msg_size);
        seq_num = hton64(seq_num);
        cl_ord_id = hton64(cl_ord_id);
        ord_status = (exchange::OrdStatus_t) htonl((int32_t)ord_status);
        orig_cl_ord_id = hton64(orig_cl_ord_id);
        order_id = hton64(order_id);
        cxl_rej_reason = (exchange::CxlRejReason_t) htonl((int32_t)cxl_rej_reason);
        cxl_rej_response_to = (exchange::CxlRejResponseTo_t) htonl((int32_t)cxl_rej_response_to);
    }

    friend std::ostream & operator<<(std::ostream & out, const CancelRejectMsg& msg)
    {
        out << "CancelRejectMsg";
        msg.printMsgType(out);
        msg.printMsgSize(out);
        msg.printSeqNum(out);
        msg.printSenderCompId(out);
        msg.printTargetCompId(out);
        msg.printOnBehalfOfCompId(out);
        msg.printDeliverToCompId(out);
        msg.printOnBehalfOfLocationId(out);
        msg.printDeliverToLocationId(out);
        msg.printPartyId(out);
        msg.printPartySubId(out);
        msg.printAccount(out);
        msg.printClOrdId(out);
        msg.printOrdStatus(out);
        msg.printOrigClOrdId(out);
        msg.printSymbol(out);
        msg.printOrderId(out);
        msg.printText(out);
        msg.printCxlRejReason(out);
        msg.printCxlRejResponseTo(out);
        return out;
    }
};

union ExchangeApiUnion
{
    exchange::ExchangeApiMsgType_t api_msg_type;
    struct GenMsg gen_msg;
    // Trading API Messages
    struct NewOrderMsg new_order_msg;
    struct ReplaceOrderMsg replace_order_msg;
    struct CancelMsg cancel_msg;
    struct ExecReportMsg exec_report_msg;
    struct CancelRejectMsg cancel_reject;

    int32_t hton()
    {
        int32_t rc = 0;
        switch(api_msg_type)
        {
            case exchange::ExchangeApiMsgType_t::gen_msg :
                gen_msg.hton();
                break;
            case exchange::ExchangeApiMsgType_t::new_order :
                new_order_msg.hton();
                break;
            case exchange::ExchangeApiMsgType_t::replace_order :
                replace_order_msg.hton();
                break;
            case exchange::ExchangeApiMsgType_t::cancel  :
                cancel_msg.hton();
                break;
            case exchange::ExchangeApiMsgType_t::exec_report  :
                exec_report_msg.hton();
                break;
            case exchange::ExchangeApiMsgType_t::cancel_reject  :
                cancel_reject.hton();
                break;
            default:
                rc = -1;
                break;
        }

        return rc;
    }

    int32_t ntoh()
    {
        exchange::ExchangeApiMsgType_t msg_type = (exchange::ExchangeApiMsgType_t) ntohl((int32_t)api_msg_type);

        int32_t rc = 0;
        switch(msg_type)
        {
            case exchange::ExchangeApiMsgType_t::gen_msg :
                gen_msg.ntoh();
                break;
            case exchange::ExchangeApiMsgType_t::new_order :
                new_order_msg.ntoh();
                break;
            case exchange::ExchangeApiMsgType_t::replace_order :
                replace_order_msg.ntoh();
                break;
            case exchange::ExchangeApiMsgType_t::cancel  :
                cancel_msg.ntoh();
                break;
            case exchange::ExchangeApiMsgType_t::exec_report  :
                exec_report_msg.ntoh();
                break;
            case exchange::ExchangeApiMsgType_t::cancel_reject  :
                cancel_reject.ntoh();
                break;
            default:
                rc = -1;
                break;
        }

        return rc;
    };

} // end of exchange_api namespace


#pragma pack(pop)   /* restore original alignment from stack */

#endif
