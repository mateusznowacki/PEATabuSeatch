#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H
#include "../dto/ResultDataDto.h"
#include "../dto/ConfigDataDto.h"
#include "../service/Graph.h"



class FileController {
public:
        std::vector<std::vector<int>> readGraphFromFile(const std::string& filename, bool testMode);
        int readCost(const std::string& string, bool get_test_mode);
        ConfigDataDto readConfigFile(const char* filename);
};

#endif // FILECONTROLLER_H

