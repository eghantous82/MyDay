#include "IDataRetriever.h"

class MockDataRetriever : public IDataRetriever {
public:
    // Implement necessary methods here
    std::string getMlbData(const std::string& url);
};