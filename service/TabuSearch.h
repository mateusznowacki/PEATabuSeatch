#ifndef TABUSEARCH_H
#define TABUSEARCH_H

#include <vector>
#include <queue>
#include <unordered_set>
#include <string>
#include <chrono>

class TabuSearch {
private:
    // Parametry konfiguracyjne
    double procentOpt;          // Procent optymalnego kosztu, przy którym kończymy
    int maxIterNoImprove;       // Maksymalna liczba iteracji bez poprawy
    bool dynamicTabuList;       // Czy lista tabu ma rozmiar zależny od liczby wierzchołków
    int tabuListSize;           // Bazowy rozmiar listy tabu (jeśli dynamicTabuList = false)
    int timeMax;                // Maksymalny czas w minutach
    std::string genNeighbour;   // Rodzaj generowania sąsiedztwa: "SWAP" lub "INSERT"

    // Zmienne pomocnicze
    int currentTabuListSize;    // Aktualny (dynamicznie ustalany) rozmiar tabu
    int iterWithoutImprovement; // Licznik iteracji bez poprawy
    std::queue<std::string> tabuList;
    std::unordered_set<std::string> tabuSet;
    std::vector<int> bestPath;

    int bestCost;

    // Zmienne do pomiaru czasu
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
    std::chrono::steady_clock::time_point algorithmTime;


    // Metody pomocnicze
    bool isTabu(const std::vector<int>& solution);
    void updateTabuList(const std::vector<int>& solution);
    bool isTimeExceeded();
    std::vector<std::vector<int>> generateNeighborhood(const std::vector<int>& solution);

public:
    // Konstruktor
    TabuSearch(double procentOpt,
               int maxIterNoImprove,
               bool dynamicTabuList,
               int tabuListSize,
               int timeMax,
               std::string genNeighbour);

    // Główna metoda rozwiązująca problem:
    // - graph: macierz wag
    // - initialSolution: ścieżka startowa
    // - optimumCost: koszt uznawany za optymalny (lub zbliżony do optimum)
    std::vector<int> solve(const std::vector<std::vector<int>>& graph,
                           const std::vector<int>& initialSolution,
                           int optimumCost);

    // Gettery czasu
    std::chrono::steady_clock::time_point getStartTime();
    std::chrono::steady_clock::time_point getEndTime();
    std::chrono::steady_clock::time_point getAlgorithmTime();

    long long getElapsedTimeMilliseconds();
    // Gettery
    double getProcentOpt() ;
    int getMaxIterNoImprove() ;
    bool isDynamicTabuList() ;
    int getTabuListSize() ;
    int getTimeMax() ;
    std::string getGenNeighbour() ;
    int getCurrentTabuListSize() ;
    int getIterWithoutImprovement() ;
    int getBestCost();
    std::vector<int> getBestPath();

};

#endif // TABUSEARCH_H
