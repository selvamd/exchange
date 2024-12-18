#include <fstream>
#include <ostream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include "ux_selector.hh"
#include "TickerDomainObjects.hh"
#include "loginsvcclient.hpp"
#include <curl/curl.h>
#include "jsonxx.hpp"

DomainDB &stockdb = DomainDB::instance(1);
void curlreader(void *ptr, size_t size, size_t nmemb, void *uptr) { memcpy(uptr, ptr, size*nmemb); }

std::string stockstr(int32_t index, StockData_t d)
{
    auto stk = stockdb.lookupByIndex<StockLookup>(index);
    EnumData<MarketCap_t> data(stk->mktcap());

    switch (d)
    {
        case StockData_t::SECTOR:       return stockdb.strkey<SectorLookup>(stk->getSectorIndex());
        case StockData_t::INDUSRTY:     return stockdb.strkey<IndustryLookup>(stk->getIndustryIndex());
        case StockData_t::SYMBOL:       return stk->getName().c_str();
        case StockData_t::NAME:         return stk->getDescription().c_str();
        case StockData_t::MKTCAPSTR:    return data.toString();
        default: break;
    }
    return "";
}

double stockvalue(int32_t index, StockData_t d)
{
    auto stk = stockdb.lookupByIndex<StockLookup>(index);
    switch (d)
    {
        case StockData_t::IPOYR:        return stk->getIpoYear();
        case StockData_t::MKTCAP:       return stk->getMarketCap();
        case StockData_t::MKTPX:        return stk->getLastSale();
        case StockData_t::SHARES:       return (int)(stk->getMarketCap()/stk->getLastSale());
        case StockData_t::MKTPXCHG:     return 0;
        case StockData_t::MKTPXPCTCHG:  return 0;
        default: break;
    }
    return 0;
}

double stockvalue(int32_t index, int32_t evt, StockDistData_t d)
{
    StockTrend key;
    key.setStockIndex(index);
    key.setEventIndex(evt);
    auto trend = stockdb.lookup<StockTrend>(key);
    if (trend == nullptr) return 0;
    switch (d)
    {
        case StockDistData_t::MINPX:          return trend->getMinPrice();
        case StockDistData_t::MAXPX:          return trend->getMaxPrice();
        case StockDistData_t::MINPX6SIG:      return trend->getLastMktPrice() - 3.0 * trend->getSdTrendDev();
        case StockDistData_t::MAXPX6SIG:      return trend->getLastMktPrice() + 3.0 * trend->getSdTrendDev();
        case StockDistData_t::AVGPX:          return trend->getMeanPrice();
        case StockDistData_t::MEDPX:          return trend->getMedianPrice();
        case StockDistData_t::INITMOVAVGPX:   return trend->getInitAvgPrice();
        case StockDistData_t::ENDMOVAVGPX:    return trend->getLastAvgPrice();
        case StockDistData_t::MOVAVGYIELD:    return trend->getAnnualYield();
        case StockDistData_t::TRENDSD    :    return trend->getSdTrendDev();
        case StockDistData_t::TRENDSD2AVG:    return trend->getAbsVariance();
        case StockDistData_t::MKT2TRENDZ:     return trend->getLastMktTrendDevZ();
        case StockDistData_t::INITMKTCAP:     return trend->getInitAvgPrice() * stockvalue(index, StockData_t::SHARES);
        case StockDistData_t::ENDMKTCAP:      return trend->getLastAvgPrice() * stockvalue(index, StockData_t::SHARES);
        default: break;
    }
    return 0;
}

