#include "NearestNeighbour.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <queue>
#include <chrono>  // Dodane do pomiaru czasu wykonania

/*
 * Implementacja heurystyki najbliższego sąsiada dla problemu komiwojażera (TSP).
 * Algorytm zaczyna od dowolnego wierzchołka, a następnie wybiera najbliższego, nieodwiedzonego sąsiada, aż wszystkie wierzchołki zostaną odwiedzone.
 * Heurystyka ta nie gwarantuje znalezienia optymalnego rozwiązania, ale jest znacznie szybsza niż metoda brute-force.
 */

// Konstruktor
NearestNeighbour::NearestNeighbour()
        : graph(graph), display(display), minCost(std::numeric_limits<int>::max()), executionTime(0) {}

// Rozwiązywanie problemu TSP za pomocą heurystyki najbliższego sąsiada
int NearestNeighbour::solve(int optimalCost, const std::vector<int>& optimalPath) {
    int vertices = graph.getVertices();
    if (vertices <= 0) {
        std::cerr << "Error: The graph contains no vertices." << std::endl;
        return -1;
    }

    // Rozpoczęcie pomiaru czasu
    auto startTime = std::chrono::high_resolution_clock::now();

    // Próba algorytmu rozpoczynająca się od każdego wierzchołka
    for (int start = 0; start < vertices; ++start) {
        // Kolejka do przechowywania stanów (ścieżek) do eksploracji
        std::queue<std::tuple<std::vector<int>, std::vector<bool>, int, int>> states;

        // Inicjalizacja pierwszego stanu dla wybranego wierzchołka początkowego
        std::vector<int> initialPath = {start};
        std::vector<bool> initialVisited(vertices, false);
        initialVisited[start] = true;
        int initialCost = 0;
        states.push({initialPath, initialVisited, start, initialCost});

        // Przetwarzanie stanów w kolejce
        while (!states.empty()) {
            auto [path, visited, current, totalCost] = states.front();
            states.pop();

            // Jeśli odwiedziliśmy wszystkie wierzchołki, zamykamy cykl i aktualizujemy wynik
            if (path.size() == vertices) {
                // Dodaj koszt powrotu na początek
                int returnCost = graph.getEdge(current, path.front());
                int totalCycleCost = totalCost + returnCost;

                // Aktualizuj minimalny koszt i najlepszą ścieżkę
                if (totalCycleCost < minCost) {
                    minCost = totalCycleCost;
                    bestPath = path;
                }
                continue;
            }

            // Znalezienie wszystkich najbliższych sąsiadów o minimalnej wadze
            int minDistance = std::numeric_limits<int>::max();
            std::vector<int> candidates;

            for (int i = 0; i < vertices; ++i) {
                int distance = graph.getEdge(current, i);
                if (!visited[i]) {
                    if (distance < minDistance) {
                        // Zaktualizuj minimalną odległość i wyczyść listę kandydatów
                        minDistance = distance;
                        candidates = {i};
                    } else if (distance == minDistance) {
                        // Dodaj do listy kandydatów, jeśli odległość jest równa minimalnej
                        candidates.push_back(i);
                    }
                }
            }

            // Dodanie nowych stanów do kolejki dla każdego kandydata
            for (int next : candidates) {
                std::vector<int> newPath = path;
                std::vector<bool> newVisited = visited;
                newPath.push_back(next);
                newVisited[next] = true;
                int newCost = totalCost + graph.getEdge(current, next);

                // Dodanie nowego stanu do kolejki
                states.push({newPath, newVisited, next, newCost});
            }
        }
    }

    // Zakończenie pomiaru czasu
    auto endTime = std::chrono::high_resolution_clock::now();
    executionTime = endTime - startTime;

    return minCost;
}

// Funkcja do zwracania najlepszego wyniku jako ciąg znaków
std::string NearestNeighbour::getBestPath() const {
    std::ostringstream oss;
    for (const auto& node : bestPath) {
        oss << node << " ";
    }
    oss << bestPath.front(); // Dodanie wierzchołka początkowego na końcu cyklu
    return oss.str();
}

// Funkcja do zwracania minimalnego kosztu
int NearestNeighbour::getMinCost() const {
    return minCost;
}

// Funkcja do zwracania czasu wykonania
std::chrono::duration<double> NearestNeighbour::getExecutionTime() const {
    return executionTime;
}
