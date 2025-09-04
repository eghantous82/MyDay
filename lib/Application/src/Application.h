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

#pragma once

class Application {
public:
    Application(IDataRetriever& retriever, Secrets& secrets) : _retriever(retriever), _secrets(secrets) {}
    void renderScreen(Adafruit_GFX& display);

private:

    void renderMlbInfo(Adafruit_GFX& display);
    void renderMarketInfo(Adafruit_GFX& display);
    void renderBlynkInfo(Adafruit_GFX& display);
    void renderGoogleTasks(Adafruit_GFX& display);

    void getMarketInfo(std::vector<MarketApi::EquityInfo>& equities);
    void getMlbInfo(std::vector<MlbApi::TeamStanding>& alStandings,
    std::vector<MlbApi::TeamStanding>& alEastStandings);
    void getGoogleTasks(std::vector<GoogleScriptApi::Task>& tasks);
    std::string getBlynkValue();

    IDataRetriever& _retriever;
    Secrets&        _secrets;
    
};