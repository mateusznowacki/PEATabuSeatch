#include "FileController.h"
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include <sstream>
#include <cmath>
#include <dirent.h>   // For directory operations
#include <iostream>   // For std::cerr


std::vector<std::vector<int>> FileController::readGraphFromFile(const std::string& filename, bool testMode)
{
    // Określenie ścieżki pliku
    std::string filePath = testMode ? "instances/" + filename : filename;

    // Otwarcie pliku
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Nie można otworzyć pliku: " + filePath);
    }

    // Opcjonalna wartość optymalna (opt)
    int optValue = -1;
    std::string firstLine;
    std::getline(file, firstLine);

    // Sprawdzenie, czy linia zaczyna się od "opt="
    if (firstLine.rfind("opt=", 0) == 0) // sprawdza, czy "opt=" jest na początku
    {
        std::istringstream iss(firstLine.substr(4)); // Pomijamy "opt="
        iss >> optValue;
        if (iss.fail())
        {
            throw std::runtime_error("Nieprawidłowy format pliku: błąd odczytu wartości opt.");
        }
    }
    else
    {
        // Jeśli nie zaczyna się od "opt=", cofnij wskaźnik pliku
        file.clear();
        file.seekg(0);
    }

    // Odczyt macierzy sąsiedztwa
    std::vector<std::vector<int>> graph;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::vector<int> row;
        int value;

        while (iss >> value)
        {
            row.push_back(value);
        }

        if (!row.empty())
        {
            graph.push_back(row);
        }
    }



    file.close(); // Zamknięcie pliku
    return graph;
}


int FileController::readCost(const std::string& filename, bool testMode)
{
    // 1) Zbuduj ścieżkę do pliku
    std::string filePath = testMode ? "instances/" + filename : filename;

    // 2) Otwarcie pliku
    std::ifstream file(filePath);
    if (!file.is_open()) {
        // Rzucamy wyjątek, jeśli nie udało się otworzyć
        throw std::runtime_error("Nie można otworzyć pliku: " + filePath);
    }

    // 3) Przeszukujemy linie pliku
    std::string line;
    const std::string key = "opt=";

    while (std::getline(file, line))
    {

        // find() zwróci npos, jeśli nie znajdzie "sum_min="
        size_t pos = line.find(key);
        if (pos != std::string::npos) {
            // Znaleźliśmy fragment "sum_min="
            try {
                // Odczytujemy liczbę po "sum_min="
                // substring od (pos + key.size()) do końca linii
                int cost = std::stoi(line.substr(pos + key.size()));

                file.close();  // Zamykamy plik
                return cost;   // Zwracamy wczytaną wartość
            }
            catch (const std::invalid_argument&) {
                file.close();
                throw std::runtime_error("Nieprawidłowy format wartości po sum_min=");
            }
            catch (const std::out_of_range&) {
                file.close();
                throw std::runtime_error("Wartość sum_min= przekracza zakres typu int.");
            }
        }
    }

    // Jeśli nie znaleziono "sum_min=..." w pliku, zwracamy INT32_MAX
    file.close();
    return INT32_MAX;
}

#include "FileController.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

ConfigDataDto FileController::readConfigFile(const char* filename) {
    ConfigDataDto config;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku konfiguracyjnego: " + std::string(filename));
    }

    std::string line;
    while (std::getline(file, line)) {
        // Pomijamy puste linie i komentarze (# na początku linii)
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Znajdujemy pozycję znaku '='
        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) {
            // Jeżeli nie ma znaku '=', to format linii jest niepoprawny
            throw std::runtime_error("Nieprawidłowy format linii w pliku konfiguracyjnym: " + line);
        }

        // Rozdzielamy klucz i wartość
        std::string key   = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        // Usuwanie białych znaków na końcu klucza
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        // Usuwanie białych znaków z początku wartości
        value.erase(0, value.find_first_not_of(" \t\r\n"));

        // ---- Przypisywanie wartości na podstawie klucza ----
        if (key == "test_mode") {
            config.setTestMode(value == "true");
        }
        else if (key == "algorithm_type") {
            config.setAlgorithmType(value);
        }

        // --- Parametry dla ACO ---
        else if (key == "numOfAnts") {
            config.setNumOfAnts(std::stoi(value));
        }
        else if (key == "numOfIterations") {
            config.setNumOfIterations(std::stoi(value));
        }
        else if (key == "alpha") {
            config.setAlpha(std::stod(value)); // lub std::stof, zależnie od typu w DTO
        }
        else if (key == "beta") {
            config.setBeta(std::stod(value));
        }
        else if (key == "method") {
            // "cas" lub "das" – można przechowywać w stringu
            config.setMethod(value);
        }
        else if (key == "pheromoneQ") {
            config.setPheromoneQ(std::stod(value));
        }
        else if (key == "pheromoneEvaporation") {
            config.setPheromoneEvaporation(std::stod(value));
        }
        // --- Koniec parametrów ACO ---

        else if (key == "input_file") {
            config.setInputFile(value);
        }
        else if (key == "output_file") {
            config.setOutputFile(value);
        }
        else if (key == "name") {
            config.setName(value);
        }
        else if (key == "display_results") {
            config.setDisplayResults(value == "true");
        }
        else if (key == "show_progress") {
            config.setShowProgress(value == "true");
        }
        else {
            // Jeżeli pojawia się jakiś klucz, którego nie znamy, zgłaszamy wyjątek
            throw std::runtime_error("Nieznany klucz w pliku konfiguracyjnym: " + key);
        }
    }

    file.close();

    // Ewentualna weryfikacja, czy wczytano wszystkie wymagane parametry.
    // Można np. sprawdzić, czy numOfAnts > 0, czy plik wejściowy nie jest pusty itd.
    // Jeżeli brakuje jakiegoś kluczowego parametru, można ustawić domyślną wartość
    // lub rzucić wyjątek.

    return config;
}



