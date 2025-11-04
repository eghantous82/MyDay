#include "Secrets/src/Secrets.h"
#include <fstream>
#include <sstream>
#ifndef ARDUINO
Secrets::Secrets(const std::string& secretsFile) {
	std::ifstream file(secretsFile);
	std::stringstream buffer;
	if (file) {
		buffer << file.rdbuf();
		std::string json = buffer.str();
		deserializeJson(_doc, json);
	}
}
#else
Secrets::Secrets(const std::string& jsonString) {
	deserializeJson(_doc, jsonString);
}
#endif

std::string Secrets::getBlynkAuthToken() {
	return _doc["blynk"].as<std::string>();
}
std::string Secrets::getNhlStandingsUrl() {
	return _doc["nhlStandings"].as<std::string>();
}
std::string Secrets::getMarketApiKey() {
	return _doc["rapidapi"].as<std::string>();
}

std::string Secrets::getGoogleTasksUrl() {
	return _doc["googleTasks"].as<std::string>();
} 

std::string Secrets::getLoggerUrl() {
	return _doc["logger"].as<std::string>();
}

std::string Secrets::getGoogleCalendarUrl() {
	return _doc["calendar"].as<std::string>();
}