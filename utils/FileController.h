#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H
#include "../dto/ResultDataDto.h"
#include "../dto/ConfigDataDto.h"
#include <vector>


class FileController
{
public:
    std::vector<std::vector<int>> readGraphFromFile(const std::string& filename, bool testMode);
    int readCost(const std::string& string, bool get_test_mode);
    ConfigDataDto readConfigFile(const char* filename);
    bool fileExists(const std::string& fileName);
    void saveResultsToCSV(
        const std::string& outputFileName,
        const std::string& instanceName,
        int optimalCostFromFile,
        int foundCost,
        const std::vector<int>& foundPath,
        std::chrono::steady_clock::time_point startTime,
        std::chrono::steady_clock::time_point endTime
    );
    std::vector<int> readOptimalPath(const char* filename);
};
#endif // FILECONTROLLER_H
