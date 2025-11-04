#pragma once
#include <ArduinoJson.h>
#include <vector>
#include <map>

#include "IDataRetriever.h"

class NhlApi {
public:
    struct TeamStanding {
        std::string Team;
        int GamesPlayed;
        int Wins;
        int Losses;
        int Points;
    };

    NhlApi(const std::string& url = "") : _url(url) {}

    void getStandings(IDataRetriever& retriever,
        std::map<std::string, std::vector<NhlApi::TeamStanding> >& teamsStandings);

private:   
    std::string _url;
};