typedef struct expr
{
    StockData_t     data;
    StockDistData_t distdata;
    int32_t    dist = 0;
    char filter     = 0; //<,>,=,*
    std::string value;
    
    struct expr * mainexpr = nullptr;
    char joinoper = 0; // &|!^
    struct expr * joinexpr = nullptr;

    bool eval(int32_t index)
    {
        if (mainexpr != nullptr)
        {
            if (joinexpr != nullptr && joinoper == '&') return mainexpr->eval(index) && joinexpr->eval(index);
            if (joinexpr != nullptr && joinoper == '|') return mainexpr->eval(index) || joinexpr->eval(index);
            if (joinoper == '!') return !mainexpr->eval(index);
            std::cout << "cannot evaluate compound expression " << std::endl;
            return false;
        }
        if (dist == 0 && (data == StockData_t::SYMBOL || data == StockData_t::NAME ||
                data == StockData_t::SECTOR || data == StockData_t::INDUSRTY))
        {
            std::string s = stockstr(index, data);
            int i = s.compare(value);
            if (filter == '<') return i < 0;
            if (filter == '>') return i > 0;
            if (filter == '=') return i == 0;
            if (filter == '*')
                return strstr(s.c_str(), value.c_str()) != nullptr;
        }
        else
        {
            if (filter == '*') return true;
            double d = (dist == 0)? stockvalue(index, data):
                       stockvalue(index, dist, distdata);
            double d1 = strtod(value.c_str(),nullptr);
            if (filter == '<') return d < d1;
            if (filter == '>') return d > d1;
            if (filter == '=') return d == d1;
        }
        std::cout << "cannot evaluate expression " << std::endl;
        return false;
    }
    
    expr()
    {
        filter   = 0;
        joinexpr = mainexpr = nullptr;
    }

    expr(struct expr &e, char j)
    {
        filter   = 0;
        joinexpr = nullptr;
        joinoper = j;
        mainexpr = &e;
    }

    expr(StockData_t d, char f, std::string v)
    {
        joinexpr = mainexpr = nullptr;
        data = d; filter = f; value = v, dist = 0;
    }
    
    expr(StockDistData_t d, int32_t e, char f, std::string v)
    {
        joinexpr = mainexpr = nullptr;
        distdata = d; filter = f; value = v;dist = e;
    }

    void join(struct expr &e, char j)
    {
        joinexpr = &e; joinoper = j;
    }
    
    void join(struct expr &m, char j, struct expr &e)
    {
        mainexpr = &m;joinexpr = &e; joinoper = j;
    }
} Query;

//pick the best qualifying stock based on criteria.
void stock_picker(Query * filter, double (*weight)(int32_t), std::vector<int32_t> &result, int count = -1)
{
    result.clear();
    std::multimap<double, int32_t> gainers;
    DomainTable<StockLookup>::IndexIterator stkItr, stkItrN;
    stockdb.getTableIters<StockLookup>(stkItr,stkItrN);

    for (;stkItr != stkItrN;++stkItr)
    {
        auto stk = (*stkItr);
        if (filter == nullptr || filter->eval(stk->d_row))
            gainers.insert(std::pair<double,int32_t>(weight(stk->d_row),stk->d_row));
    }
    
    for (auto rit = gainers.rbegin(); rit!=gainers.rend(); ++rit)
    {
        if (count-- == 0) break;
        result.push_back(rit->second);
    }
}

std::string livepx(std::string stock)
{
    static CURL *curl = nullptr;
    static char userptr[1024];
    if (curl == nullptr)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        //curl_easy_cleanup(curl);
        //curl_global_cleanup();
    }
    memset(userptr,0,1024);
    std::string url = "http://finance.google.com/finance/info?client=ig&q=";
    url.append(stock);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlreader);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)userptr);
    curl_easy_perform(curl);
    jsonxx::Object obj;
    obj.parse(userptr);
    return obj.get<jsonxx::String>("l_cur");
}

void printStockHead()
{
    std::cout << std::setw(37) << "Description";
    std::cout << std::setw(15) << "OB_AVGYIELD";
    std::cout << std::setw(15) << "TR_AVGYIELD";
    std::cout << std::setw(15) << "INITMOVAVGPX";
    std::cout << std::setw(15) << "ENDMOVAVGPX";
    std::cout << std::setw(15) << "MKTCAP";
    //std::cout << std::setw(25) << "MEDPX YR3->YR->QTR";
    //std::cout << std::setw(20) << "YRPX-RANGE";
    //std::cout << std::setw(20) << "YRPX-6SIGONTREND";
    //std::cout << std::setw(10) << "YRYIELD";
    //std::cout << std::setw(10) << "YRTRENDZ";
    //std::cout << std::setw(10) << "YR-SD/DLR";
    //std::cout << std::setw(10) << "YR8YIELD";
    std::cout << std::endl;
}

