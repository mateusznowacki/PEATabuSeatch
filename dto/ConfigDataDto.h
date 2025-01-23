#ifndef CONFIGDATADTO_H
#define CONFIGDATADTO_H

#include <string>

class ConfigDataDto {
private:
    // --- Podstawowe pola ---
    bool test_mode;
    std::string algorithm_type;
    std::string input_file;
    std::string output_file;
    std::string name;
    bool display_results;
    bool show_progress;

    // --- Pola ACO ---
    int numOfAnts;
    int numOfIterations;
    double alpha;
    double beta;
    std::string method;
    double pheromoneQ;
    double pheromoneEvaporation;

public:
    // Konstruktor domyślny
    ConfigDataDto();

    // --- Settery ---
    void setTestMode(bool value);
    void setAlgorithmType(const std::string& val);
    void setInputFile(const std::string& val);
    void setOutputFile(const std::string& val);
    void setName(const std::string& val);
    void setDisplayResults(bool value);
    void setShowProgress(bool value);

    void setNumOfAnts(int val);
    void setNumOfIterations(int val);
    void setAlpha(double val);
    void setBeta(double val);
    void setMethod(const std::string& val);
    void setPheromoneQ(double val);
    void setPheromoneEvaporation(double val);

    // --- Gettery ---
    bool getTestMode() const;
    std::string getAlgorithmType() const;
    std::string getInputFile() const;
    std::string getOutputFile() const;
    std::string getName() const;
    bool getDisplayResults() const;
    bool getShowProgress() const;

    int getNumOfAnts() const;
    int getNumOfIterations() const;
    double getAlpha() const;
    double getBeta() const;
    std::string getMethod() const;
    double getPheromoneQ() const;
    double getPheromoneEvaporation() const;

    // Debug: Wyświetlenie wszystkich danych
    void printDebugInfo() const;
};

#endif // CONFIGDATADTO_H
