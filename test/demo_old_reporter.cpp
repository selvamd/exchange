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


DEFINE_ENUM_TYPES(LEVEL,ALL,SECTOR,INDUSTRY,MKTCAP,SINGLESTOCK)

//pick the best qualifying stock based on criteria.
void stock_picker(int N, LEVEL level, std::string filter,
        void (*pick)(int,LEVEL,int32_t,std::vector<int32_t> &, void *),
        void * params,
        std::vector<int32_t> &result)
{
    if (N <= 0 ) return;
    result.clear();

    if (level == LEVEL::SECTOR)
    {
        DomainTable<SectorLookup>::IndexIterator itr, itrN;
        stockdb.getTableIters<SectorLookup>(itr,itrN);
        for (;itr != itrN;++itr)
            if (filter.size() == 0 || strstr((*itr)->getName().c_str(), filter.c_str()) != nullptr)
                pick(N, level, (*itr)->d_row, result, params);

        return;
    }
    else if (level == LEVEL::INDUSTRY)
    {
        DomainTable<IndustryLookup>::IndexIterator itr, itrN;
        stockdb.getTableIters<IndustryLookup>(itr,itrN);
        for (;itr != itrN;++itr)
            if (filter.size() == 0 || strstr((*itr)->getName().c_str(), filter.c_str())!=nullptr)
                pick(N, level, (*itr)->d_row, result, params);
        return;
    }
    else if (level == LEVEL::MKTCAP)
    {
        if (filter.size() == 0)
        {
            pick(N, level, (int)MarketCap_t::PINK, result, params);
            pick(N, level, (int)MarketCap_t::PENNY, result, params);
            pick(N, level, (int)MarketCap_t::SMALLCAP, result, params);
            pick(N, level, (int)MarketCap_t::MEDIUMCAP, result, params);
            pick(N, level, (int)MarketCap_t::LARGECAP, result, params);
            pick(N, level, (int)MarketCap_t::BLUECHIP, result, params);
            pick(N, level, (int)MarketCap_t::GIANT, result, params);
        }
        else
        {
            EnumData<MarketCap_t> data = filter;
            if (data.isValid())
                pick(N, level, (int)data(), result, params);
        }
        return;
    }
    else if (level == LEVEL::SINGLESTOCK)
    {
        StockLookup stkkey;stkkey.setName(filter.c_str());
        auto stkrec = stockdb.lookup<StockLookup>(stkkey);
        if (stkrec == nullptr) return;
        pick(N, level, stkrec->d_row, result, params);
        return;
    }
    pick(N, level, -1, result, params);
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
    //std::cout << "Name (Sector)" << std::endl;
    //std::cout << std::setw(37) << "Industry" << std::endl;
    std::cout << std::setw(10) << "LIVEPX";
    std::cout << std::setw(10) << "MKTPX";
    std::cout << std::setw(25) << "MEDPX YR3->YR->QTR";
    std::cout << std::setw(20) << "YRPX-RANGE";
    std::cout << std::setw(20) << "YRPX-6SIGONTREND";
    std::cout << std::setw(10) << "YRYIELD";
    std::cout << std::setw(10) << "YRTRENDZ";
    std::cout << std::setw(10) << "YR-SD/DLR";
    std::cout << std::setw(10) << "YR8YIELD";
    std::cout << std::endl;
}


