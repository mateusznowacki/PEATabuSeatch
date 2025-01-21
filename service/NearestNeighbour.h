#ifndef PEA11_NEARESTNEIGHBOUR_H
#define PEA11_NEARESTNEIGHBOUR_H


#include <vector>
#include <string>
#include <chrono>
#include "Graph.h"


class NearestNeighbour {
public:
    NearestNeighbour();

    int solve(int optimalCost = -1, const std::vector<int>& optimalPath = {});
    std::string getBestPath() const;
    std::chrono::duration<double> getExecutionTime() const;
    int getMinCost() const;

private:
    const Graph& graph;
    bool display;
    int minCost;
    std::vector<int> bestPath;
    std::chrono::duration<double> executionTime;

    int findNearest(int current, const std::vector<bool>& visited) const;
};

#endif // NEAREST_NEIGHBOUR_H