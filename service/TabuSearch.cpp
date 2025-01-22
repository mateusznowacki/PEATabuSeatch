#include "TabuSearch.h"
#include <algorithm>
#include <limits>
#include <random>
#include <iostream>
#include <sstream>

// Pomocnicza funkcja do tworzenia hasha (klucza) z wektora (rozwiązania)
static std::string hashSolution(const std::vector<int>& solution)
{
    std::ostringstream oss;
    for (int node : solution)
    {
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
    // Można tu coś dodatkowo zainicjalizować, jeśli potrzeba
}

// Główna metoda rozwiązująca problem
std::vector<int> TabuSearch::solve(
    const std::vector<std::vector<int>>& graph,
    const std::vector<int>& initialSolution,
    int optimumCost
)
{
    // 1) Jeśli dynamicTabuList = true, ustalamy bieżący rozmiar tabu
    if (dynamicTabuList)
    {
        currentTabuListSize = static_cast<int>(initialSolution.size() / 3);
        // na wszelki wypadek minimalny rozmiar to 1
        if (currentTabuListSize < 1)
        {
            currentTabuListSize = 1;
        }
    }

    // 2) Pomiar czasu startu
    startTime = std::chrono::steady_clock::now();

    // 3) Oblicz koszt ścieżki początkowej
    int currentCost = 0;
    for (size_t i = 0; i < initialSolution.size() - 1; ++i)
    {
        currentCost += graph[initialSolution[i]][initialSolution[i + 1]];
    }
    // Dodaj powrót do wierzchołka startowego
    currentCost += graph[initialSolution.back()][initialSolution.front()];

    // Ustawiamy aktualne oraz najlepsze rozwiązanie
    std::vector<int> currentSolution = initialSolution;
    std::vector<int> bestOverallSolution = initialSolution;
    bestCost = currentCost;

    // 4) Główna pętla Tabu Search
    while (true)
    {
        // a) Warunek czasu
        if (isTimeExceeded())
        {
            break;
        }
        // b) Warunek maksymalnej liczby iteracji bez poprawy
        if (iterWithoutImprovement >= maxIterNoImprove)
        {
            break;
        }
        // c) Warunek osiągnięcia wymaganego procentu optymalnego kosztu
        //    Jeśli bestCost <= procentOpt * optimumCost -> stop
        if (currentCost <= procentOpt * optimumCost)
        {
            break;
        }

        // 5) Generowanie sąsiedztwa aktualnego rozwiązania
        auto neighborhood = generateNeighborhood(currentSolution);

        int bestNeighborCost = std::numeric_limits<int>::max();
        std::vector<int> bestNeighbor;

        // 6) Przeglądanie sąsiedztwa
        for (const auto& neighbor : neighborhood)
        {
            // Obliczamy koszt kandydata
            int neighborCost = 0;
            for (size_t i = 0; i < neighbor.size() - 1; ++i)
            {
                neighborCost += graph[neighbor[i]][neighbor[i + 1]];
            }
            // Powrót do startu
            neighborCost += graph[neighbor.back()][neighbor.front()];

            bool tabuStatus = isTabu(neighbor);
            bool aspiration = (neighborCost < bestCost);

            // Kryteria: nie tabu (lub spełniona aspiracja) i najlepszy z dotychczasowych
            if ((!tabuStatus || aspiration) && neighborCost < bestNeighborCost)
            {
                bestNeighbor = neighbor;
                bestNeighborCost = neighborCost;
            }
        }

        // 7) Aktualizacja bieżącego rozwiązania, jeśli znaleziono lepszego sąsiada
        if (!bestNeighbor.empty())
        {
            currentSolution = bestNeighbor;
            currentCost = bestNeighborCost;

            // Aktualizujemy listę tabu (wstawiamy nowe rozwiązanie)
            updateTabuList(bestNeighbor);

            // Sprawdź, czy poprawiliśmy najlepsze dotychczasowe
            if (currentCost < bestCost)
            {
                bestCost = currentCost;
                bestOverallSolution = currentSolution;
                iterWithoutImprovement = 0;
            }
            else
            {
                ++iterWithoutImprovement;
            }
        }
        else
        {
            // Jeśli nie znaleźliśmy w sąsiedztwie niczego dozwolonego lepszego,
            // to możemy traktować to jako kolejną iterację bez poprawy
            ++iterWithoutImprovement;
        }

        // 8) Dywersyfikacja po pewnej liczbie iteracji bez poprawy
        if (iterWithoutImprovement > 10)
        {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(currentSolution.begin(), currentSolution.end(), g);

            // Oblicz koszt po przetasowaniu
            currentCost = 0;
            for (size_t i = 0; i < currentSolution.size() - 1; ++i)
            {
                currentCost += graph[currentSolution[i]][currentSolution[i + 1]];
            }
            currentCost += graph[currentSolution.back()][currentSolution.front()];

            iterWithoutImprovement = 0; // reset stagnacji
        }
    }

    // 9) Zakończyliśmy pętlę, więc ustawiamy czas końcowy i obliczamy łączny czas
    endTime = std::chrono::steady_clock::now();
    // Przechowujemy różnicę w algorithmTime w postaci time_point przesuniętego od epoki:
    algorithmTime = std::chrono::steady_clock::time_point(endTime - startTime);


    bestPath = bestOverallSolution;
    return bestOverallSolution;
}

// --------------------------------------------------
// Metody pomocnicze
// --------------------------------------------------

// Generowanie sąsiedztwa
std::vector<std::vector<int>> TabuSearch::generateNeighborhood(const std::vector<int>& solution)
{
    std::vector<std::vector<int>> neighborhood;

    if (genNeighbour == "SWAP")
    {
        // Sąsiedztwo przez zamianę par elementów
        for (size_t i = 0; i < solution.size(); ++i)
        {
            for (size_t j = i + 1; j < solution.size(); ++j)
            {
                std::vector<int> neighbor = solution;
                std::swap(neighbor[i], neighbor[j]);
                neighborhood.push_back(neighbor);
            }
        }
    }
    else if (genNeighbour == "INSERT")
    {
        // Sąsiedztwo przez "INSERT"
        for (size_t i = 0; i < solution.size(); ++i)
        {
            for (size_t j = 0; j < solution.size(); ++j)
            {
                if (i != j)
                {
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

// Sprawdza, czy rozwiązanie jest już w tabu
bool TabuSearch::isTabu(const std::vector<int>& solution)
{
    std::string hash = hashSolution(solution);
    return (tabuSet.find(hash) != tabuSet.end());
}

// Aktualizuje listę tabu (wstawia nowe rozwiązanie, usuwa najstarsze przy przekroczeniu limitu)
void TabuSearch::updateTabuList(const std::vector<int>& solution)
{
    std::string hash = hashSolution(solution);
    tabuList.push(hash);
    tabuSet.insert(hash);

    while ((int)tabuList.size() > currentTabuListSize)
    {
        auto frontHash = tabuList.front();
        tabuList.pop();
        tabuSet.erase(frontHash);
    }
}

// Sprawdza, czy przekroczono maksymalny czas (timeMax minut)
bool TabuSearch::isTimeExceeded()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::minutes>(currentTime - startTime).count();
    return (elapsedTime >= timeMax);
}

// Gettery czasu
std::chrono::steady_clock::time_point TabuSearch::getStartTime()
{
    return startTime;
}

std::chrono::steady_clock::time_point TabuSearch::getEndTime()
{
    return endTime;
}

std::chrono::steady_clock::time_point TabuSearch::getAlgorithmTime()
{
    return algorithmTime;
}

long long TabuSearch::getElapsedTime() {
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
}



// Gettery
double TabuSearch::getProcentOpt()
{
    return procentOpt;
}

int TabuSearch::getMaxIterNoImprove()
{
    return maxIterNoImprove;
}

bool TabuSearch::isDynamicTabuList()
{
    return dynamicTabuList;
}

int TabuSearch::getTabuListSize()
{
    return tabuListSize;
}

int TabuSearch::getTimeMax()
{
    return timeMax;
}

std::string TabuSearch::getGenNeighbour()
{
    return genNeighbour;
}

int TabuSearch::getCurrentTabuListSize()
{
    return currentTabuListSize;
}

int TabuSearch::getIterWithoutImprovement()
{
    return iterWithoutImprovement;
}

int TabuSearch::getBestCost()
{
    return bestCost;
}

std::vector<int> TabuSearch::getBestPath()
{
    return bestPath;
}
