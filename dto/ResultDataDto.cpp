#include "ResultDataDto.h"

// Konstruktor domyślny
ResultDataDto::ResultDataDto()
    : outputFileName(""),
      instanceName(""),
      inputFileName(""),
      inputOptimalCost(0),
      optimalPath(""),
      foundCost(0),
      foundPath(""),
      executionTime(std::chrono::duration<double>(0)) {}

// Settery
void ResultDataDto::setOutputFileName(const std::string& value) {
    outputFileName = value;
}

void ResultDataDto::setInstanceName(const std::string& value) {
    instanceName = value;
}

void ResultDataDto::setInputFileName(const std::string& value) {
    inputFileName = value;
}

void ResultDataDto::setInputOptimalCost(int value) {
    inputOptimalCost = value;
}

void ResultDataDto::setOptimalPath(const std::string& value) {
    optimalPath = value;
}

void ResultDataDto::setFoundCost(int value) {
    foundCost = value;
}

void ResultDataDto::setFoundPath(const std::string& value) {
    foundPath = value;
}

void ResultDataDto::setExecutionTime(const std::chrono::duration<double>& value) {
    executionTime = value;
}

// Gettery
std::string ResultDataDto::getOutputFileName() const {
    return outputFileName;
}

std::string ResultDataDto::getInstanceName() const {
    return instanceName;
}

std::string ResultDataDto::getInputFileName() const {
    return inputFileName;
}

int ResultDataDto::getInputOptimalCost() const {
    return inputOptimalCost;
}

std::string ResultDataDto::getOptimalPath() const {
    return optimalPath;
}

int ResultDataDto::getFoundCost() const {
    return foundCost;
}

std::string ResultDataDto::getFoundPath() const {
    return foundPath;
}

std::chrono::duration<double> ResultDataDto::getExecutionTime() const {
    return executionTime;
}
