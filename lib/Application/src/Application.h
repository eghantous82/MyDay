#pragma once
#ifdef _WIN32
#include "../MockEink/Adafruit_GFX.h"
#else
#include <Adafruit_GFX.h>
#endif

#include <vector>
#include "Api/src/MarketApi.h"
#include "Api/src/BlynkApi.h"
#include "Api/src/MlbApi.h"
#include "Secrets/src/Secrets.h"
#include "Api/src/GoogleScriptApi.h"

class Application {
public:
    Application(IDataRetriever& retriever, Secrets& secrets) : _retriever(retriever), _secrets(secrets) {}
    void renderScreen(Adafruit_GFX& display);

private:

    void renderMlbInfo(Adafruit_GFX& display);
    void renderMarketInfo(Adafruit_GFX& display, std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve);
    void renderBlynkInfo(Adafruit_GFX& display);
    void renderGoogleInfo(Adafruit_GFX& display);

    void getMarketInfo(std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve, std::vector<MarketApi::EquityInfo>& equities);
    void getMlbInfo(std::vector<MlbApi::TeamStanding>& alStandings,
    std::vector<MlbApi::TeamStanding>& alEastStandings);
    void getGoogleInfo(std::pair<std::vector<GoogleScriptApi::Task>, std::vector<GoogleScriptApi::StockInfo> >& googleInfo);
    std::string getBlynkValue();

    IDataRetriever& _retriever;
    Secrets&        _secrets;
    
};