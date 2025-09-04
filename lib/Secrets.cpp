// empty
#include "Secrets.h"
#include <fstream>
#include <sstream>

Secrets::Secrets(const std::string& secretsFile) {
	std::ifstream file(secretsFile);
	std::stringstream buffer;
	if (file) {
		buffer << file.rdbuf();
		std::string json = buffer.str();
		deserializeJson(_doc, json);
	}
}

std::string Secrets::getBlynkAuthToken() {
	if (_doc.containsKey("blynk"))
		return _doc["blynk"].as<std::string>();
	return "";
}

std::string Secrets::getMlbApiKey() {
	if (_doc.containsKey("sportsio"))
		return _doc["sportsio"].as<std::string>();
	return "";
}

std::string Secrets::getMarketApiKey() {
	if (_doc.containsKey("rapidapi"))
		return _doc["rapidapi"].as<std::string>();
	return "";
}

std::string Secrets::getGoogleTasksUrl() {
    if (_doc.containsKey("googleTasks"))
        return _doc["googleTasks"].as<std::string>();
    return "";
} 
