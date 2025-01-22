#include "FileController.h"
#include <chrono>
#include <fstream>
#include <string>
#include <vector>

#include <stdexcept>
#include <regex>



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
    // Sprawdzenie, czy plik już istnieje
    bool exists = fileExists(outputFileName);

    // Otwórz plik w trybie dołączania, jeśli istnieje; w trybie zapisu, jeśli nie istnieje
    std::ofstream file(outputFileName, std::ios::app);

    // Sprawdzenie, czy plik został poprawnie otwarty
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << outputFileName << std::endl;
        return;
    }

    // Zapis nagłówka, jeśli plik nie istnieje (tworzony po raz pierwszy)
    if (!exists) {
        file << "Nazwa instancji,Optymalny koszt,Optymalna ścieżka,"
             << "Znaleziony koszt,Znaleziona ścieżka,Czas wykonania (ms),"
             << "Błąd bezwzględny,Błąd względny (%),"
             << "Procent rozwiazania optymalnego (%)\n";
    }

    // Obliczanie czasu wykonania w milisekundach
    auto executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Obliczanie błędów
    int absoluteError = std::abs(foundCost - optimalCostFromFile);
    double relativeError = (static_cast<double>(absoluteError) / optimalCostFromFile) * 100;
    double percentageOfOptimal = (static_cast<double>(foundCost) / optimalCostFromFile) * 100;

    std::ostringstream foundPathStream;
    for (size_t i = 0; i < foundPath.size(); ++i) {
        foundPathStream << foundPath[i];
        if (i != foundPath.size() - 1) {
            foundPathStream << " -> ";
        }
    }

    file << std::fixed << std::setprecision(10);

    // Zapis danych w nowym wierszu
    file << instanceName << ","
         << optimalCostFromFile << ",\""
         << foundCost << ",\""
         << foundPathStream.str() << "\","
         << executionTime << ","
         << absoluteError << ","
         << relativeError << ","
         << percentageOfOptimal << "\n";

    // Zamknięcie pliku
    file.close();
}

