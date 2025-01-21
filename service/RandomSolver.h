#ifndef RANDOM_SOLVER_H
#define RANDOM_SOLVER_H

#include <vector>
#include <chrono>

class RandomSolver {
public:
    RandomSolver() = default;

    // Rozwiązuje problem TSP dla grafu i zwraca najlepszą ścieżkę
    std::vector<int> solve(const std::vector<std::vector<int>>& graph, int timeLimitMinutes);

    // Getter dla minimalnego kosztu
    int getMinCost() const;

private:
    int calculatePathCost(const std::vector<int>& path, const std::vector<std::vector<int>>& graph);

    std::vector<int> bestPath; // Najlepsza ścieżka
    int minCost = std::numeric_limits<int>::max(); // Minimalny koszt
};

#endif // RANDOM_SOLVER_H
