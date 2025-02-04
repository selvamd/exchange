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
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end


/**
 * Checks if a segmentation match exists between two orders based on their urgency and contra category.
 * 
 * @param u1 The urgency level of the first order.
 * @param c1 The contra category of the first order.
 * @param u2 The urgency level of the second order.
 * @param c2 The contra category of the second order.
 * 
 * @return true if the combination of urgencies and contra categories is valid, false otherwise.
 */
bool check_segmentation_match(OrderUrgency_t u1, ContraCategory_t c1, OrderUrgency_t u2, ContraCategory_t c2)
{
    // LA	LA,LB,LC		
    // LB	LA,LB,LC,MA,MC	
    // LC	LA,LB,LC,MA,MC,HC
    // MA	LB,LC,MA,MC	
    // MC	LB,LC,MA,MC,HC
    // HC   LC,MC,HC    
    char val[] = { (char)('1'+(int)u1), (char)('1'+(int)c1), (char)('1'+(int)u2), (char)('1'+(int)c2), 0 };
    int ival = atoi(val);
    static int validvals[] = { 
        1111,1112,1113, 
        1211, 1212, 1213, 1221,1223,
        1311, 1312, 1313, 1321,1323, 1333,
        2112, 1213, 2121,1223, 
        2312, 2313, 2321,2323, 2333, 
        3313, 3323, 3333 
    };
    return std::find(std::begin(validvals), std::end(validvals), ival) != std::end(validvals);
}

/**
 * Check if the risk provider is eligible for Risk Offsetting 
 * for a given symbol and return true if eligible, false otherwise.
 */
bool check_risk_offsef(context &ctx, int32_t rpid, int32_t symbol, 
        int64_t minratio, int64_t maxratio)
{
    auto val = readUserConfig(ctx, ConfigName_t::RP_RO_ELIGIBLE, rpid);
    if (val == 0) return false; // Eligible by default (-1) or when set to 1
    auto rtval = readUserConfig(ctx, ConfigName_t::RP_RT_NOTIONAL, rpid, symbol);
    auto roval = readUserConfig(ctx, ConfigName_t::RP_RO_NOTIONAL, rpid, symbol);
    auto pause = readUserConfig(ctx, ConfigName_t::RP_RO_PAUSED, rpid, symbol);
    // upstream error becos when first rp order is received, it needs to be populated
    if (pause == -1 || rtval == -1 || roval == -1) return false;  
    if (pause == 1 && (100 * roval)/rtval < minratio) pause = 0;
    if (pause == 0 && (100 * roval)/rtval > maxratio) pause = 1;
    saveUserConfig(ctx, ConfigName_t::RP_RO_PAUSED, rpid, symbol, pause);
    return (pause == 0);
}

/**
 * Compares the priority of two orders.
 * If the client type of the two orders is the same, then:
 *   - If the orders are both investor orders, then the order with higher urgency
 *     has higher priority. If the urgency is the same, then the order with higher
 *     contra category has higher priority.
 *   - If the orders are both non-investor orders, then the order with higher risk
 *     tier has higher priority. If the risk tier is the same, then the order with
 *     higher quantity has higher priority.
 * If the client type of the two orders is different, then the order with lower
 * client type has higher priority.
 * 
 * @return -1 if ordc has lower priority than ordbk, 0 if they have the same
 * priority, 1 if ordbk has lower priority than ordc
 */
int32_t compare_priority(OrderLookup *ordc, OrderLookup *ordbk) 
{
    if (ordc->d_row == ordbk->d_row) return 0;
    if (ordc->getClientType()() > ordbk->getClientType()()) 
        return -1;

    if (ordc->getClientType()() == ClientType_t::INVESTOR) {
        if (ordc->getOrderUrgency()() < ordbk->getOrderUrgency()()) 
            return -1;
        if (ordc->getOrderUrgency()() == ordbk->getOrderUrgency()() && 
            ordc->getContraCategory()() < ordbk->getContraCategory()()) 
                return -1;
    } else {
        if (ordc->getRiskTier()() < ordbk->getRiskTier()())
            return -1;
        if (ordc->getOrderQty() < ordbk->getOrderQty())
            return -1;
    }
    return 1;
}

/**
 * Check if two orders can be matched based on conditional rules.
 * If ordIn is a conditional order, then it must be active to match further.
 * OrdBk is DAY and eligible for conditional matching.
 * If ordIn is a firmup order, it can match with non-co higher priority orders
 * If ordIn is a firm or active order, then it can be matched with ordBk
 * unless it is already locked in co-match and that co has higher priority.
 */
