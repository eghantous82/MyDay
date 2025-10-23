#pragma once
#include <ArduinoJson.h>
#include <vector>
#include <map>

#include "IDataRetriever.h"

class NhlApi {
public:
    struct TeamStanding {
        std::string Team;
        int Wins;
        int Losses;
        int Points;
    };

    NhlApi(const std::string& apiKey = "") : _apiKey(apiKey) {}

    void getStandings(IDataRetriever& retriever,
        std::map<std::string, std::vector<NhlApi::TeamStanding> >& teamsStandings);

private:   
    std::string _apiKey;
};