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
                initial_solution = nearestNeighbour.solve(graph);
            }
            else if (configData.getAlgorithmType() == "random")
            {
                initial_solution = randomSolver.solve(graph);
            }

            cost = fileController.readCost(configData.getInputFile(), false);


            std::vector<int> bestFoundPath = tabuSearch.solve(graph, initial_solution, cost);

             consolePrinter.printEndInfo(tabuSearch.getBestCost(), bestFoundPath, tabuSearch.getElapsedTime(),cost);
            fileController.saveResultsToCSV(configData.getOutputFile(), configData.getInputFile(),
                                            cost,tabuSearch.getBestCost(), bestFoundPath, tabuSearch.getStartTime(),
                                            tabuSearch.getEndTime());

            }
            else
            if (configData.getTestMode() == true)
            {
                TabuSearch tabuSearch(configData.getProcentOpt(), configData.getMaxIterNoImprove(),
                                      configData.getDynamicTabuList(), configData.getTabuListSize(),
                                      configData.getTimeMax(),
                                      configData.getGenNeighbour());

            }
        }

        catch
        (std::exception & e)
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
