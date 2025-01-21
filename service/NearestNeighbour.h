#ifndef NEAREST_NEIGHBOUR_H
#define NEAREST_NEIGHBOUR_H

#include <vector>
#include <limits>
#include <queue>

class NearestNeighbour {
public:
    NearestNeighbour() = default;

    // Rozwiązanie problemu TSP dla grafu
    std::vector<int> solve(const std::vector<std::vector<int>>& graph);

    // Getter dla minimalnego kosztu
    int getMinCost() const;

private:
    std::vector<int> bestPath; // Najlepsza ścieżka
    int minCost = std::numeric_limits<int>::max(); // Minimalny koszt
};

#endif // NEAREST_NEIGHBOUR_H
