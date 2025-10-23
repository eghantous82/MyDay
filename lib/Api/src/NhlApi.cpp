#include <ArduinoJson.h>
#include "NhlApi.h"
#include <map>

void NhlApi::getStandings(IDataRetriever& retriever, std::map<std::string, std::vector<NhlApi::TeamStanding> >& teamStandings) {
    
    ArduinoJson::JsonDocument doc;
    std::string data = retriever.getSportsData("https://api.sportsdata.io/v3/nhl/scores/json/Standings/2026", _apiKey);
    if(data.empty()) {
        TeamStanding errorTeam;
        errorTeam.Team = "Error retrieving standings";
        errorTeam.Wins = 0;
        errorTeam.Losses = 0;
        errorTeam.Points = 0;
        teamStandings["Error"].emplace_back(errorTeam);
        return;
    }
    DeserializationError error = deserializeJson(doc, data);

    if (error) {
        TeamStanding errorTeam;
        errorTeam.Team = "Error parsing standings";
        errorTeam.Wins = 0;
        errorTeam.Losses = 0;
        errorTeam.Points = 0;
        teamStandings["Error"].emplace_back(errorTeam);
        return;
    }

    std::vector<std::string> divisions = {"Atlantic", "Metropolitan", "Central", "Pacific"};

    for (const auto& division : divisions) {
        teamStandings[division].reserve(8);
    }

    for (const JsonObject& team : doc.as<JsonArray>()) {
        TeamStanding ts;
        ts.Team = std::string(team["Key"] | "");
        ts.Wins = team["Wins"] | 0;
        ts.Losses = team["Losses"] | 0;
        std::string division = std::string(team["Division"] | "");
        int otLosses = team["OvertimeLosses"] | 0;
        ts.Points = (ts.Wins * 2) + otLosses; // NHL points system
        teamStandings[division].emplace_back(ts);
    }
}