void printStock(int stockindex)
{
    auto stock    = stockdb.lookupByIndex<StockLookup>(stockindex);
    auto sector   = stockdb.lookupByIndex<SectorLookup>(stock->getSectorIndex());
    auto industry = stockdb.lookupByIndex<IndustryLookup>(stock->getIndustryIndex());

    std::string str = stock->getName().c_str();
    str.append(" (");
    str.append(sector->getName().c_str());
    str.append(")");
    if (str.length()>37) str = str.substr(0,37);
    std::cout << std::setw(37) << str;

    StockTrend key;
    key.setStockIndex(stockindex);
    key.setTimeToDate(TimeToDate_t::QUARTER);
    auto trendqtr = stockdb.lookup<StockTrend>(key);
    key.setTimeToDate(TimeToDate_t::YEAR);
    auto trendyr = stockdb.lookup<StockTrend>(key);
    key.setTimeToDate(TimeToDate_t::YEAR3);
    auto trendyr3 = stockdb.lookup<StockTrend>(key);
    key.setTimeToDate(TimeToDate_t::YEAR8);
    auto trendyr8 = stockdb.lookup<StockTrend>(key);
    char pricestr[75];
    memset(pricestr,0,75);
    if (trendqtr == nullptr || trendyr == nullptr || trendyr3 == nullptr) return;
    //sprintf(pricestr,"%.2f [%.2f (%.2f/%.2f) (%.2f/%.2f) (%.2f/%.2f) %.2f]",
    sprintf(pricestr,"%.2f [%.2f (%.2f<-%.2f<-%.2f) %.2f]",
            trendqtr->getLastMktPrice(), trendyr->getMinPrice(),
            /*trendqtr->getMeanPrice(), */trendqtr->getMedianPrice(),
            /*trendyr->getMeanPrice(), */trendyr->getMedianPrice(),
            /*trendyr3->getMeanPrice(), */trendyr3->getMedianPrice(),
            trendyr->getMaxPrice());
    
    //std::cout << std::setw(50) << pricestr;

    //LivePrice
    std::cout << std::setw(10) << livepx(stock->getName().c_str());

    //Price
    memset(pricestr,0,75);
    sprintf(pricestr,"%.2f", trendqtr->getLastMktPrice());
    std::cout << std::setw(10) << pricestr;

    //Minitrend
    memset(pricestr,0,75);
    sprintf(pricestr,"%.2f->%.2f->%.2f", trendyr3->getMedianPrice(), trendyr->getMedianPrice(), trendqtr->getMedianPrice());
    std::cout << std::setw(25) << pricestr;

    //Min-max range
    memset(pricestr,0,75);
    sprintf(pricestr,"[%.2f-%.2f]", trendyr->getMinPrice(), trendyr->getMaxPrice());
    std::cout << std::setw(20) << pricestr;

    //6 sigma Min-max range
    memset(pricestr,0,75);
    double min6sig = trendyr->getTrendLineEnd() - 3 * trendyr->getSdTrendDev();
    double max6sig = trendyr->getTrendLineEnd() + 3 * trendyr->getSdTrendDev();
    sprintf(pricestr,"[%.2f-%.2f]", min6sig, max6sig);
    std::cout << std::setw(20) << pricestr;

    std::cout << std::setw(10) << std::setprecision(2) << (100.0*trendyr->getAnnualYield());
    std::cout << std::setw(10) << std::setprecision(2) << trendyr->getLastMktTrendDevZ();
    std::cout << std::setw(10) << std::setprecision(2) << trendyr->getAbsVariance();
    std::cout << std::setw(10) << std::setprecision(2) << (100.0*trendyr8->getAnnualYield());
    std::cout << std::endl;

    str = stock->getDescription().c_str();
    if (str.length()>35) str = str.substr(0,35);
    std::cout << std::setw(37) << str << std::endl;

    str = industry->getName().c_str();
    if (str.length()>37) str = str.substr(0,37);
    std::cout << std::setw(37) << str << std::endl;

}

