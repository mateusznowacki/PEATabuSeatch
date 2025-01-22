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

    // Odczyt liczby wierzchołków
    int numberOfVertices;
    file >> numberOfVertices;
    if (file.fail())
    {
        throw std::runtime_error("Nieprawidłowy format pliku: brak liczby wierzchołków.");
    }

    // Odczyt macierzy sąsiedztwa
    std::vector<std::vector<int>> graph(numberOfVertices, std::vector<int>(numberOfVertices));
    for (int i = 0; i < numberOfVertices; ++i)
    {
        for (int j = 0; j < numberOfVertices; ++j)
        {
            file >> graph[i][j];
            if (file.fail())
            {
                throw std::runtime_error("Nieprawidłowy format pliku: błąd podczas odczytu macierzy.");
            }
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
    const std::string key = "sum_min=";

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

ConfigDataDto FileController::readConfigFile(const char* filename) {
    ConfigDataDto config;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku konfiguracyjnego: " + std::string(filename));
    }

    std::string line;
    while (std::getline(file, line)) {
        // Pomijanie pustych linii lub komentarzy
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Znalezienie pozycji separatora '='
        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) {
            throw std::runtime_error("Nieprawidłowy format linii w pliku konfiguracyjnym: " + line);
        }

        // Rozdzielenie klucza i wartości
        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        // Usuwanie białych znaków
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        value.erase(0, value.find_first_not_of(" \t\r\n"));

        // Konwersja wartości na małe litery, jeśli klucz wymaga (np. genNeighbour)
        if (key == "genNeighbour") {
            std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        }

        // Przypisanie wartości do odpowiednich pól za pomocą setterów
        if (key == "test_mode") {
            config.setTestMode(value == "true");
        } else if (key == "algorithm_type") {
            config.setAlgorithmType(value);
        } else if (key == "time_max") {
            config.setTimeMax(std::stoi(value));
        } else if (key == "dynamic_tabu_list") {
            config.setDynamicTabuList(value == "true");
        } else if (key == "tabu_list_size") {
            config.setTabuListSize(std::stoi(value));
        } else if (key == "max_iter_no_improve") {
            config.setMaxIterNoImprove(std::stoi(value));
        } else if (key == "procent_opt") {
            config.setProcentOpt(std::stod(value));
        } else if (key == "genNeighbour") {
            config.setGenNeighbour(value);  // Oczekiwane wartości: SWAP, INSERT
        } else if (key == "input_file") {
            config.setInputFile(value);
        } else if (key == "output_file") {
            config.setOutputFile(value);
        } else if (key == "name") {
            config.setName(value);
        } else if (key == "display_results") {
            config.setDisplayResults(value == "true");
        } else if (key == "show_progress") {
            config.setShowProgress(value == "true");
        } else {
            throw std::runtime_error("Nieznany klucz w pliku konfiguracyjnym: " + key);
        }
    }

    file.close();
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
    std::string directoryPath = "./instances"; // The `instances` directory assumed to be in the same directory as the executable
    std::vector<std::string> fileNames;
    DIR *dir;
    struct dirent *ent;

    // Open the directory
    if ((dir = opendir(directoryPath.c_str())) != nullptr) {
        // Iterate through all files and directories within the directory
        while ((ent = readdir(dir)) != nullptr) {
            // Skip "." and ".." directories
            if (ent->d_name[0] == '.') {
                continue;
            }

            // Add the file name with the path to the list
            fileNames.push_back(directoryPath + "/" + ent->d_name);
        }
        closedir(dir);
    } else {
        // Could not open the directory
        std::cerr << "Could not open directory: " << directoryPath << std::endl;
    }
    return fileNames;
}

