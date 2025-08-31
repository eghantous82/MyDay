#include "MlbApi.h"
#include <map>

void MlbApi::getStandings(IDataRetriever& retriever, const std::string& league, const std::string& division, std::vector<TeamStanding>& divisionStandings, std::vector<TeamStanding>& alDivisionLeaders) {
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, retriever.getMlbData("https://api.sportsdata.io/v3/mlb/scores/json/Standings/2025", _apiKey));

    if (error) {
        // handle error if needed
        return;
    }

    for (JsonObject team : doc.as<JsonArray>()) {
        if (team["League"] == "AL" && team["Division"] == "East") {
            std::string name = team["Name"] | "";
            if (name == "All Stars") continue;
            TeamStanding s;
                s.Team = team["Key"] | "";
            s.Wins = team["Wins"] | 0;
            s.Losses = team["Losses"] | 0;
            s.GamesBack = team["GamesBehind"] | 0.0f;
            divisionStandings.push_back(s);
        }
    }

    getAlDivisionLeaders(retriever, doc, alDivisionLeaders);
}

void MlbApi::getAlDivisionLeaders(IDataRetriever& retriever,  JsonDocument& doc, std::vector<TeamStanding>& alDivisionLeaders) {

    // Map to hold the best team for each division
    std::map<std::string, TeamStanding> bestByDivision;

    for (JsonObject team : doc.as<JsonArray>()) {
        if (team["League"] == "AL") {
            std::string name = team["Name"] | "";
            if (name == "All Stars") continue;
            std::string division = team["Division"] | "";
            int wins = team["Wins"] | 0;
            // If this division is not in the map or this team has more wins, update
            if (bestByDivision.find(division) == bestByDivision.end() || wins > bestByDivision[division].Wins) {
                TeamStanding s;
                    s.Team = team["Key"] | "";
                s.Wins = wins;
                s.Losses = team["Losses"] | 0;
                bestByDivision[division] = s;
            }
        }
    }
    // Collect the leaders
    for (const auto& pair : bestByDivision) {
        alDivisionLeaders.push_back(pair.second);
    }
}