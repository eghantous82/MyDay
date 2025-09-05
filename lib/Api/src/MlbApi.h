#include "IDataRetriever.h"

#include <vector>
#pragma once

class JsonDocument;

class MlbApi {
public:
    struct TeamStanding {
        std::string Team;
        int Wins;
        int Losses;
        float GamesBack;
    };

    MlbApi(const std::string& apiKey = "") : _apiKey(apiKey) {}

    void getStandings(IDataRetriever& retriever,
        const std::string& league,
        const std::string& division,
        std::vector<TeamStanding>& divisionStandings,
        std::vector<TeamStanding>& alDivisionLeaders);

private:
    // Returns the top team in each AL division (East, Central, West)
    void getAlDivisionLeaders(IDataRetriever& retriever, JsonDocument& doc, std::vector<TeamStanding>& alDivisionLeaders);
    
    std::string _apiKey;
};