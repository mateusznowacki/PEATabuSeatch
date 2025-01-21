#ifndef DATADTO_H
#define DATADTO_H

#include <vector>
#include <chrono>

class DataDto {
private:
    bool display;
    int minCost;
    std::vector<int> bestPath;
    std::chrono::duration<double> executionTime;

public:
    // Konstruktor domyślny
    DataDto();

    // Settery
    void setDisplay(bool value);
    void setMinCost(int value);
    void setBestPath(const std::vector<int>& path);
    void setExecutionTime(const std::chrono::duration<double>& time);

    // Gettery
    bool getDisplay() const;
    int getMinCost() const;
    std::vector<int> getBestPath() const;
    std::chrono::duration<double> getExecutionTime() const;
};

# endif