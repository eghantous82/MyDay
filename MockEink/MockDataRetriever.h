#include "IDataRetriever.h"

class MockDataRetriever : public IDataRetriever {
public:
    // Implement necessary methods here
    std::string getSportsData(const std::string& url, const std::string& apiKey) override;

    std::string getStockInfo(const std::string& url, const std::string& apiKey) override;

    std::string getGoogleTasks(const std::string& url) override;

    std::string getBlynkValue(const std::string& url) override;

    void logToGoogle(const std::string& message) override;
};