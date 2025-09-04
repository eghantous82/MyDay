#include "Secrets/src/Secrets.h"
#ifdef ARDUINO
// ESP32/PlatformIO stub implementation
Secrets::Secrets(const std::string& secretsFile) {}
std::string Secrets::getBlynkAuthToken() { return "YOUR_BLYNK_TOKEN"; }
std::string Secrets::getMlbApiKey() { return "YOUR_MLB_API_KEY"; }
std::string Secrets::getMarketApiKey() { return "YOUR_MARKET_API_KEY"; }
#else
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
#endif

std::string Secrets::getGoogleTasksUrl() {
    if (_doc.containsKey("googleTasks"))
        return _doc["googleTasks"].as<std::string>();
    return "";
} 
