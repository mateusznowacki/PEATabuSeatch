#include "RandomSolver.h"
#include "Graph.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <random>
#include <chrono>
#include <algorithm>

/*
 * Implementacja algorytmu losowego do rozwiązywania problemu komiwojażera (TSP).
 * Algorytm losowo generuje permutacje wierzchołków i oblicza koszt każdej z nich, aż do osiągnięcia limitu czasowego.
 * Jest to podejście stochastyczne, które nie gwarantuje znalezienia optymalnego rozwiązania, ale może być użyteczne do znajdowania przybliżonych wyników.
 */

// Konstruktor
RandomSolver::RandomSolver()
        : graph(graph), display(display), minCost(std::numeric_limits<int>::max()) {}

// Rozwiązywanie problemu TSP za pomocą losowego podejścia opartego na permutacjach z limitem czasowym (w minutach)
int RandomSolver::solve(int timeLimitMinutes, int optimalCost) {
    // int vertices = std::ctype_base::graph.getVertices();
    // if (vertices <= 0) {
    //     std::cerr << "Error: Graph has no vertices." << std::endl;
    //     return -1;
    // }
    //
    // int bestCost = std::numeric_limits<int>::max();  // Inicjalizacja najlepszego kosztu
    // std::vector<int> bestPath;  // Inicjalizacja najlepszej ścieżki
    // auto start = std::chrono::high_resolution_clock::now();
    //
    // if (display) {
    //     std::cout << "Calculating...\n";
    // }
    //
    // // Ustawienie początkowej ścieżki jako [0, 1, 2, ..., vertices-1]
    // std::vector<int> path(vertices);
    // std::iota(path.begin(), path.end(), 0);
    //
    // // Ustawienie generatora losowego
    // std::random_device rd;
    // std::mt19937 rng(rd());
    //
    // int iterationCount = 0;
    //
    // while (true) {
    //     // Tasowanie ścieżki losowo (generowanie losowej permutacji)
    //     std::shuffle(path.begin() + 1, path.end(), rng); // Tasowanie wszystkich oprócz pierwszego elementu
    //
    //     // Obliczenie kosztu bieżącej ścieżki
    //     int totalCost = calculatePathCost(path);
    //
    //     // Aktualizacja najlepszego kosztu, jeśli bieżąca ścieżka ma niższy koszt
    //     if (totalCost < bestCost) {
    //         bestCost = totalCost;
    //         bestPath = path;
    //
    //         // Zapisz nową najlepszą ścieżkę i koszt
    //         minCost = bestCost;
    //         this->bestPath = bestPath;
    //
    //         // Zakończ, jeśli osiągnęliśmy lub przekroczyliśmy optymalny koszt
    //         if (minCost <= optimalCost) {
    //             if (display) {
    //                 std::cout << "\nFound solution with cost <= " << optimalCost << ".\n";
    //             }
    //             break;
    //         }
    //     }
    //
    //     // Sprawdzenie, czy osiągnięto limit czasu (przekształcony z minut na sekundy)
    //     auto currentTime = std::chrono::high_resolution_clock::now();
    //     std::chrono::duration<double> elapsed = currentTime - start;
    //     if (elapsed.count() >= timeLimitMinutes * 60.0) {
    //         if (display) {
    //             std::cout << "\nTime limit reached.\n";
    //         }
    //         break;
    //     }
    //
    //     // Wyświetlanie postępu co 100 iteracji
    //     if (display) {
    //         ++iterationCount;
    //         if (iterationCount % 100 == 0) {
    //             std::cout << ".";
    //         }
    //     }
    // }
    //
    // auto end = std::chrono::high_resolution_clock::now();
    // executionTime = end - start;
    //
    // if (display) {
    //     std::cout << "\nCalculation completed.\n";
    // }
    //
    // // Zwrócenie najlepszego znalezionego kosztu
    // return minCost;
}

// Funkcja do obliczania kosztu danej ścieżki
int RandomSolver::calculatePathCost(const std::vector<int>& path)  {
    int totalCost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalCost += graph.getEdge(path[i], path[i + 1]);
    }
    totalCost += graph.getEdge(path.back(), path.front());  // Zamknięcie cyklu
    return totalCost;
}

// Funkcja do zwracania najlepszej ścieżki jako ciąg znaków
std::string RandomSolver::getBestPath() const {
    std::ostringstream oss;
    for (const auto &node: bestPath) {
        oss << node << " ";
    }
    return oss.str();
}

// Funkcja do zwracania czasu wykonania
std::chrono::duration<double> RandomSolver::getExecutionTime() const {
    return executionTime;
}

// Funkcja do zwracania minimalnego kosztu
int RandomSolver::getMinCost() const {
    return minCost;
}