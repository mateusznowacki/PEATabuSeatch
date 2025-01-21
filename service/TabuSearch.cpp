#include "TabuSearch.h"
#include <algorithm>
#include <limits>
#include <random>
#include <iostream>
#include <sstream>

// Pomocnicza funkcja do tworzenia "hasza" z wektora (rozwiązania)
static std::string hashSolution(const std::vector<int>& solution) {
    std::ostringstream oss;
    for (int node : solution) {
        oss << node << ",";
    }
    return oss.str();
}

// Konstruktor
TabuSearch::TabuSearch(
    double procentOpt,
    int maxIterNoImprove,
    bool dynamicTabuList,
    int tabuListSize,
    int timeMax,
    std::string genNeighbour
)
    : procentOpt(procentOpt),
      maxIterNoImprove(maxIterNoImprove),
      dynamicTabuList(dynamicTabuList),
      tabuListSize(tabuListSize),
      timeMax(timeMax),
      genNeighbour(genNeighbour),
      currentTabuListSize(tabuListSize),
      iterWithoutImprovement(0)
{
    // UWAGA: Kod poniżej może zostać zmieniony zależnie od przyjętej strategii
    // Dla przykładu, jeśli dynamicTabuList = true, możemy tu nic nie robić,
    // bo i tak wielkość listy będzie modyfikowana w trakcie solve().
}

// Główna metoda rozwiązująca problem
std::vector<int> TabuSearch::solve(
    const std::vector<std::vector<int>>& graph,
    const std::vector<int>& bestSolution,
    int initialCost
) {
    // Inicjalizacja czasu startu i wektorów
    startTime = std::chrono::steady_clock::now();
    std::vector<int> currentSolution = bestSolution;
    std::vector<int> bestOverallSolution = bestSolution;
    int currentCost = initialCost;
    int bestCost = initialCost;

    // Główna pętla
    while (!isTimeExceeded() && iterWithoutImprovement < maxIterNoImprove) {
        // -------------------------
        // 1. Sprawdź kryterium procentOpt
        //    Jeśli bestCost spadnie poniżej procentOpt * initialCost,
        //    można przerwać algorytm (np. wystarczająco dobry wynik).
        if (bestCost <= procentOpt * initialCost) {
            std::cout << "[DEBUG] Stop by procentOpt criterion: "
                      << bestCost << " <= " << procentOpt * initialCost << "\n";
            break;
        }
        // -------------------------

        // 2. Generowanie sąsiedztwa
        auto neighborhood = generateNeighborhood(currentSolution);

        int bestNeighborCost = std::numeric_limits<int>::max();
        std::vector<int> bestNeighbor;

        // 3. Przeglądanie wygenerowanego sąsiedztwa
        for (size_t nIdx = 0; nIdx < neighborhood.size(); ++nIdx) {
            const auto& neighbor = neighborhood[nIdx];

            // Obliczamy koszt sąsiada (z domknięciem cyklu)
            int neighborCost = 0;
            for (size_t i = 0; i < neighbor.size() - 1; ++i) {
                neighborCost += graph[neighbor[i]][neighbor[i + 1]];
            }
            neighborCost += graph[neighbor.back()][neighbor.front()]; // powrót do startu

            bool tabuStatus = isTabu(neighbor);
            bool aspiration = (neighborCost < bestCost);

            // Kryteria tabu i aspiracji
            if ((!tabuStatus || aspiration) && neighborCost < bestNeighborCost) {
                bestNeighbor = neighbor;
                bestNeighborCost = neighborCost;
            }
        }

        // 4. Aktualizacja rozwiązania
        if (!bestNeighbor.empty()) {
            currentSolution = bestNeighbor;
            currentCost = bestNeighborCost;

            // Zaktualizuj listę tabu
            updateTabuList(bestNeighbor);

            // Sprawdź, czy mamy nowe najlepsze rozwiązanie
            if (currentCost < bestCost) {
                bestCost = currentCost;
                bestOverallSolution = currentSolution;
                iterWithoutImprovement = 0; // reset licznika

                // Jeśli dynamicTabuList jest włączone,
                // możemy np. zmniejszyć rozmiar listy tabu,
                // bo właśnie znaleźliśmy lepsze rozwiązanie:
                if (dynamicTabuList) {
                    // Przykład: minimalnie 5, a tak normalnie = tabuListSize
                    currentTabuListSize = std::max(5, tabuListSize - 2);
                }
            } else {
                ++iterWithoutImprovement;

                // Jeśli dynamicTabuList jest włączone i z rzędu nie znajdujemy poprawy,
                // możemy np. zwiększać rozmiar listy tabu:
                if (dynamicTabuList) {
                    // Prosty przykład: zwiększ rozmiar, jeśli jest sporo stagnacji
                    if (iterWithoutImprovement % 5 == 0) {
                        currentTabuListSize += 1;
                    }
                }
            }
        }

        // 5. Dywersyfikacja: proste losowe przetasowanie po "x" stagnacjach
        if (iterWithoutImprovement > 10) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(currentSolution.begin(), currentSolution.end(), g);

            // Oblicz koszt po przetasowaniu (z domknięciem cyklu)
            currentCost = 0;
            for (size_t i = 0; i < currentSolution.size() - 1; ++i) {
                currentCost += graph[currentSolution[i]][currentSolution[i + 1]];
            }
            currentCost += graph[currentSolution.back()][currentSolution.front()];

            iterWithoutImprovement = 0; // reset stagnacji

            // Można też w ramach dywersyfikacji zmienić listę tabu
            if (dynamicTabuList) {
                currentTabuListSize = tabuListSize; // np. przywróć do wartości bazowej
            }
        }
    }

    // Debug: wynik końcowy
    std::cout << "\n[DEBUG] Final best solution: ";
    for (const auto& node : bestOverallSolution) {
        std::cout << node << " ";
    }
    std::cout << "\n[DEBUG] Final cost: " << bestCost << "\n";

    return bestOverallSolution;
}

