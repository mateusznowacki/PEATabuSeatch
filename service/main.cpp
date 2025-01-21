#include <iostream>
#include <vector>
#include <conio.h>
#include "TabuSearch.h"
#include "NearestNeighbour.h"
#include "RandomSolver.h"
#include "../utils/ConsolePrinter.h"
#include "../utils/FileController.h"

void waitForExit();


int main()
{
    FileController fileController;
    ConsolePrinter consolePrinter;
    NearestNeighbour nearestNeighbour;
    RandomSolver randomSolver;

    ConfigDataDto configData;
    std::vector<std::vector<int>> graph;
    std::vector<int> bestSolution;
    int cost;


    try
    {
        ConfigDataDto configData = fileController.readConfigFile("config.txt");
        configData.printDebugInfo();

        if (configData.getTestMode() == false)
        {
            //init tabu search
            TabuSearch tabuSearch(configData.getProcentOpt(), configData.getMaxIterNoImprove(),
                                  configData.getDynamicTabuList(), configData.getTabuListSize(),
                                  configData.getTimeMax(),
                                  configData.getGenNeighbour());

            //pobranie grafu
            graph = fileController.readGraphFromFile(configData.getInputFile(), configData.getTestMode());

            if (configData.getAlgorithmType() == "nn")
            {
                bestSolution = nearestNeighbour.solve(graph);
                cost = nearestNeighbour.getMinCost();
            }
            else if (configData.getAlgorithmType() == "random")
            {
                bestSolution = randomSolver.solve(graph,1);
                cost = randomSolver.getMinCost();
            }


            tabuSearch.solve(graph, bestSolution, cost);

            //wyswietl wyniki  plus zapisz do pliku
            // consolePrinter.printEndInfo(tabuSearch.getMinCost(), tabuSearch.getBestPath(), tabuSearch.getExecutionTime());
            //  fileController.saveResultsToCSV();
        }
        else if (configData.getTestMode() == true)
        {
            TabuSearch tabuSearch(configData.getProcentOpt(), configData.getMaxIterNoImprove(),
                                  configData.getDynamicTabuList(), configData.getTabuListSize(),
                                  configData.getTimeMax(),
                                  configData.getGenNeighbour());

            //dodac wczytywanie z pliku
        }
    }

    catch
    (std::exception& e)
    {
        std::cerr << "Wystąpił błąd: " << e.what() << std::endl;
        waitForExit();
        return 1;
    }

    return
        0;
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
