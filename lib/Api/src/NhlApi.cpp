#include <ArduinoJson.h>
#include "NhlApi.h"
#include <map>

void NhlApi::getStandings(IDataRetriever& retriever, std::map<std::string, std::vector<NhlApi::TeamStanding> >& teamStandings) {
    
    ArduinoJson::JsonDocument doc;
    std::string data = retriever.getSportsData(_url);
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
        teamStandings[division].reserve(4);
    }

    if (doc.is<JsonObject>()) {
        // New format: object with division arrays. Iterate known divisions
        std::vector<std::string> divisions = {"Atlantic", "Metropolitan", "Central", "Pacific"};
        for (const auto& division : divisions) {
            if (!doc.containsKey(division.c_str())) continue;
            JsonArray arr = doc[division.c_str()].as<JsonArray>();
            for (const JsonObject& t : arr) {
                TeamStanding ts;
                ts.Team = std::string(t["teamName"] | "");
                ts.Wins = t["wins"] | 0;
                ts.GamesPlayed = t["gamesPlayed"] | 0;
                ts.Losses = t["losses"] | 0;
                ts.Points = t["points"] | 0;
                teamStandings[division].emplace_back(ts);
            }
        }
    }
}
