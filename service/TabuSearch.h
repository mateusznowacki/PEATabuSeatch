#ifndef TABUSEARCH_H
#define TABUSEARCH_H

#include <vector>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <string>

// Definicja sposobu generowania sąsiedztwa
enum NeighborGenerationMethod {
    SWAP,
    INSERT
};

class TabuSearch {
public:
    // Konstruktor
    TabuSearch(
        double procentOpt,
        int maxIterNoImprove,
        bool dynamicTabuList,
        int tabuListSize,
        int timeMax,
        std::string genNeighbour
    );

    // Główna metoda uruchamiająca algorytm Tabu Search
    std::vector<int> solve(
        const std::vector<std::vector<int>>& graph,
        const std::vector<int>& bestSolution,
        int initialCost
    );

private:
    // Parametry konstruktora
    double procentOpt;            // NIEUŻYWANY w poniższej implementacji
    int maxIterNoImprove;         // Maksymalna liczba iteracji bez poprawy rozwiązania
    bool dynamicTabuList;         // NIEUŻYWANY w poniższej implementacji (brak dynamicznej wielkości listy tabu)
    int tabuListSize;             // Rozmiar listy tabu
    int timeMax;                  // Maksymalny czas w minutach
    std::string genNeighbour; // Metoda generowania sąsiedztwa

    // Zmienna pomocnicza do pomiaru czasu rozpoczęcia
    std::chrono::steady_clock::time_point startTime;

    // Struktury wspomagające obsługę listy tabu
    std::queue<std::string> tabuList;         // Kolejka do przechowywania rozwiązań (hashów)
    std::unordered_set<std::string> tabuSet;  // Zbiór do szybkiego sprawdzania, czy rozwiązanie jest tabu

    // Inne pola pomocnicze
    int currentTabuListSize;      // Aktualny (bieżący) rozmiar listy tabu (stały w tej wersji)
    int iterWithoutImprovement;   // Licznik iteracji bez poprawy

    // Metody pomocnicze
    bool isTabu(const std::vector<int>& solution);
    void updateTabuList(const std::vector<int>& solution);
    bool isTimeExceeded();

    // Generowanie sąsiedztwa
    std::vector<std::vector<int>> generateNeighborhood(const std::vector<int>& solution);
};

#endif // TABUSEARCH_H