//Stocks exibiting most volatility
//Keep it to MidCap+, longer track record (ipos > 3yrs)
void topVolatileStocks(int N, LEVEL level, int32_t index, std::vector<int32_t> &stocks, void *)
{
    std::map<double, int32_t> gainers;
    DomainTable<StockLookup>::IndexIterator stkItr, stkItrN;
    stockdb.getTableIters<StockLookup>(stkItr,stkItrN);

    for (;stkItr != stkItrN;++stkItr)
    {
        auto stk = (*stkItr);

        if (index >= 0 && level == LEVEL::SECTOR && stk->getSectorIndex() != index) continue;
        if (index >= 0 && level == LEVEL::INDUSTRY && stk->getIndustryIndex() != index) continue;
        if (index >= 0 && level == LEVEL::MKTCAP && index != (int)stk->mktcap()) continue;
        if (index >= 0 && level == LEVEL::SINGLESTOCK && index != stk->d_row) continue;

        if (stk->mktcap() < MarketCap_t::MEDIUMCAP) continue;
        if (stk->getIpoYear() > 0 && stk->getIpoYear() > 2013) continue;
        
        StockTrend key, *trendY3;
        key.setStockIndex(stk->d_row);key.defTimeToDate();
        DomainTable<StockTrend>::IndexIterator stkTrendItr, stkTrendItrN;
        stockdb.getTableIters<StockTrend>(stkTrendItr,stkTrendItrN, "PrimaryKey", &key, &key);

        for (;stkTrendItr != stkTrendItrN;++stkTrendItr)
        {
            if ((*stkTrendItr)->getTimeToDate()() == TimeToDate_t::YEAR3)
                trendY3 = (*stkTrendItr);
        }

        if (trendY3 == nullptr) continue;
        double diff = trendY3->getAbsVariance();
        gainers[diff] = trendY3->getStockIndex();
        while ((int)gainers.size() > N)
            gainers.erase(gainers.begin());
    }
    
    for (auto rit = gainers.rbegin(); rit!=gainers.rend(); ++rit)
        stocks.push_back(rit->second);
}

//Stock by sector with highest worstcase yield from multiple time periods (<=5)
//Keep it to MidCap+, longer track record (ipos > 3yrs)
void topSteadyYielders(int N, LEVEL level, int32_t index, std::vector<int32_t> &stocks, void *)
{
    std::map<double, int32_t> gainers;
    DomainTable<StockLookup>::IndexIterator stkItr, stkItrN;
    stockdb.getTableIters<StockLookup>(stkItr,stkItrN);

    for (;stkItr != stkItrN;++stkItr)
    {
        auto stk = (*stkItr);

        if (index >= 0 && level == LEVEL::SECTOR && stk->getSectorIndex() != index) continue;
        if (index >= 0 && level == LEVEL::INDUSTRY && stk->getIndustryIndex() != index) continue;
        if (index >= 0 && level == LEVEL::MKTCAP && index != (int)stk->mktcap()) continue;
        if (index >= 0 && level == LEVEL::SINGLESTOCK && index != stk->d_row) continue;
        
        if (stk->mktcap() < MarketCap_t::MEDIUMCAP) continue;
        if (stk->getIpoYear() > 0 && stk->getIpoYear() > 2013) continue;
        
        StockTrend key;
        key.setStockIndex(stk->d_row);key.defTimeToDate();
        DomainTable<StockTrend>::IndexIterator stkTrendItr, stkTrendItrN;
        stockdb.getTableIters<StockTrend>(stkTrendItr,stkTrendItrN, "PrimaryKey", &key, &key);

        double minyield = 999999999999;
        for (;stkTrendItr != stkTrendItrN;++stkTrendItr)
        {
            if ((*stkTrendItr)->getTimeToDate()() == TimeToDate_t::QUARTER) continue;
            if ((*stkTrendItr)->getTimeToDate()() == TimeToDate_t::QUARTER_OLD) continue;
            minyield = std::min(minyield,(*stkTrendItr)->getAnnualYield());
        }
        
        gainers[minyield] = stk->d_row;
        while ((int)gainers.size() > N)
            gainers.erase(gainers.begin());
    }

    for (auto rit = gainers.rbegin(); rit!=gainers.rend(); ++rit)
        stocks.push_back(rit->second);
}

