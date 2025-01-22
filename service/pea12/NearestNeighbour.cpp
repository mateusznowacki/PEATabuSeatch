#include "NearestNeighbour.h"

std::vector<int> NearestNeighbour::solve(const std::vector<std::vector<int>>& graph) {
    int vertices = graph.size();
    if (vertices == 0) {
        return {}; // Zwraca pustą ścieżkę, jeśli graf jest pusty
    }

    minCost = std::numeric_limits<int>::max();

    // Próbuj rozpocząć od każdego wierzchołka
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
                // Dodaj koszt powrotu do wierzchołka początkowego
                int returnCost = graph[current][path.front()];
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
                int distance = graph[current][i];
                if (!visited[i] && distance < minDistance) {
                    // Zaktualizuj minimalną odległość i wyczyść listę kandydatów
                    minDistance = distance;
                    candidates = {i};
                } else if (!visited[i] && distance == minDistance) {
                    // Dodaj do listy kandydatów, jeśli odległość jest równa minimalnej
                    candidates.push_back(i);
                }
            }

            // Dodanie nowych stanów do kolejki dla każdego kandydata
            for (int next : candidates) {
                std::vector<int> newPath = path;
                std::vector<bool> newVisited = visited;
                newPath.push_back(next);
                newVisited[next] = true;
                int newCost = totalCost + graph[current][next];

                // Dodanie nowego stanu do kolejki
                states.push({newPath, newVisited, next, newCost});
            }
        }
    }

    return bestPath;
}

int NearestNeighbour::getMinCost() const {
    return minCost;
}
