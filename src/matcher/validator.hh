#ifndef VALIDATOR_H
#define VALIDATOR_H
#include "DataTypes.hpp"
#include "DomainDatabase.hh"
#include "DomainDBListeners.hh"
#include "Enums.hh"
#include "MatcherDomainObjects.hh"
#include "ExchangeApi.hpp"
#include <cstring> 
#include "dbprocessor.hh"
#include "DataDictionary.hpp"

std::string validate_new_order(context &ctx, OrderLookup &ord) {
    auto req = ctx.request()->new_order_msg;;
    Timestamp now;
    now.set();
    ord.setCumQty(0);
    ord.setAvgPx(0);
    ord.setRankTime(now);
    ord.setArrivalTime(now);
    ord.setPrice(req.getPrice());
    ord.setOrderQty(req.getOrderQty());
    ord.setLeavesQty(req.getOrderQty());
    ord.setClOrdId(req.getClOrdId());
    ord.setOrdStatus(OrdStatus_t::NEW);
    ord.setSide(EnumData<Side_t>(req.toStringSide())());
    ord.setTimeInForce(EnumData<TimeInForce_t>(req.toStringTimeInForce())());
    ord.setOrdType(EnumData<OrdType_t>(req.toStringOrdType())());
    ord.setAutoCancel(EnumData<AutoCancel_t>(req.toStringAutoCancel())());
    ord.setClientType(EnumData<ClientType_t>(req.toStringClientType()));
    ord.setOrderLife(EnumData<OrderLife_t>(req.toStringOrderLife()));
    ord.setMinQtyInst(EnumData<MinQtyInst_t>(req.toStringMinQtyInst()));
    ord.setConditionalEligible(EnumData<ConditionalEligible_t>(req.toStringConditionalEligible()));
    ord.setSelfTradeInst(EnumData<SelfTradeInst_t>(req.toStringSelfTradeInst()));
    ord.setOrderUrgency(EnumData<OrderUrgency_t>(req.toStringOrderUrgency()));
    ord.setContraCategory(EnumData<ContraCategory_t>(req.toStringContraCategory()));
    ord.setRiskTier(EnumData<RiskTier_t>(req.toStringRiskTier()));
    ord.setOrdTypeExt(EnumData<OrdTypeExt_t>(req.toStringOrdTypeExt()));
    ord.setInviteId(req.getInviteId());
    ord.setMinQty(req.getMinQty());

    auto firm = findFirmByName(ctx, FirmRecordType_t::GATEWAY, req.getSenderCompId());
    if (firm == nullptr) return "Invalid sendercomp";
    ord.setSenderCompId(firm->d_row);

    firm = findFirmByName(ctx, FirmRecordType_t::USER_SESSION, req.getDeliverToCompId());
    if (firm == nullptr) return "Invalid deliverto";
    ord.setDeliverToCompId(firm->d_row);

    auto sym = findSymbol(ctx, req.getSymbol());
    if (sym == nullptr) return "Invalid symbol";
    ord.setSymbolIdx(sym->d_row);

    firm = findFirmByName(ctx, FirmRecordType_t::MPID, req.getPartyId());
    if (firm == nullptr) return "Invalid partyid/mpid";
    ord.setPartyId(firm->d_row);

    firm = findFirm(ctx, firm->getParentFirm());
    if (firm == nullptr) return "Invalid firmid";
    ord.setFirmId(firm->d_row);

    firm = findFirmByName(ctx, FirmRecordType_t::SUBID, req.getPartySubId());
    if (firm == nullptr) 
        firm = createSubID(ctx, req.getPartySubId(), ord.getPartyId(), ord.getFirmId());
    ord.setPartySubId(firm->d_row);

    ord.setOrderId(ctx.getNextOrderId());

    return "";
}

#endif
