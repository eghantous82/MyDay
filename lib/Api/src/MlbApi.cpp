#include <ArduinoJson.h>
#include "MlbApi.h"
#include <map>

void MlbApi::getStandings(IDataRetriever& retriever, const std::string& league, const std::string& division, std::vector<TeamStanding>& divisionStandings, std::vector<TeamStanding>& alDivisionLeaders) {
    
    ArduinoJson::JsonDocument doc;
    std::string mlbData = retriever.getMlbData("https://api.sportsdata.io/v3/mlb/scores/json/Standings/2025", _apiKey);
    if(mlbData.empty()) {
        TeamStanding errorTeam;
        errorTeam.Team = "Error retrieving standings";
        errorTeam.Wins = 0;
        errorTeam.Losses = 0;
        errorTeam.GamesBack = 0.0f;
        divisionStandings.push_back(errorTeam);
        return;
    }
    DeserializationError error = deserializeJson(doc, mlbData);

    if (error) {
        TeamStanding errorTeam;
        errorTeam.Team = "Error parsing standings";
        errorTeam.Wins = 0;
        errorTeam.Losses = 0;
        errorTeam.GamesBack = 0.0f;
        divisionStandings.push_back(errorTeam);
        return;
    }

    divisionStandings.reserve(10); // Typical division size
    for (const JsonObject& team : doc.as<JsonArray>()) {
        if (team["League"] == "AL" && team["Division"] == "East") {
            std::string name = team["Name"] | "";
            if (name == "All Stars") continue;
            divisionStandings.emplace_back(TeamStanding{
                team["Key"] | "",
                team["Wins"] | 0,
                team["Losses"] | 0,
                team["GamesBehind"] | 0.0f
            });
        }
    }

    getAlDivisionLeaders(retriever, doc, alDivisionLeaders);
}

void MlbApi::getAlDivisionLeaders(IDataRetriever& retriever,  JsonDocument& doc, std::vector<TeamStanding>& alDivisionLeaders) {

    // Map to hold the best team for each division
    std::map<std::string, TeamStanding> bestByDivision;

    // First pass: find the leader for each division
    for (const JsonObject& team : doc.as<JsonArray>()) {
        if (team["League"] == "AL") {
            std::string name = team["Name"] | "";
            if (name == "All Stars") continue;
            std::string division = team["Division"] | "";
            int wins = team["Wins"] | 0;
            int losses = team["Losses"] | 0;
            // If this division is not in the map or this team has more wins, update
            if (bestByDivision.find(division) == bestByDivision.end() || wins > bestByDivision[division].Wins) {
                bestByDivision[division] = TeamStanding{
                    team["Key"] | "",
                    wins,
                    losses,
                    0.0f
                };
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

    alDivisionLeaders.reserve(bestByDivision.size());
    for (auto& pair : bestByDivision) {
        alDivisionLeaders.emplace_back(pair.second);
    }

    // Sort the vector in descending order of wins
    std::sort(alDivisionLeaders.begin(), alDivisionLeaders.end(),
        [](const TeamStanding& a, const TeamStanding& b) {
            return (a.Wins > b.Wins) || (a.Wins == b.Wins && a.Losses < b.Losses);
        });
}