//Stocks with largest turnaround in YEAR yield with respect to YEAR5
//Keep it to MidCap+, longer track record (ipos > 3yrs)
void topTurnarounds(int N, LEVEL level, int32_t index, std::vector<int32_t> &stocks, void *)
{
    std::map<double, int32_t> gainers;
    DomainTable<StockLookup>::IndexIterator stkItr, stkItrN;
    stockdb.getTableIters<StockLookup>(stkItr,stkItrN);

    for (;stkItr != stkItrN;++stkItr)
    {
        auto stk = (*stkItr);

        if (index >= 0 && level == LEVEL::SECTOR && stk->getSectorIndex() != index) continue;
        if (index >= 0 && level == LEVEL::INDUSTRY && stk->getIndustryIndex() != index) continue;
        if (index >= 0 && level == LEVEL::MKTCAP && index != (int)stk->mktcap()) continue;
        if (index >= 0 && level == LEVEL::SINGLESTOCK && index != stk->d_row) continue;

        if (stk->mktcap() < MarketCap_t::MEDIUMCAP) continue;
        if (stk->getIpoYear() > 0 && stk->getIpoYear() > 2013) continue;
        
        StockTrend key, *trendY, *trendY3;
        key.setStockIndex(stk->d_row);key.defTimeToDate();
        DomainTable<StockTrend>::IndexIterator stkTrendItr, stkTrendItrN;
        stockdb.getTableIters<StockTrend>(stkTrendItr,stkTrendItrN, "PrimaryKey", &key, &key);

        for (;stkTrendItr != stkTrendItrN;++stkTrendItr)
        {
            if ((*stkTrendItr)->getTimeToDate()() == TimeToDate_t::YEAR3)
                trendY3 = (*stkTrendItr);
            if ((*stkTrendItr)->getTimeToDate()() == TimeToDate_t::QUARTER)
                trendY = (*stkTrendItr);
        }

        if (trendY3 == nullptr || trendY == nullptr) continue;
        double diff = trendY->getAnnualYield() - trendY3->getAnnualYield();
        gainers[diff] = trendY->getStockIndex();
        while ((int)gainers.size() > N)
            gainers.erase(gainers.begin());
    }
    
    for (auto rit = gainers.rbegin(); rit!=gainers.rend(); ++rit)
        stocks.push_back(rit->second);
}

