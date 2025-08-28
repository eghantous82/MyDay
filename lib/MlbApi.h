#include "IDataRetriever.h"
#include <ArduinoJson.h>

#include <vector>
#pragma once

class MlbApi {
public:
    struct TeamStanding {
        std::string Team;
        int Wins;
        int Losses;
        float GamesBack;
    };
    void getStandings(IDataRetriever& retriever,
        const std::string& league,
        const std::string& division,
        std::vector<TeamStanding>& divisionStandings,
        std::vector<TeamStanding>& alDivisionLeaders);


    // Returns the top team in each AL division (East, Central, West)
    void getAlDivisionLeaders(IDataRetriever& retriever, JsonDocument& doc, std::vector<TeamStanding>& alDivisionLeaders);
    
};