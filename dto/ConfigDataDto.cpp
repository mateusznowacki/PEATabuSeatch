#include "ConfigDataDto.h"

#include <ios>
#include <iostream>
#include <string>

// Konstruktor domyślny
ConfigDataDto::ConfigDataDto()
    : test_mode(false),
      algorithm_type(""),
      time_max(0),
      dynamic_tabu_list(false),
      tabu_list_size(0),
      max_iter_no_improve(0),
      procent_opt(0.0),
      genNeighbour(""),
      input_file(""),
      output_file(""),
      name(""),
      display_results(false),
      show_progress(false)
{
}

// Settery
void ConfigDataDto::setTestMode(bool value)
{
    test_mode = value;
}

void ConfigDataDto::setAlgorithmType(const std::string& value)
{
    algorithm_type = value;
}

void ConfigDataDto::setTimeMax(int value)
{
    time_max = value;
}

void ConfigDataDto::setDynamicTabuList(bool value)
{
    dynamic_tabu_list = value;
}

void ConfigDataDto::setTabuListSize(int value)
{
    tabu_list_size = value;
}

void ConfigDataDto::setMaxIterNoImprove(int value)
{
    max_iter_no_improve = value;
}

void ConfigDataDto::setProcentOpt(double value)
{
    procent_opt = value;
}

void ConfigDataDto::setGenNeighbour(const std::string& value)
{
    genNeighbour = value;
}

void ConfigDataDto::setInputFile(const std::string& value)
{
    input_file = value;
}

void ConfigDataDto::setOutputFile(const std::string& value)
{
    output_file = value;
}

void ConfigDataDto::setName(const std::string& value)
{
    name = value;
}

void ConfigDataDto::setDisplayResults(bool value)
{
    display_results = value;
}

void ConfigDataDto::setShowProgress(bool value)
{
    show_progress = value;
}

// Gettery
bool ConfigDataDto::getTestMode() const
{
    return test_mode;
}

std::string ConfigDataDto::getAlgorithmType() const
{
    return algorithm_type;
}

int ConfigDataDto::getTimeMax() const
{
    return time_max;
}

bool ConfigDataDto::getDynamicTabuList() const
{
    return dynamic_tabu_list;
}

int ConfigDataDto::getTabuListSize() const
{
    return tabu_list_size;
}

int ConfigDataDto::getMaxIterNoImprove() const
{
    return max_iter_no_improve;
}

double ConfigDataDto::getProcentOpt() const
{
    return procent_opt;
}

std::string ConfigDataDto::getGenNeighbour() const
{
    return genNeighbour;
}

std::string ConfigDataDto::getInputFile() const
{
    return input_file;
}

std::string ConfigDataDto::getOutputFile() const
{
    return output_file;
}

std::string ConfigDataDto::getName() const
{
    return name;
}

bool ConfigDataDto::getDisplayResults() const
{
    return display_results;
}

bool ConfigDataDto::getShowProgress() const
{
    return show_progress;
}


// Metoda debug
void ConfigDataDto::debug()
{
    std::cout << "Test Mode: " << std::boolalpha << test_mode << "\n"
        << "Algorithm Type: " << algorithm_type << "\n"
        << "Time Max: " << time_max << "\n"
        << "Dynamic Tabu List: " << std::boolalpha << dynamic_tabu_list << "\n"
        << "Tabu List Size: " << tabu_list_size << "\n"
        << "Max Iter No Improve: " << max_iter_no_improve << "\n"
        << "Procent Opt: " << procent_opt << "\n"
        << "Gen Neighbour: " << genNeighbour << "\n"
        << "Input File: " << input_file << "\n"
        << "Output File: " << output_file << "\n"
        << "Name: " << name << "\n"
        << "Display Results: " << std::boolalpha << display_results << "\n"
        << "Show Progress: " << std::boolalpha << show_progress << "\n";
}
