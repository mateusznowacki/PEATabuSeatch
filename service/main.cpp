#include <iostream>
#include <vector>
#include <conio.h> // dla _getch() - czekanie na klawisz
#include <chrono>  // dla pomiaru czasu

#include "../utils/ConsolePrinter.h"
#include "../utils/FileController.h"
#include "AntColonyOptimization.h"

// Tutaj ewentualnie include do innego solvera startowego (np. NearestNeighbour, RandomSolver)
// jeśli chciałbyś generować jakąś ścieżkę startową.
// W ACO zwykle generujemy od mrówek, więc nie zawsze jest to potrzebne.

void waitForExit();

int main()
{
    FileController fileController;
    ConsolePrinter consolePrinter;

    ConfigDataDto configData;
    std::vector<std::vector<int>> graph;
    int cost = 0; // np. wczytany koszt optymalny (jeśli dostępny)

    try
    {
        // 1. Wczytujemy config
        configData = fileController.readConfigFile("config.txt");
        configData.printDebugInfo();

        // 2. Tworzymy instancję ACO w zależności od tego, co wczytaliśmy
        //    W większości implementacji klasa ACO potrzebuje:
        //    - n (liczba wierzchołków)
        //    - evaporationRate (rho)
        //    - numOfIterations
        //    - alpha, beta
        //    - type (0=CAS, 1=DAS, 2=QAS itp.)
        //    - Q
        //    - opt (opcjonalnie, jeśli chcesz)

        // Ale "n" w praktyce ustalimy dopiero PO wczytaniu grafu (graph.size()).
        // Więc najpierw sprawdzamy, czy test_mode czy nie.

        if (!configData.getTestMode())
        {
            // --------------------------------------------------------------------
            // Tryb standardowy (pojedynczy plik)
            // --------------------------------------------------------------------
            // 2a. Wczytanie grafu:
            graph = fileController.readGraphFromFile(configData.getInputFile(), configData.getTestMode());
            int n = static_cast<int>(graph.size());

            // 2b. Tworzymy ACO
            //     Odczytaj parametry z configu (alpha, beta, pheromoneQ, pheromoneEvaporation, itp.)
            //     W configu jest "method" -> "cas", "das", ...
            //     Zakładam: method="cas" => type=0, "das" => type=1.
            int acoType = 0; // domyślnie CAS
            if (configData.getMethod() == "cas") {
                acoType = 0;
            } else if (configData.getMethod() == "das") {
                acoType = 1;
            }
            // (ew. inne typy, jeżeli masz QAS = 2 itd.)

            AntColonyOptimization aco(
                n,
                static_cast<float>(configData.getPheromoneEvaporation()), // evaporationRate (rho)
                configData.getNumOfIterations(),
                static_cast<float>(configData.getAlpha()),
                static_cast<float>(configData.getBeta()),
                acoType,
                static_cast<float>(configData.getPheromoneQ()),
                0 // "opt", jeśli potrzebujesz, możesz wczytać cost
            );

            // 2c. Jeżeli masz w pliku optymalny koszt, możesz go wczytać:
            cost = fileController.readCost(configData.getInputFile(), false);

            // 3. Pomiar czasu start
         auto start = std::chrono::steady_clock::now();


            // 4. Rozwiązanie ACO
            std::vector<int> bestRoute = aco.solve(graph);

            auto end = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(end - start).count();

            // 6. Zwykle ostatni element wektora "bestRoute" to koszt (zależnie od implementacji).
            int bestCost = 0;
            if (!bestRoute.empty()) {
                bestCost = bestRoute.back();
                // Możesz usunąć ten koszt z wektora, jeśli chcesz mieć tylko wierzchołki:
                bestRoute.pop_back();
            }

            // 7. Wypisanie wyniku
            consolePrinter.printEndInfo(bestCost, bestRoute, elapsed, cost);

            // 8. Zapis do CSV
            //    Możesz tu dodać dodatkowe parametry (alpha, beta, itd.), jeśli tego wymagasz w analizie:
            fileController.saveResultsToCSV(
                configData.getOutputFile(),     // plik CSV
                configData.getInputFile(),      // instancja
                cost,                           // koszt optymalny (jeśli go znamy)
                bestCost,                       // koszt znaleziony
                bestRoute,                      // trasa
                start,                              // startTime -> ewentualnie w innej postaci
                end                               // endTime -> ewentualnie w innej postaci
                // Możesz dołożyć parametry: alpha, beta, rho, Q...
            );
        }
        else
        {
            // --------------------------------------------------------------------
            // Tryb testowy (wiele plików w folderze, wiele powtórzeń)
            // --------------------------------------------------------------------
            // 1) Pobieramy listę plików z katalogu "instances"
            std::vector<std::string> fileList = fileController.getFileNamesFromDirectory();

            // 2) Dla każdego pliku w folderze
            for (const auto& filePath : fileList)
            {
                // a) Przygotowujemy nazwę pliku CSV
                std::string fileName = filePath;
                size_t dotPos = fileName.find_last_of('.');
                if (dotPos != std::string::npos)
                {
                    fileName.erase(dotPos); // usuwa rozszerzenie np. ".txt"
                }
                fileName += ".csv";  // np. "./instances/graph1.csv"

                // b) Wczytujemy graf
                std::vector<std::vector<int>> graphLocal = fileController.readGraphFromFile(filePath, true);
                int n = graphLocal.size();

                if (graphLocal.empty()) {
                    throw std::runtime_error("Graf w pliku " + filePath + " jest pusty lub niepoprawny.");
                }
                for (const auto& row : graphLocal) {
                    if (row.size() != static_cast<size_t>(n)) {
                        throw std::runtime_error("Niepoprawny rozmiar wiersza w grafie: " + filePath);
                    }
                }

                int costLocal = fileController.readCost(filePath, true);

                // c) Kilka powtórzeń (np. 5)
                int numberOfRuns = 5;
                for (int run = 1; run <= numberOfRuns; ++run)
                {
                    // *Tworzymy obiekt ACO* za każdym razem (lub raz, zależy od Ciebie)
                    int acoType = 0;
                    if (configData.getMethod() == "cas") {
                        acoType = 0;
                    } else if (configData.getMethod() == "das") {
                        acoType = 1;
                    }

                    AntColonyOptimization aco(
                        n,
                        static_cast<float>(configData.getPheromoneEvaporation()),
                        configData.getNumOfIterations(),
                        static_cast<float>(configData.getAlpha()),
                        static_cast<float>(configData.getBeta()),
                        acoType,
                        static_cast<float>(configData.getPheromoneQ()),
                        0 // opt
                    );

                    // 3. Pomiar czasu start
                    auto start = std::chrono::steady_clock::now();


                    // 4. Rozwiązanie ACO
                    std::vector<int> bestRouteLocal = aco.solve(graphLocal);

                    auto end = std::chrono::steady_clock::now();
                    double elapsed = (end - start).count();

                    // Wyciągamy koszt
                    int bestCostLocal = 0;
                    if (!bestRouteLocal.empty()) {
                        bestCostLocal = bestRouteLocal.back();
                        bestRouteLocal.pop_back();
                    }

                    // Wypisujemy na konsolę
                    consolePrinter.printEndInfo(bestCostLocal, bestRouteLocal, elapsed, costLocal);

                    // Zapis do CSV (w trybie std::ios::app -> dopisywanie do istniejącego pliku)
                    // Możesz dodać parametry run, alpha, beta, itp.
                    fileController.saveResultsToCSV(
                        fileName,      // plik CSV (np. "graph1.csv")
                        filePath,      // instancja
                        costLocal,     // koszt optymalny
                        bestCostLocal, // koszt znaleziony
                        bestRouteLocal,
                        start,             // startTime (jeśli chcesz zapisać w milisekundach, zrób to samo co w Tabu)
                        end              // endTime
                        // Dodaj tu parametry (run, alpha, beta, rho, Q), jeśli potrzebujesz w analizie
                    );
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Wystąpił błąd: " << e.what() << std::endl;
        waitForExit();
        return 1;
    }

    return 0;
}

void waitForExit()
{
    std::cout << "Nacisnij spacje zeby zakonczyc" << std::endl;
    while (true)
    {
        char key = _getch(); // Odczyt pojedynczego znaku
        if (key == ' ')
        {
            break; // Przerwij pętlę, jeśli wciśnięto spację
        }
    }
}