void printStock(int stockindex)
{
    static auto time_ob = stockdb.fastindex<EventLookup>("OBAMA_PRESIDENCY");
    static auto time_tr = stockdb.fastindex<EventLookup>("TRUMP_PRESIDENCY");
    
    std::string str = stockstr(stockindex,StockData_t::SYMBOL);
    str.append(" (");
    str.append(stockstr(stockindex,StockData_t::SECTOR));
    str.append(")");
    if (str.length()>37) str = str.substr(0,35);
    std::cout << std::setw(37) << str;
    //LivePrice
    //std::cout << std::setw(10) << livepx(getStockStr(stockindex,StockData_t::SYMBOL));
    std::cout << std::setw(15) << stockvalue(stockindex, time_ob, StockDistData_t::MOVAVGYIELD);
    std::cout << std::setw(15) << stockvalue(stockindex, time_tr, StockDistData_t::MOVAVGYIELD);
    std::cout << std::setw(15) << stockvalue(stockindex, time_tr, StockDistData_t::INITMOVAVGPX);
    std::cout << std::setw(15) << stockvalue(stockindex, time_tr, StockDistData_t::ENDMOVAVGPX);
    std::cout << std::setw(15) << stockstr(stockindex, StockData_t::MKTCAPSTR);

    //Price
    char pricestr[75];
    memset(pricestr,0,75);
    sprintf(pricestr,"%.2f", stockvalue(stockindex,StockData_t::MKTPX));
    //std::cout << std::setw(10) << pricestr;

    std::cout << std::endl;
    std::cout << std::setw(37) << stockstr(stockindex,StockData_t::NAME).substr(0,35) << std::endl;
    std::cout << std::setw(37) << stockstr(stockindex,StockData_t::INDUSRTY).substr(0,35) << std::endl;
}

void initDb(std::string dbfile)
{
    stockdb.getTable<EventLookup>(nullptr,20*sizeof(EventLookup));
    stockdb.getTable<SectorLookup>(nullptr,50*sizeof(SectorLookup));
    stockdb.getTable<IndustryLookup>(nullptr,500*sizeof(IndustryLookup));
    stockdb.getTable<StockLookup>(nullptr,10000*sizeof(StockLookup));
    stockdb.getTable<StockTrend>(nullptr,100000*sizeof(StockTrend));
    stockdb.getTable<StockEodPrice>(nullptr,10000*sizeof(StockEodPrice));
    stockdb.load<EventLookup>(dbfile);
    stockdb.load<SectorLookup>(dbfile);
    stockdb.load<IndustryLookup>(dbfile);
    stockdb.load<StockLookup>(dbfile);
    stockdb.load<StockTrend>(dbfile);

    std::cout << "Sector count\t\t" << stockdb.getRecordCount<SectorLookup>()   << std::endl;
    std::cout << "Industry count\t\t" << stockdb.getRecordCount<IndustryLookup>() << std::endl;
    std::cout << "Stock count\t\t" << stockdb.getRecordCount<StockLookup>()    << std::endl;
    std::cout << "StockTrend count\t" << stockdb.getRecordCount<StockTrend>() << std::endl;
}