bool check_conditional_match(context &ctx, OrderLookup *ordin, OrderLookup *ordbk) {
    // conditional checks
    //CO=active/cowait,Firmup=cowait,Firm=active/cowait 
    if (ordin->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL) {
        if (ordbk->getTimeInForce()() != TimeInForce_t::DAY) 
            return false;
        if (ordbk->getConditionalEligible()() == ConditionalEligible_t::CO_ELIBIGLE_OFF) 
            return false;
        if (ordin->getOrderState()() == OrderState_t::CO_WAIT) 
            return false;
    } else if (ordin->getOrdTypeExt()() == OrdTypeExt_t::FIRMUP) {
        if (ordbk->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL) 
            return false;
        //find cowaiting order (lordc).
        //   if lordc firm, and has higher priority than lords, return false 
        //   if lordc a co, return false. 
        //   if lordc a firmup, and lordc != lords, return false 
        //   if lordc=n/a, allow trade 
        auto ordc = findConditionalOrder(ctx, ordin->getInviteId(), ordbk->getSide()());
        if (ordc == nullptr) 
            return true;
        // CO vs CO, where firmup has not happened on both sides. Just wait
        if (ordc->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL) 
            return false;
        // Firmup vs firmup, do not match if they are no part of same CO
        if (ordc->getOrdTypeExt()() == OrdTypeExt_t::FIRMUP) 
            return ordbk->d_row == ordc->d_row;
        // return false if ordbk has lower priority than my cowaiting co
        if (compare_priority(ordbk, ordc) < 0) 
            return false;
    } else {
        if (ordin->getOrderState()() == OrderState_t::CO_WAIT) {
            if (ordbk->getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL)
                return false;
            auto ordc = findConditionalOrder(ctx, ordin->getInviteId(), ordbk->getSide()());
            //if ordbk lower priority than my cowaitng co, then return false
            if (ordc != nullptr && compare_priority(ordbk, ordc) < 0) 
                return false;
        }
    }
    return true;
}

bool is_match_eligible(context &ctx, int32_t lordb, int32_t lords) 
{
    auto ordb = ctx.imdb.getTable<OrderLookup>().getObject(lordb);
    auto ords = ctx.imdb.getTable<OrderLookup>().getObject(lords);
    auto sym = ctx.imdb.getTable<SymbolLookup>().getObject(ordb->getSymbolIdx());

    if (ordb->getSide()() != Side_t::BUY) return false;
    if (ords->getSide()() != Side_t::SELL) return false;
    if (ordb->getSymbolIdx() != ords->getSymbolIdx()) return false;

    if (ordb->getOrderState()() != OrderState_t::MRI_WAIT) return false;
    if (ords->getOrderState()() != OrderState_t::MRI_WAIT) return false;

    if (ordb->getPrice() < ords->getPrice()) return false;
    if (sym->getNBBOBidPx() > sym->getNBBOAskPx()) return false;
    if (sym->getNBBOAskPx() == 0) return false;

    int64_t trdpx = (sym->getNBBOBidPx() + sym->getNBBOAskPx()) / 2;
    if (ordb->getPrice() < trdpx) return false;
    if (ords->getPrice() > trdpx) return false;

    if (ordb->getOrderQty() < ords->getMinQty()) return false;
    if (ords->getOrderQty() < ordb->getMinQty()) return false;

    if (ordb->getOrderQty() == ordb->getCumQty()) return false;
    if (ords->getOrderQty() == ords->getCumQty()) return false;

    // segmentation and self-trade check 
    if (ordb->getClientType()() != ords->getClientType()()) 
    {
        // check for RT eligibility
        auto val = (ordb->getClientType()() == ClientType_t::INVESTOR)?
            readUserConfig(ctx, ConfigName_t::SID_RT_ELIGIBLE, ordb->getPartySubId()):
            readUserConfig(ctx, ConfigName_t::SID_RT_ELIGIBLE, ords->getPartySubId());
        if (val == 0) return false; // low not eligible regardless of orderlife
        if (val < 2 && ordb->getOrderLife()() == OrderLife_t::FIRST) 
            return false; //includes med and n/a
    } else if (ordb->getClientType()() == ClientType_t::INVESTOR) {
        //self trade check
        if (ordb->getPartySubId() == ords->getPartySubId())
            if (ordb->getSelfTradeInst()() == SelfTradeInst_t::STOP_MPID_SUBID || 
                ords->getSelfTradeInst()() == SelfTradeInst_t::STOP_MPID_SUBID) 
                return false;
        if (!check_segmentation_match(ordb->getOrderUrgency()(), ordb->getContraCategory()(), 
            ords->getOrderUrgency()(), ords->getContraCategory()())) 
            return false;
    } else {
        //read min/max ratio
        auto ro2rtmin = readSystemConfig(ctx, ConfigName_t::SYM_RO2RT_MIN, ordb->getSymbolIdx());
        auto ro2rtmax = readSystemConfig(ctx, ConfigName_t::SYM_RO2RT_MAX, ordb->getSymbolIdx());
        //check risk offset eligibility
        if (!check_risk_offsef(ctx, ordb->getPartyId(), sym->d_row, ro2rtmin, ro2rtmax)) 
            return false;
        if (!check_risk_offsef(ctx, ords->getPartyId(), sym->d_row, ro2rtmin, ro2rtmax)) 
            return false;
    }
    if (check_conditional_match(ctx, ordb, ords) == false) 
        return false;  
    if (check_conditional_match(ctx, ords, ordb) == false) 
        return false;  
    return true;
}

