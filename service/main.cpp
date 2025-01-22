#include <iostream>
#include <vector>
#include <conio.h>
#include "TabuSearch.h"

#include "../utils/ConsolePrinter.h"
#include "../utils/FileController.h"
#include "pea12/NearestNeighbour.h"
#include "pea12/RandomSolver.h"

void waitForExit();


int main()
{
    FileController fileController;
    ConsolePrinter consolePrinter;
    NearestNeighbour nearestNeighbour;
    RandomSolver randomSolver;

    ConfigDataDto configData;
    std::vector<std::vector<int>> graph;
    std::vector<int> initial_solution;
    int cost;

    int numberOfRuns = 10;

    try
    {
        ConfigDataDto configData = fileController.readConfigFile("config.txt");
        configData.printDebugInfo();
        if (!configData.getTestMode())
        {
            // --------------------------------------------------------------------
            // Tryb standardowy (nie testowy) - uruchamiamy TabuSearch dla jednego pliku
            // --------------------------------------------------------------------
            TabuSearch tabuSearch(
                configData.getProcentOpt(),
                configData.getMaxIterNoImprove(),
                configData.getDynamicTabuList(),
                configData.getTabuListSize(),
                configData.getTimeMax(),
                configData.getGenNeighbour()
            );

            // Wczytanie grafu z pliku
            graph = fileController.readGraphFromFile(configData.getInputFile(), configData.getTestMode());

            // Wybór algorytmu startowego (nn/random)
            if (configData.getAlgorithmType() == "nn")
            {
                initial_solution = nearestNeighbour.solve(graph);
            }
            else if (configData.getAlgorithmType() == "random")
            {
                initial_solution = randomSolver.solve(graph);
            }

            // Wczytanie optymalnego kosztu sum_min
            cost = fileController.readCost(configData.getInputFile(), false);

            // Uruchom TabuSearch
            std::vector<int> bestFoundPath = tabuSearch.solve(graph, initial_solution, cost);

            // Wypisanie wyniku
            consolePrinter.printEndInfo(tabuSearch.getBestCost(), bestFoundPath, tabuSearch.getElapsedTime(), cost);

            // Zapis do pliku CSV (nazwa określona w configu)
            fileController.saveResultsToCSV(
                configData.getOutputFile(),
                configData.getInputFile(),
                cost,
                tabuSearch.getBestCost(),
                bestFoundPath,
                tabuSearch.getStartTime(),
                tabuSearch.getEndTime()
            );
        }
        else
        {

            // 1) Pobieramy listę plików z katalogu "instances"
            std::vector<std::string> fileList = fileController.getFileNamesFromDirectory();

            // 2) Dla każdego pliku w folderze
            for (const auto& filePath : fileList)
            {
                // a) Przygotowujemy nazwę pliku CSV
                //    usuwamy stare rozszerzenie z filePath i dodajemy ".csv"
                std::string fileName = filePath;
                size_t dotPos = fileName.find_last_of('.');
                if (dotPos != std::string::npos)
                {
                    fileName.erase(dotPos); // usuwa rozszerzenie, np. ".txt"
                }
                fileName += ".csv";  // np. "./instances/graph1.csv"

                // b) Wczytujemy graf i koszt sum_min bez dodatkowego prefixu
                //    => dajemy param. false, bo `filePath` już zawiera "./instances/"
                std::vector<std::vector<int>> graphLocal = fileController.readGraphFromFile(filePath, false);
                int costLocal = fileController.readCost(filePath, false);

                // c) Pętla na 10 powtórzeń (przykład)
                int numberOfRuns = 10;
                for (int run = 1; run <= numberOfRuns; ++run)
                {
                    //  - Tworzymy TabuSearch
                    TabuSearch tabuSearch(
                        configData.getProcentOpt(),
                        configData.getMaxIterNoImprove(),
                        configData.getDynamicTabuList(),
                        configData.getTabuListSize(),
                        configData.getTimeMax(),
                        configData.getGenNeighbour()
                    );

                    //  - Generujemy rozwiązanie startowe (nn / random)
                    std::vector<int> initial_solution_local;
                    if (configData.getAlgorithmType() == "nn")
                    {
                        initial_solution_local = nearestNeighbour.solve(graphLocal);
                    }
                    else if (configData.getAlgorithmType() == "random")
                    {
                        initial_solution_local = randomSolver.solve(graphLocal);
                    }

                    //  - Uruchamiamy TabuSearch
                    std::vector<int> bestFoundPathLocal =
                        tabuSearch.solve(graphLocal, initial_solution_local, costLocal);

                    consolePrinter.printEndInfo(
                        tabuSearch.getBestCost(),
                        bestFoundPathLocal,
                        tabuSearch.getElapsedTime(),
                        costLocal
                    );

                    //  - Zapis do CSV (za każdym razem dopisujemy nowy wiersz w tym samym pliku CSV)
                    //    Zakładamy, że saveResultsToCSV ma parametr np. 'int runNumber'
                    //    i potrafi zapisywać w trybie std::ios::app.
                    fileController.saveResultsToCSV(
                        fileName, // plik CSV np. "graph1.csv"
                        filePath, // nazwa instancji
                        costLocal,
                        tabuSearch.getBestCost(),
                        bestFoundPathLocal,
                        tabuSearch.getStartTime(),
                        tabuSearch.getEndTime()

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
        char key = _getch(); // Odczyt pojedynczego znaku bez oczekiwania na Enter
        if (key == ' ')
        {
            break; // Przerwij pętlę, jeśli wciśnięto spację
        }
    }
}
