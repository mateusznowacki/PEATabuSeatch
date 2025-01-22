#include "RandomSolver.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>
#include <limits>

std::vector<int> RandomSolver::solve(const std::vector<std::vector<int>>& graph)
{
    int vertices = graph.size();
    if (vertices == 0)
    {
        return {}; // Zwraca pustą ścieżkę, jeśli graf jest pusty
    }

    minCost = std::numeric_limits<int>::max();
    bestPath.clear(); // Czyszczenie poprzednich wyników
    std::vector<int> path(vertices);
    std::iota(path.begin(), path.end(), 0); // Ustawienie początkowej ścieżki jako [0, 1, 2, ..., vertices-1]

    std::random_device rd;
    std::mt19937 rng(rd());
    auto start = std::chrono::high_resolution_clock::now();


    std::shuffle(path.begin(), path.end(), rng); // Tasowanie ścieżki (oprócz pierwszego wierzchołka)
    bestPath = path;


    return bestPath;
}

// Obliczanie kosztu ścieżki
int RandomSolver::calculatePathCost(const std::vector<int>& path, const std::vector<std::vector<int>>& graph)
{
    int totalCost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        totalCost += graph[path[i]][path[i + 1]];
    }
    totalCost += graph[path.back()][path.front()]; // Dodanie kosztu powrotu do wierzchołka początkowego
    return totalCost;
}

// Getter dla minimalnego kosztu
int RandomSolver::getMinCost() const
{
    return minCost;
}
