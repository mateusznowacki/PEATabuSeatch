
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

void ConsolePrinter::printStartInfo(std::string inputFileName, std::string outputFileName, bool displayResults,
                                    bool showProgress, int iterations, int getInputOptimalCost,
                                    std::string getInputOptimalPath, int timeLimit, std::string algorithmType) {
    std::cout << "Nazwa pliku wejsciowego: " << inputFileName << std::endl;
    std::cout << "Nazwa pliku wyjsciowego: " << outputFileName << std::endl;
    std::cout << "Nazwa algorytmu: " << algorithmType << std::endl;
    std::cout << "Wyswietlanie wynikow: " << (displayResults ? "Tak" : "Nie") << std::endl;
    std::cout << "Wyswietlanie postepu: " << (showProgress ? "Tak" : "Nie") << std::endl;
    std::cout << "Maksymalny czas: " << timeLimit << " minut\n";
    std::cout << "Liczba iteracji: " << iterations << std::endl;
    std::cout << "Optymalna sciezka zapisana w pliku wejsciowym: "
              << (getInputOptimalPath.empty() ? "brak danych" : getInputOptimalPath) << std::endl;
    std::cout << "Optymalny koszt zapisany w pliku wejsciowym: "
              << (getInputOptimalCost > 0 ? std::to_string(getInputOptimalCost) : "brak danych") << std::endl;
    std::cout << "#####################################" << std::endl;
}

void ConsolePrinter::printEndInfo(int minCost, std::string bestPath, std::chrono::duration<double> duration, int inputOptimalCost) {
    int absoluteError = std::abs(minCost - inputOptimalCost);
    double relativeError = (inputOptimalCost > 0)
        ? (static_cast<double>(absoluteError) / inputOptimalCost) * 100
        : 0;

    std::cout << "\n#####################################\n";
    std::cout << "Koniec obliczen znalezione wyniki\n";
    std::cout << "Czas wykonania: " << std::fixed << std::setprecision(6) << duration.count() << " sekund\n";
    std::cout << "Najmniejszy koszt: " << minCost << std::endl;
    std::cout << "Najlepsza sciezka: " << (bestPath.empty() ? "brak danych" : bestPath) << std::endl;
    std::cout << "Blad bezwzgledny: " << absoluteError << std::endl;
    std::cout << "Blad wzgledny: " << relativeError << "%" << std::endl;
}

void ConsolePrinter::bestPath(int minCost, std::string basicString) {
    std::cout << "Najlepsza sciezka: " << basicString << std::endl;
}



