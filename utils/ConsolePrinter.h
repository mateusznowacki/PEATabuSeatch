//
// Created by matty on 14.10.2024.
//

#ifndef PEA11_CONSOLEPRINTER_H
#define PEA11_CONSOLEPRINTER_H


#include <string>
#include <chrono>

class ConsolePrinter {
public:
    void printInfo(std::string filename, std::string optimalPath, int optimalCost);

    void displayResults(const std::string& fileName, int calculatedCost, int optimalCost,
                                        const std::vector<int>& calculatedPath, const std::vector<int>& optimalPath,
                                        double executionTime);


    void printStartInfo(std::string inputFileName, std::string outputFileName, bool displayResults, bool showProgress,
                        int iterations, int getInputOptimalCost, std::string getInputOptimalPath, int timeLimit,std::string algorithm);

    void bestPath(int minCost, std::string basicString);

    void printEndInfo(int minCost, std::string bestPath, std::chrono::duration<double> duration, int inputOptimalCost);
};


#endif //PEA11_CONSOLEPRINTER_H
