#include "ConfigDataDto.h"
#include <iostream>

// Konstruktor domyślny
ConfigDataDto::ConfigDataDto()

{
}

// ----- Settery -----
void ConfigDataDto::setTestMode(bool value)              { test_mode = value; }
void ConfigDataDto::setAlgorithmType(const std::string& val) { algorithm_type = val; }
void ConfigDataDto::setInputFile(const std::string& val) { input_file = val; }
void ConfigDataDto::setOutputFile(const std::string& val){ output_file = val; }
void ConfigDataDto::setName(const std::string& val)      { name = val; }
void ConfigDataDto::setDisplayResults(bool value)        { display_results = value; }
void ConfigDataDto::setShowProgress(bool value)          { show_progress = value; }

void ConfigDataDto::setNumOfAnts(int val)                { numOfAnts = val; }
void ConfigDataDto::setNumOfIterations(int val)          { numOfIterations = val; }
void ConfigDataDto::setAlpha(double val)                 { alpha = val; }
void ConfigDataDto::setBeta(double val)                  { beta = val; }
void ConfigDataDto::setMethod(const std::string& val)    { method = val; }
void ConfigDataDto::setPheromoneQ(double val)            { pheromoneQ = val; }
void ConfigDataDto::setPheromoneEvaporation(double val)  { pheromoneEvaporation = val; }

// ----- Gettery -----
bool ConfigDataDto::getTestMode() const                  { return test_mode; }
std::string ConfigDataDto::getAlgorithmType() const      { return algorithm_type; }
std::string ConfigDataDto::getInputFile() const          { return input_file; }
std::string ConfigDataDto::getOutputFile() const         { return output_file; }
std::string ConfigDataDto::getName() const               { return name; }
bool ConfigDataDto::getDisplayResults() const            { return display_results; }
bool ConfigDataDto::getShowProgress() const              { return show_progress; }

int ConfigDataDto::getNumOfAnts() const                  { return numOfAnts; }
int ConfigDataDto::getNumOfIterations() const            { return numOfIterations; }
double ConfigDataDto::getAlpha() const                   { return alpha; }
double ConfigDataDto::getBeta() const                    { return beta; }
std::string ConfigDataDto::getMethod() const             { return method; }
double ConfigDataDto::getPheromoneQ() const              { return pheromoneQ; }
double ConfigDataDto::getPheromoneEvaporation() const    { return pheromoneEvaporation; }

// Debug
void ConfigDataDto::printDebugInfo() const {
    std::cout << "=== Konfiguracja (ACO) ===" << std::endl;
    std::cout << "test_mode: " << (test_mode ? "true" : "false") << std::endl;
    std::cout << "algorithm_type: " << algorithm_type << std::endl;
    std::cout << "input_file: " << input_file << std::endl;
    std::cout << "output_file: " << output_file << std::endl;
    std::cout << "name: " << name << std::endl;
    std::cout << "display_results: " << (display_results ? "true" : "false") << std::endl;
    std::cout << "show_progress: " << (show_progress ? "true" : "false") << std::endl;
    std::cout << "\n--- ACO ---" << std::endl;
    std::cout << "numOfAnts: " << numOfAnts << std::endl;
    std::cout << "numOfIterations: " << numOfIterations << std::endl;
    std::cout << "alpha: " << alpha << std::endl;
    std::cout << "beta: " << beta << std::endl;
    std::cout << "method: " << method << std::endl;
    std::cout << "pheromoneQ: " << pheromoneQ << std::endl;
    std::cout << "pheromoneEvaporation: " << pheromoneEvaporation << std::endl;
    std::cout << "==========================" << std::endl;
}
