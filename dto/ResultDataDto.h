#ifndef RESULTDATADTO_H
#define RESULTDATADTO_H

#include <string>
#include <chrono>

class ResultDataDto {
private:
    std::string outputFileName;
    std::string instanceName;
    std::string inputFileName;
    int inputOptimalCost;
    std::string optimalPath;
    int foundCost;
    std::string foundPath;
    std::chrono::duration<double> executionTime;

public:
    // Konstruktor domyślny
    ResultDataDto();

    // Settery
    void setOutputFileName(const std::string& value);
    void setInstanceName(const std::string& value);
    void setInputFileName(const std::string& value);
    void setInputOptimalCost(int value);
    void setOptimalPath(const std::string& value);
    void setFoundCost(int value);
    void setFoundPath(const std::string& value);
    void setExecutionTime(const std::chrono::duration<double>& value);

    // Gettery
    std::string getOutputFileName() const;
    std::string getInstanceName() const;
    std::string getInputFileName() const;
    int getInputOptimalCost() const;
    std::string getOptimalPath() const;
    int getFoundCost() const;
    std::string getFoundPath() const;
    std::chrono::duration<double> getExecutionTime() const;
};

#endif // RESULTDATADTO_H
