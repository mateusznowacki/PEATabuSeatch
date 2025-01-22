
#include <iostream>
#include <vector>
#include <iomanip>
#include "ConsolePrinter.h"


void ConsolePrinter::printInfo(std::string filename, std::string optimalPath, int optimalCost) {
    std::cout << "Nazwa pliku z danymi: " << filename << std::endl;
    std::cout << "Optymalna ścieżka: " << optimalPath << std::endl;
    std::cout << "Optymalny koszt: " << optimalCost << std::endl;
}


void ConsolePrinter::displayResults(const std::string& fileName, int calculatedCost, int optimalCost,
                                    const std::vector<int>& calculatedPath, const std::vector<int>& optimalPath,
                                    double executionTime) {
    std::cout << "-------------------------------------------\n";
    std::cout << "Nazwa pliku: " << fileName << std::endl;
    std::cout << "Czas wykonania: " << std::fixed << std::setprecision(6) << executionTime << " sekund\n";
    std::cout << "Wynik dla znalezionego rozwiązania: " << calculatedCost << std::endl;

    std::cout << "Ścieżka dla znalezionego rozwiązania: ";
    for (int node : calculatedPath) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    if (!optimalPath.empty()) {
        std::cout << "Optymalna ścieżka: ";
        for (int node : optimalPath) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }

    // Obliczamy i wyświetlamy błędy bezwzględne i względne
    int absoluteError = calculatedCost - optimalCost;
    double relativeError = (static_cast<double>(absoluteError) / optimalCost) * 100;

    std::cout << "Błąd bezwzględny: " << absoluteError << std::endl;
    std::cout << "Błąd względny: " << std::fixed << std::setprecision(2) << relativeError << " %" << std::endl;
}


void ConsolePrinter::printEndInfo(int minCost, std::vector<int> bestPath, long long duration, int inputOptimalCost) {
    int absoluteError = std::abs(minCost - inputOptimalCost);
    double relativeError = (static_cast<double>(absoluteError) / inputOptimalCost) * 100;

    // Formatting the path
    std::ostringstream pathStream;
    pathStream << "[";
    for (size_t i = 0; i < bestPath.size(); ++i) {
        pathStream << bestPath[i];
        if (i < bestPath.size() - 1) {
            pathStream << ", ";
        }
    }
    pathStream << "]";
    std::string formattedPath = pathStream.str();

    // Printing the information
    std::cout << "\n#####################################\n";
    std::cout << "Koniec obliczen znalezione wyniki\n";
    std::cout << "Czas wykonania: " << std::fixed << std::setprecision(6) << duration << " milisekund\n";
    std::cout << "Najlepsza znaleziona sciezka: " << formattedPath << std::endl;
    std::cout << "Najmniejszy koszt znaleziony: " << minCost << std::endl;
    std::cout << "Koszt opytmalny pobrany z pliku: " << inputOptimalCost << std::endl;
    std::cout << "Blad procent optymalnego rozwiazania: " << (static_cast<double>(minCost) / inputOptimalCost * 100) << "%" << std::endl;
    std::cout << "#####################################\n";
}



void ConsolePrinter::bestPath(int minCost, std::string basicString) {
    std::cout << "Najlepsza sciezka: " << basicString << std::endl;
}



