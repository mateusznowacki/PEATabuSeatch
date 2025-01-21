#include "DataDto.h"

// Konstruktor domyślny
DataDto::DataDto()
    : display(false),
      minCost(0),
      bestPath(),
      executionTime(std::chrono::duration<double>(0)) {}

// Settery
void DataDto::setDisplay(bool value) {
    display = value;
}

void DataDto::setMinCost(int value) {
    minCost = value;
}

void DataDto::setBestPath(const std::vector<int>& path) {
    bestPath = path;
}

void DataDto::setExecutionTime(const std::chrono::duration<double>& time) {
    executionTime = time;
}

// Gettery
bool DataDto::getDisplay() const {
    return display;
}

int DataDto::getMinCost() const {
    return minCost;
}

std::vector<int> DataDto::getBestPath() const {
    return bestPath;
}

std::chrono::duration<double> DataDto::getExecutionTime() const {
    return executionTime;
}