void favorites(std::string stocklist)
{
    std::string tickers = "";

    //******************* These are non-ishare general etfs ***********************************************
    //tickers.append("SPY,IVV,VTI,EFA,VOO,VWO,AGG,GLD,QQQ,VEA,VNQ,LQD,BND,EEM,IWF,IJH,IWD,IWM,VTV,VIG,VUG,IJR,").
    //        append("PFF,MDY,DVY,IWB,HYG,IEMG,XLF,VYM,VO,XLE,SDY,USMV,EWJ,VB,VCSH,VEU,IVW,IWR,XLK,XLV,JNK,IEFA,DIA,").
    //        append("VGK,CSJ,BIV,IVE,SHY,RSP,VCIT,DBEF,XLY,EMB,HEDJ,IAU,GDX,VGT,XLP,IEF,MBB,AMLP,EZU,EFAV,TLT,VBR,").
    //        append("XLU,IWS,VV,XLI,SPLV,MUB,CIU,SCHB,DXJ,IWN,SLV,HDV,IWO,VXUS,IWP,IEI,IWV,SCHF,SCHX,VT,SCZ,VHT,").
    //        append("IJK,BNDX,ACWI,VOE,IYR,BKLN,IJJ,VBK,TIP,BSV");
    //******************* These are non-ishare general etfs ***********************************************
    if (stocklist == "indices")
        tickers.append("IBB,IYW,IYH,IYF,IHI,IYE,ITB,IGM,ITA,IYJ,IYC,IYG,IDU,IGV,IYT,IYK,IHF,IHE,IYZ,").
                append("SOXX,IYM,IEO,IAT,IEZ,IAI,IAK,IGN,TCHF,MATF,FNCF,ERGF,HCRF,INDF,CNDF,UTLF,CNSF");

    if (stocklist == "energy")
        tickers = "DVN";

    if (stocklist == "outsource")
        tickers = "CTSH";

    if (stocklist == "consumer")
        tickers = "BURL,COST";

    if (stocklist == "transport")
        tickers = "";

    if (stocklist == "biotech")
        tickers.append("BIIB,GILD,AMGN,MDT,INCY,SGEN,IDXX,MTD,ILMN,Q,WAT,A,STJ");

    if (stocklist == "software")
        tickers = "ENV,INTU";

    if (stocklist == "exchanges")
        tickers = "MKTX,INFO,ICE,CBOE,CME";

    if (stocklist == "banks")
        tickers = "BAC,GS,MS,JPM,C";

    if (stocklist == "bigtechs")
        tickers = "AMZN,GOOG,MSFT,FB";

    if (stocklist == "trump")
        tickers = "AAPL,BMY,T,KMI,VZ,ETP,ALTR,IBM,COP,JPM,CAT";

    printStockHead();
    std::vector<std::string> stocks;
    tokenize(tickers.c_str(),',',stocks);
    for (auto &aStock: stocks)
    {
        StockLookup stkkey;stkkey.setName(aStock.c_str());
        auto stkrec = stockdb.lookup<StockLookup>(stkkey);
        if (stkrec == nullptr)
        {
            std::cout << "Stock not valid " << aStock << std::endl;
            continue;
        }
        printStock(stkrec->d_row);
        std::cout << std::endl;
    }
}

int32_t processMessage(const int &iClientID, const char *request, char *response)
{
    std::cout << "Request from " << iClientID << " for " << request << std::endl;
    memset(response,0,4096);
    sprintf(response,"%s\n", "Not implemented");
    return 0;
}

int processRequests()
{
    static ux_selector *server = nullptr;
    if (server == nullptr)
    {
        server = new ux_selector(0);
        server->AddServer(63600);
    }
    
    int iRC, iServerPort, iClientFD, iClientID;
    server->PollForSocketEvent();
    char request[4096], response[4096];
    
    while (true)
    {
        iRC = server->Accept(iServerPort, iClientFD);
        if ( iRC == ux_selector::END_OF_SOCK_LIST) break;
        if ( iRC == ux_selector::WOULD_BLOCK) continue;
        SocketStreamerBase * streamer = new SocketStreamerBase(iClientFD, 4096);
        iClientID = server->AddClient(streamer, 4096, 0, false);
    }
    
    while (true)
    {
        iRC = server->Read(iClientID,(char *)&request);
        if ( iRC == ux_selector::SUCCESS )
        {
            processMessage(iClientID, request, response);
            server->Write(iClientID, (char *)&response, strlen(response));
            server->flush();
        }
        else return (iRC == ux_selector::END_OF_SOCK_LIST)? 0:-1;
    }
    return 0;
}

int main(int, char **)
{
    cout.precision(3);
    initDb("stock_trend.dat");
    
    std::vector<int32_t> stocks;
    
    Query cap(StockData_t::MKTCAP, '>', "2000000000");
    Query sec(StockData_t::INDUSRTY, '*', "Biotechnology");
    Query filter;
    filter.join(cap,'&', sec);

    auto sortyield = [](int32_t index) {
        static auto evttr = stockdb.fastindex<EventLookup>("TRUMP_PRESIDENCY");
        static auto evtob = stockdb.fastindex<EventLookup>("OBAMA_PRESIDENCY");
        return stockvalue(index, evttr, StockDistData_t::MOVAVGYIELD) -
                std::max(0.0,stockvalue(index, evtob, StockDistData_t::MOVAVGYIELD));
    };
    
    stock_picker(&filter, sortyield, stocks);
    std::cout << stocks.size() << std::endl;
     //favorites("banks");
     //favorites("exchanges");
    printStockHead();
    for (auto index:stocks)
        printStock(index);

    favorites("banks");
    favorites("bigtechs");
    favorites("exchanges");
    favorites("trump");
/*
    loginsvcclient client("127.0.0.1",63600);
    while (true)
    {
        client.process();
        sleep(1);
    }
    //while (true)
    //{
    //    processRequests();
    //}
*/
}