void mktCapReport(int, LEVEL level, int32_t index, std::vector<int32_t> &, void *)
{
    /* std::cout   << std::setw(25) << "Name"
    << std::setw(15) << "Cap2016"
    << std::setw(15) << "Cap2015"
    << std::setw(15) << "Cap2013"
    << std::setw(15) << "Cap2011"
    << std::setw(15) << "Cap2009"
    << std::setw(15) << "Cap2008"
    << std::setw(15) << "Cap2006"
    << std::setw(15) << "StockCount"
    << std::setw(15) << "GrowthAmt"
    << std::setw(15) << "LossAmt"
    << std::endl; */
    
	double cap10 = 0, cap8 = 0, cap7 = 0, cap5 = 0, cap3 = 0, cap1 = 0, cap0 = 0;
	double growthamt = 0, lossamt = 0;
	int count = 0, growthcnt = 0, losscnt = 0;

    DomainTable<StockTrend>::IndexIterator trend1Itr, trendNItr;
	stockdb.getTableIters<StockTrend>(trend1Itr,trendNItr);
	for (;trend1Itr != trendNItr;++trend1Itr)
	{
		StockTrend &rec = *(*trend1Itr);
		
		auto stock = stockdb.lookupByIndex<StockLookup>(rec.getStockIndex());
        if (index >= 0 && level == LEVEL::SECTOR && stock->getSectorIndex() != index) continue;
        if (index >= 0 && level == LEVEL::INDUSTRY && stock->getIndustryIndex() != index) continue;
        if (index >= 0 && level == LEVEL::MKTCAP && index != (int)stock->mktcap()) continue;
        if (index >= 0 && level == LEVEL::SINGLESTOCK && index != stock->d_row) continue;
		
		std::string stkname(rec.getSymbol().c_str());
		if (stkname == "OHGI") continue;
		if (stkname == "TVIX") continue;

		if (rec.getTimeToDate()() == TimeToDate_t::QUARTER ||
			rec.getTimeToDate()() == TimeToDate_t::QUARTER_OLD)
				continue;

		double BILL = 1000000000;
		if (rec.getTimeToDate()() == TimeToDate_t::YEAR)
		{
			cap1 += rec.getInitMktCap()/BILL;
			cap0 += stock->getMarketCap()/BILL;
			count++;
			
			double growth = stock->getMarketCap() - rec.getInitMktCap();
			if (growth > 0)
			{
				growthcnt++;
				growthamt += growth/BILL;
			}
			else
			{
				losscnt++;
				lossamt += growth/BILL;
			}
		}
		else if (rec.getTimeToDate()() == TimeToDate_t::YEAR3)
		{
			cap3 += rec.getInitMktCap()/BILL;
		}
		else if (rec.getTimeToDate()() == TimeToDate_t::YEAR5)
		{
			cap5 += rec.getInitMktCap()/BILL;
		}
		else if (rec.getTimeToDate()() == TimeToDate_t::YEAR7)
		{
			cap7 += rec.getInitMktCap()/BILL;
		}
		else if (rec.getTimeToDate()() == TimeToDate_t::YEAR8)
		{
			cap8 += rec.getInitMktCap()/BILL;
		}
		else
		{
			cap10 += rec.getInitMktCap()/BILL;
		}
	}
	
	std::string stkcnt("");
	stkcnt.append(std::to_string(growthcnt));
	stkcnt.append("/");
	stkcnt.append(std::to_string(count));
	stkcnt.append("=");
	stkcnt.append(std::to_string((growthcnt*100)/count));
	stkcnt.append("%");
	
	std::string name = "ALL STOCKS";
    if (level == LEVEL::SECTOR && index >= 0)
    {
        auto lookup = stockdb.lookupByIndex<SectorLookup>(index);
        name = lookup->getName().c_str();
    }
    else if (level == LEVEL::INDUSTRY && index >= 0)
    {
        auto lookup = stockdb.lookupByIndex<IndustryLookup>(index);
        name = lookup->getName().c_str();
    }
    else if (index >= 0 && level == LEVEL::MKTCAP)
    {
        EnumData<MarketCap_t> cap = index;
        name = cap.toString();
    }
    else if (level == LEVEL::SINGLESTOCK && index >= 0)
    {
        auto lookup = stockdb.lookupByIndex<StockLookup>(index);
        name = lookup->getName().c_str();
    }
	if (name.length() > 25) name = name.substr(0,25);
	
	std::cout   << std::setw(25) << name
				<< std::setw(15) << fixed << cap0
				<< std::setw(15) << fixed << cap1
				<< std::setw(15) << fixed << cap3
				<< std::setw(15) << fixed << cap5
				<< std::setw(15) << fixed << cap7
				<< std::setw(15) << fixed << cap8
				<< std::setw(15) << fixed << cap10
				<< std::setw(15) << fixed << stkcnt
				<< std::setw(15) << fixed << growthamt
				<< std::setw(15) << fixed << lossamt
				<< std::endl;

	std::cout   << std::setw(25) << " "
				<< std::setw(15) << fixed << " "
				<< std::setw(14) << fixed << ((cap0 - cap1)*100/cap1) << "%"
				<< std::setw(14) << fixed << ((cap0 - cap3)*100/cap3) << "%"
				<< std::setw(14) << fixed << ((cap0 - cap5)*100/cap5) << "%"
				<< std::setw(14) << fixed << ((cap0 - cap7)*100/cap7) << "%"
				<< std::setw(14) << fixed << ((cap0 - cap8)*100/cap8) << "%"
				<< std::setw(14) << fixed << ((cap0 - cap10)*100/cap10) << "%"
				<< std::endl << std::endl;
}

