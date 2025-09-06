#include <ArduinoJson.h>
#include "MlbApi.h"
#include <map>

void MlbApi::getStandings(IDataRetriever& retriever, const std::string& league, const std::string& division, std::vector<TeamStanding>& divisionStandings, std::vector<TeamStanding>& alDivisionLeaders) {
    
    ArduinoJson::JsonDocument doc;
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
    std::map<std::string, std::pair<int, int>> leaderRecordByDivision;

    // First pass: find the leader for each division
    for (JsonObject team : doc.as<JsonArray>()) {
        if (team["League"] == "AL") {
            std::string name = team["Name"] | "";
            if (name == "All Stars") continue;
            std::string division = team["Division"] | "";
            int wins = team["Wins"] | 0;
            int losses = team["Losses"] | 0;
            // If this division is not in the map or this team has more wins, update
            if (bestByDivision.find(division) == bestByDivision.end() || wins > bestByDivision[division].Wins) {
                TeamStanding s;
                s.Team = team["Key"] | "";
                s.Wins = wins;
                s.Losses = losses;
                bestByDivision[division] = s;
                leaderRecordByDivision[division] = std::make_pair(wins, losses);
            }
        }
    }

    // Find the overall AL leader (most wins, fewest losses as tiebreaker)
    int maxWins = -1;
    int minLosses = 1000;
    for (const auto& pair : bestByDivision) {
        int wins = pair.second.Wins;
        int losses = pair.second.Losses;
        if (wins > maxWins || (wins == maxWins && losses < minLosses)) {
            maxWins = wins;
            minLosses = losses;
        }
    }

    // Assign GamesBack for each division leader relative to the overall AL leader
    for (auto& pair : bestByDivision) {
        TeamStanding& leader = pair.second;
        leader.GamesBack = ((maxWins - leader.Wins) + (leader.Losses - minLosses)) / 2.0f;
        alDivisionLeaders.push_back(leader);
    }
}