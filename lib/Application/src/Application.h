#pragma once
#ifdef _WIN32
#include "../MockEink/Adafruit_GFX.h"
#else
#include <Adafruit_GFX.h>
#endif

#include <vector>
#include <time.h>
#include "Api/src/MarketApi.h"
#include "Api/src/BlynkApi.h"
#include "Api/src/MlbApi.h"
#include "Secrets/src/Secrets.h"
#include "Api/src/GoogleScriptApi.h"

class Application {
public:
    Application(IDataRetriever& retriever, Secrets& secrets) : 
        _retriever(retriever),
        _secrets(secrets),
        _lastBlynkRunTime(0),
        _lastGoogleTaskRunTime(0),
        _lastMlbRunTime(0),
        _lastStockInfoRunTime(0) {}

    bool renderMlbInfo(Adafruit_GFX& display);
    bool renderBlynkInfo(Adafruit_GFX& display);
    bool renderGoogleInfo(Adafruit_GFX& display, std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve);
    bool renderMarketInfo(Adafruit_GFX& display, int x, std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve);

private:

    void getMarketInfo(std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve, std::vector<MarketApi::EquityInfo>& equities);
    void getMlbInfo(std::vector<MlbApi::TeamStanding>& alStandings,
    std::vector<MlbApi::TeamStanding>& alEastStandings);
    void getGoogleInfo(std::pair<std::vector<GoogleScriptApi::Task>, std::vector<GoogleScriptApi::StockInfo> >& googleInfo);
    std::string getBlynkValue();

    IDataRetriever& _retriever;
    Secrets&        _secrets;
    time_t         _lastGoogleTaskRunTime;
    time_t         _lastStockInfoRunTime;
    time_t         _lastMlbRunTime;
    time_t         _lastBlynkRunTime;
    
};