//Stocks with largest avg gain in last 1 week
//Keep it to MidCap+, longer track record (ipos > 3yrs)
void topWeeklyGainers(int N, LEVEL level, int32_t index, std::vector<int32_t> &stocks, void *)
{
    std::map<double, int32_t> gainers;
    DomainTable<StockLookup>::IndexIterator stkItr, stkItrN;
    stockdb.getTableIters<StockLookup>(stkItr,stkItrN);

    for (;stkItr != stkItrN;++stkItr)
    {
        auto stk = (*stkItr);

        if (index >= 0 && level == LEVEL::SECTOR && stk->getSectorIndex() != index) continue;
        if (index >= 0 && level == LEVEL::INDUSTRY && stk->getIndustryIndex() != index) continue;
        if (index >= 0 && level == LEVEL::MKTCAP && index != (int)stk->mktcap()) continue;
        if (index >= 0 && level == LEVEL::SINGLESTOCK && index != stk->d_row) continue;

        if (stk->mktcap() < MarketCap_t::MEDIUMCAP) continue;
        if (stk->getIpoYear() > 0 && stk->getIpoYear() > 2013) continue;
        
        StockTrend key, *trendQ, *trendQOld;
        key.setStockIndex(stk->d_row);key.defTimeToDate();
        DomainTable<StockTrend>::IndexIterator stkTrendItr, stkTrendItrN;
        stockdb.getTableIters<StockTrend>(stkTrendItr,stkTrendItrN, "PrimaryKey", &key, &key);

        for (;stkTrendItr != stkTrendItrN;++stkTrendItr)
        {
            if ((*stkTrendItr)->getTimeToDate()() == TimeToDate_t::QUARTER)
                trendQ = (*stkTrendItr);
            if ((*stkTrendItr)->getTimeToDate()() == TimeToDate_t::QUARTER_OLD)
                trendQOld = (*stkTrendItr);
        }

        if (trendQOld == nullptr || trendQ == nullptr) continue;
        double diff = trendQ->getLastAvgPrice() - trendQOld->getLastAvgPrice();
        //diff = trendQ->getLastMktPrice() - trendQOld->getLastMktPrice();
        diff /= trendQ->getLastMktPrice()/100;
        gainers[diff] = trendQ->getStockIndex();
        while ((int)gainers.size() > N)
            gainers.erase(gainers.begin());
    }
    
    for (auto rit = gainers.rbegin(); rit!=gainers.rend(); ++rit)
        stocks.push_back(rit->second);
}

void initDb(char * dbfile)
{
    stockdb.getTable<SectorLookup>(nullptr,50*sizeof(SectorLookup));
    stockdb.getTable<IndustryLookup>(nullptr,500*sizeof(IndustryLookup));
    stockdb.getTable<StockLookup>(nullptr,10000*sizeof(StockLookup));
    stockdb.getTable<StockTrend>(nullptr,100000*sizeof(StockTrend));
    stockdb.getTable<StockEodPrice>(nullptr,10000*sizeof(StockEodPrice));
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
        tickers = "";

    if (stocklist == "software")
        tickers = "ENV,INTU";

    if (stocklist == "exchanges")
        tickers = "MKTX,INFO,ICE,CBOE,CME";

    if (stocklist == "banks")
        tickers = "BAC,GS,MS,JPM,C";

    if (stocklist == "bigtechs")
        tickers = "AMZN,GOOG,MSFT,FB";

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

void mktCapReportHead()
{
    std::cout << std::setw(25) << "Name"
            << std::setw(15) << "Cap2016"
            << std::setw(15) << "Cap2015"
            << std::setw(15) << "Cap2013"
            << std::setw(15) << "Cap2011"
            << std::setw(15) << "Cap2009"
            << std::setw(15) << "Cap2008"
            << std::setw(15) << "Cap2006"
            << std::setw(15) << "StockCount"
            << std::setw(15) << "GrowthAmt"
            << std::setw(15) << "LossAmt"
            << std::endl;
};


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

int main(int, char ** argsv)
{
    std::vector<int32_t> stocks;
    cout.precision(3);
    
    initDb(argsv[1]);
    
    //int records, LEVEL level, std::string filter, function, void * params, std::vector<int32_t> &result
    //Available functions = topSteadyYielders, topVolatileStocks, topWeeklyGainers, topTurnarounds, mktCapReport
    //mktCapReportHead();
    //stock_picker(25, LEVEL::ALL,    "", mktCapReport, nullptr, stocks);
    //stock_picker(25, LEVEL::MKTCAP, "", mktCapReport, nullptr, stocks);
    //stock_picker(25, LEVEL::SECTOR, "", mktCapReport, nullptr, stocks);

/*
    stock_picker(25, LEVEL::MKTCAP, "GIANT", topWeeklyGainers, nullptr, stocks);

    printStockHead();
    for (auto &iStk:stocks)
    {
        printStock(iStk);
        std::cout << std::endl;
    }
    
    printStockHead();
    for (auto &iStk:stocks)
    {
        auto stock = stockdb.lookupByIndex<StockLookup>(iStk);
        stock_picker(25, LEVEL::SINGLESTOCK, stock->getName().c_str(), mktCapReport, nullptr, stocks);
        std::cout << std::endl;
    } 
*/
    
     favorites("banks");
     //favorites("exchanges");
 
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
