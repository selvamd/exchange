#pragma once

#include "SocketStreamerBase.hh"
#include <stdlib.h>
#include "DataDictionary.hpp"
#include "ExchangeApi.hpp"

class api_SocketStreamer : public SocketStreamerBase
{
public:
	//api_SocketStreamer();
	//api_SocketStreamer(int iSocket);
	//api_SocketStreamer(int iSocket, int iBufferSize);
	api_SocketStreamer() : SocketStreamerBase() { }
	api_SocketStreamer(int iSocket): SocketStreamerBase(iSocket) { }
	api_SocketStreamer(int iSocket, int iBufferSize): SocketStreamerBase(iSocket, iBufferSize) { }
	/*
	* Return messages size based on header of next message
	*/
	int iCalculateMsgSize()
	{
		exchange_api::ExchangeApiUnion * inmsg = (exchange_api::ExchangeApiUnion *) sMsgBuffer;
        exchange::ExchangeApiMsgType_t msg_type = (exchange::ExchangeApiMsgType_t) ntohl((int32_t)inmsg->api_msg_type);
		//std::cout << iPos << "," << iBytes << std::endl;
		switch(msg_type) {
			case exchange::ExchangeApiMsgType_t::gen_msg:
				return sizeof(exchange_api::GenMsg);
			case exchange::ExchangeApiMsgType_t::new_order:
				return sizeof(exchange_api::NewOrderMsg);
			case exchange::ExchangeApiMsgType_t::replace_order:
				return sizeof(exchange_api::ReplaceOrderMsg);
			case exchange::ExchangeApiMsgType_t::cancel:
				return sizeof(exchange_api::CancelMsg);
			case exchange::ExchangeApiMsgType_t::exec_report:
				return sizeof(exchange_api::ExecReportMsg);
			case exchange::ExchangeApiMsgType_t::cancel_reject:
				return sizeof(exchange_api::CancelRejectMsg);
			default:
				return ERROR_UNKNOWN_HEADER;
		}
	}
};


