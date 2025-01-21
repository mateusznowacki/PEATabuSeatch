#ifndef RANDOMSOLVER_H
#define RANDOMSOLVER_H

#include "Graph.h"
#include <vector>
#include <string>
#include <chrono>

/*
 * Klasa RandomSolver implementuje algorytm losowy do rozwiązywania problemu komiwojażera (TSP).
 */
class RandomSolver {
public:
    // Konstruktor
    RandomSolver();

    // Metoda rozwiązująca problem TSP z limitem czasowym (w minutach) i opcjonalnym kosztem optymalnym
    int solve(int timeLimitMinutes, int optimalCost);

    // Funkcja zwracająca najlepszą ścieżkę jako ciąg znaków
    std::string getBestPath() const;

    // Funkcja zwracająca minimalny koszt znaleziony przez algorytm
    int getMinCost() const;

    // Funkcja zwracająca czas wykonania algorytmu
    std::chrono::duration<double> getExecutionTime() const;

private:
    // Funkcja do obliczania kosztu danej ścieżki
    int calculatePathCost(const std::vector<int>& path);

    const Graph& graph;                        // Referencja do grafu
    bool display;                              // Flaga kontrolująca wyświetlanie wyników
    int minCost;                               // Minimalny znaleziony koszt
    std::vector<int> bestPath;                 // Najlepsza znaleziona ścieżka
    std::chrono::duration<double> executionTime; // Czas wykonania algorytmu
};

#endif // RANDOMSOLVER_H
