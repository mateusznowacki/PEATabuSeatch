#include "AntColonyOptimization.h"
#include <cmath>
#include <limits>
#include <random>

AntColonyOptimization::AntColonyOptimization(
    int nodes,
    float evaporationRate,
    int iterations,
    float alpha,
    float beta,
    int type,
    float Q,
    int opt
) : n(nodes),
    evaporationRate(evaporationRate),
    iterations(iterations),
    alpha(alpha),
    beta(beta),
    type(type),
    Q(Q),
    opt(opt),
    numOfAnts(nodes)
{
    pheromones.resize(n, std::vector<float>(n, 0.00001f));
    std::random_device rd;
    gen = std::mt19937(rd());
}

std::vector<float> AntColonyOptimization::calculateProbabilities(
    const std::vector<std::vector<int>>& graph,
    int currentVertex,
    const std::vector<bool>& ifVisited
) {
    std::vector<float> probabilities(n, 0.0f);
    float pheroms = 0.0f;

    // Suma istotności feromonów oraz składnika heurystycznego
    for (int j = 0; j < n; j++) {
        if (!ifVisited[j] && graph[currentVertex][j] > 0) {
            pheroms += std::pow(pheromones[currentVertex][j], alpha)
                       * std::pow(1.0f / graph[currentVertex][j], beta);
        }
    }

    // Wyliczanie prawdopodobieństwa wyboru kolejnego wierzchołka
    for (int j = 0; j < n; j++) {
        if (!ifVisited[j] && graph[currentVertex][j] > 0) {
            float nij = 1.0f / graph[currentVertex][j];
            float up  = std::pow(pheromones[currentVertex][j], alpha) * std::pow(nij, beta);
            probabilities[j] = up / pheroms;
        }
    }

    return probabilities;
}

int AntColonyOptimization::getNextNode(const std::vector<float>& probabilities) {
    // Losowy wybór wierzchołka na podstawie wyliczonych prawdopodobieństw
    std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
    return dist(gen);
}

void AntColonyOptimization::updatePheromones(
    const std::vector<int>& route,
    int weight,
    std::vector<std::vector<float>>& currentPheromones
) {
    for (size_t i = 0; i < route.size() - 1; i++) {
        int from = route[i];
        int to   = route[i + 1];

        // Różne warianty aktualizacji feromonów (CAS, DAS)
        if (type == 0) { // CAS
            currentPheromones[from][to] += Q / weight;
        } else if (type == 1) { // DAS
            currentPheromones[from][to] += Q;
        }
    }
}

std::vector<int> AntColonyOptimization::solve(const std::vector<std::vector<int>>& graph) {
    // Wektor rozwiązania: (0,1,2,...,0, najlepszy_koszt)
    // Rezerwujemy n+2 miejsca: n+1 na trasę (ze zdublowanym początkiem)
    // plus 1 na wagę (koszt) najlepszej trasy
    std::vector<int> bestRoute(n + 2, 0);
    bestRoute.back() = std::numeric_limits<int>::max();

    for (int iter = 0; iter < iterations; iter++) {
        // Parowanie (odparowywanie) feromonów
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                pheromones[i][j] *= evaporationRate;
            }
        }

        // Macierz lokalnych przyrostów feromonów
        std::vector<std::vector<float>> currentPheromones(n, std::vector<float>(n, 0.00001f));

        // Każda mrówka buduje rozwiązanie
        for (int ant = 0; ant < numOfAnts; ant++) {
            std::vector<int> currentRoute;
            std::vector<bool> ifVisited(n, false);
            int weight = 0;

            // Startujemy z wierzchołka = numer mrówki (lub można wybrać 0)
            int currentVertex = ant;
            currentRoute.push_back(currentVertex);
            ifVisited[currentVertex] = true;

            // Odwiedzamy kolejne wierzchołki
            while ((int)currentRoute.size() < n) {
                auto probabilities = calculateProbabilities(graph, currentVertex, ifVisited);
                currentVertex = getNextNode(probabilities);
                weight += graph[currentRoute.back()][currentVertex];
                currentRoute.push_back(currentVertex);
                ifVisited[currentVertex] = true;
            }

            // Powrót do wierzchołka startowego
            weight += graph[currentRoute.back()][currentRoute.front()];
            currentRoute.push_back(currentRoute.front());

            // Aktualizacja feromonów dla tej trasy
            updatePheromones(currentRoute, weight, currentPheromones);

            // Sprawdzenie, czy to najlepsze dotąd znalezione rozwiązanie
            if (bestRoute.back() > weight) {
                bestRoute = currentRoute;
                bestRoute.push_back(weight); // doklejamy wagi na koniec
            }
        }

        // Globalna aktualizacja feromonów – sumujemy to, co zebrały wszystkie mrówki
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                pheromones[i][j] += currentPheromones[i][j];
            }
        }
    }

    return bestRoute;
}