// Generowanie sąsiedztwa
std::vector<std::vector<int>> TabuSearch::generateNeighborhood(const std::vector<int>& solution) {
    std::vector<std::vector<int>> neighborhood;

    if (genNeighbour == "SWAP") {
        // Sąsiedztwo przez zamianę par elementów
        for (size_t i = 0; i < solution.size(); ++i) {
            for (size_t j = i + 1; j < solution.size(); ++j) {
                std::vector<int> neighbor = solution;
                std::swap(neighbor[i], neighbor[j]);
                neighborhood.push_back(neighbor);
            }
        }
    } else if (genNeighbour == "INSERT") {
        // Sąsiedztwo przez "insert"
        for (size_t i = 0; i < solution.size(); ++i) {
            for (size_t j = 0; j < solution.size(); ++j) {
                if (i != j) {
                    std::vector<int> neighbor = solution;
                    int element = neighbor[i];
                    neighbor.erase(neighbor.begin() + i);
                    neighbor.insert(neighbor.begin() + j, element);
                    neighborhood.push_back(neighbor);
                }
            }
        }
    }
    return neighborhood;
}

// Sprawdza, czy rozwiązanie jest tabu
bool TabuSearch::isTabu(const std::vector<int>& solution) {
    std::string hash = hashSolution(solution);
    return tabuSet.find(hash) != tabuSet.end();
}

// Aktualizuje listę tabu
void TabuSearch::updateTabuList(const std::vector<int>& solution) {
    std::string hash = hashSolution(solution);
    tabuList.push(hash);
    tabuSet.insert(hash);

    // Usuwanie najstarszego wpisu, jeśli rozmiar listy tabu przekroczony
    while (static_cast<int>(tabuList.size()) > currentTabuListSize) {
        auto expiredHash = tabuList.front();
        tabuList.pop();
        tabuSet.erase(expiredHash);
    }
}

// Sprawdza, czy przekroczono maksymalny czas (timeMax minut)
bool TabuSearch::isTimeExceeded() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::minutes>(currentTime - startTime).count();
    return (elapsedTime >= timeMax);
}