// Funkcja do sprawdzania istnienia pliku
bool FileController::fileExists(const std::string &fileName) {
    std::ifstream file(fileName);
    return file.good();
}

void FileController::saveResultsToCSV(
    const std::string &outputFileName,
    const std::string &instanceName,
    int optimalCostFromFile,
    int foundCost,
    const std::vector<int> &foundPath,
    std::chrono::steady_clock::time_point startTime,
    std::chrono::steady_clock::time_point endTime)
{
    // Check if file already exists
    bool exists = fileExists(outputFileName);

    // Open file in append mode if it exists, or create a new one
    std::ofstream file(outputFileName, std::ios::app);

    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << outputFileName << std::endl;
        return;
    }

    // Write the header if the file is newly created
    if (!exists) {
        file << "Instance Name,Optimal Cost,Found Cost,Found Path,Execution Time (us),"
             << "Absolute Error,Relative Error (%),Percentage of Optimal (%)\n";
    }

    // Calculate execution time in milliseconds
    auto executionTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    // Calculate errors and percentage
    int absoluteError = std::abs(foundCost - optimalCostFromFile);
    double relativeError = (optimalCostFromFile != 0)
        ? (static_cast<double>(absoluteError) / optimalCostFromFile) * 100
        : 0.0;
    double percentageOfOptimal = (optimalCostFromFile != 0)
        ? (static_cast<double>(foundCost) / optimalCostFromFile) * 100
        : 0.0;

    // Format the found path as a string
    std::ostringstream foundPathStream;
    for (size_t i = 0; i < foundPath.size(); ++i) {
        foundPathStream << foundPath[i];
        if (i != foundPath.size() - 1) {
            foundPathStream << " -> ";
        }
    }

    // Write the results into the file
    file << std::fixed << std::setprecision(2);  // Set precision to 2 decimal places
    file << instanceName << ","                 // Instance name
         << optimalCostFromFile << ","          // Optimal cost
         << foundCost << ","                    // Found cost
         << "\"" << foundPathStream.str() << "\","  // Found path with quotes
         << executionTime << ","                // Execution time in us
         << absoluteError << ","                // Absolute error
         << relativeError << ","                // Relative error (%)
         << percentageOfOptimal                 // Percentage of optimal (%)
         << "\n";

    // Close the file
    file.close();
}

std::vector<std::string> FileController::getFileNamesFromDirectory() {
    // Katalog roboczy (ścieżka do folderu "instances")
    const std::string directoryPath = "./instances";
    std::vector<std::string> fileNames;

    // Otwieramy katalog
    DIR *dir = opendir(directoryPath.c_str());
    if (dir == nullptr) {
        throw std::runtime_error("Nie można otworzyć katalogu: " + directoryPath);
    }

    // Iterujemy przez pliki w katalogu
    struct dirent *ent;
    while ((ent = readdir(dir)) != nullptr) {
        // Pomijamy "." i ".."
        if (ent->d_name[0] == '.') {
            continue;
        }

        // Dodajemy pełną ścieżkę pliku do wyniku
        fileNames.push_back(ent->d_name);
    }

    // Zamykamy katalog
    closedir(dir);


    return fileNames;
}