#include <iostream>
#include <vector>
#include <conio.h>
#include "service/TabuSearch.h"
#include "service/NearestNeighbour.h"
#include "service/RandomSolver.h"
#include "utils/ConsolePrinter.h"
#include "utils/FileController.h"

void waitForExit();


int main()
{
    FileController fileController;
    ConsolePrinter consolePrinter;
    //NearestNeighbour nearestNeighbour;
    //   RandomSolver randomSolver;

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


            /////////////poprawic to bestSolution
            if (configData.getAlgorithmType() == "nn")
            {
                bestSolution = {0, 1, 2, 3, 4, 5};
                // bestSolution = nearestNeighbour.getBestPath(graph);
            }
            else if (configData.getAlgorithmType() == "random")
            {
                bestSolution = {0, 1, 2, 3, 4, 5};
                //  bestSolution = random.getBestPath(graph);
            }

        //pobranie kosztu
        cost = fileController.readCost(configData.getInputFile(), configData.getTestMode());
        //jesli nie ma to oblicz z nn
        if (cost == INT32_MAX)
        {
            // cost = nearestNeighbour.getMinCost(graph);
        }


        tabuSearch.solve(graph, bestSolution, cost);

        //wyswietl wyniki  plus zapisz do pliku
        // consolePrinter.printEndInfo(tabuSearch.getMinCost(), tabuSearch.getBestPath(), tabuSearch.getExecutionTime());
        //  fileController.saveResultsToCSV();
    }
    else
    if (configData.getTestMode() == true)
    {
        TabuSearch tabuSearch(configData.getProcentOpt(), configData.getMaxIterNoImprove(),
                              configData.getDynamicTabuList(), configData.getTabuListSize(), configData.getTimeMax(),
                              configData.getGenNeighbour());

        //dodac wczytywanie z pliku
    }
}

catch
(std::exception&e)
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