std::string validate_new_order(context &ctx, OrderLookup &ord) {
    auto req = ctx.request()->new_order_msg;
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
    if (ord.getOrdType()() == OrdType_t::MARKET && ord.getSide()() == Side_t::BUY) 
        ord.setPrice(exchange::MAX_PRICE);
    if (ord.getOrdType()() == OrdType_t::MARKET && ord.getSide()() == Side_t::SELL) 
        ord.setPrice(exchange::MIN_PRICE);
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
    if (ord.getOrdTypeExt()() == OrdTypeExt_t::CONDITIONAL && 
        ord.getTimeInForce()() != TimeInForce_t::DAY)
            return "CO must have tif=day";

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

    //Adjust the urgency and contra category here based on urgency-level
    auto urglvl = readUserConfig(ctx, ConfigName_t::SID_URGENCY_LEVEL, ord.getPartySubId(), ord.getSymbolIdx());
    if (ord.getOrderUrgency()() == OrderUrgency_t::HIGH)
        ord.setContraCategory(ContraCategory_t::PARITY_PLUS_TWO);
    if (ord.getOrderUrgency()() == OrderUrgency_t::MEDIUM)
        if (ord.getContraCategory()() != ContraCategory_t::PARITY)
            ord.setContraCategory(ContraCategory_t::PARITY_PLUS_TWO);
    OrderUrgency_t urg = OrderUrgency_t::LOW;
    if (urglvl == 1) urg = OrderUrgency_t::MEDIUM;
    if (urglvl == 2) urg = OrderUrgency_t::HIGH;
    if (urglvl >= 0 && urg != ord.getOrderUrgency()()) 
    {
        if (urg == OrderUrgency_t::HIGH) 
            ord.setContraCategory(ContraCategory_t::PARITY_PLUS_TWO);
        if (urg == OrderUrgency_t::MEDIUM)
            if (ord.getContraCategory()() != ContraCategory_t::PARITY)
                ord.setContraCategory(ContraCategory_t::PARITY_PLUS_TWO);
        if (urg == OrderUrgency_t::LOW) 
            if (ord.getContraCategory()() != ContraCategory_t::PARITY || 
                ord.getOrderUrgency()() != OrderUrgency_t::MEDIUM)
                    ord.setContraCategory(ContraCategory_t::PARITY_PLUS_TWO);
        ord.setOrderUrgency(urg);
    } 

    ord.setOrderId(ctx.getNextOrderId());
    if (ord.getClientType()() == ClientType_t::RISK_PROVIDER) 
    {
        saveNewUserConfig(ctx, ConfigName_t::RP_RO_NOTIONAL, ord.getPartyId(), ord.getSymbolIdx(), 0);
        saveNewUserConfig(ctx, ConfigName_t::RP_RT_NOTIONAL, ord.getPartyId(), ord.getSymbolIdx(), 1);
        saveNewUserConfig(ctx, ConfigName_t::RP_RO_PAUSED, ord.getPartyId(), ord.getSymbolIdx(), 1);
    }

    return "";
}

#endif
