#ifndef CONFIGDATADTO_H
#define CONFIGDATADTO_H
#include <iostream>
#include <string>


class ConfigDataDto {
private:
    bool test_mode;
    std::string algorithm_type;
    int time_max;
    bool dynamic_tabu_list;
    int tabu_list_size;
    int max_iter_no_improve;
    double procent_opt;
    std::string genNeighbour;
    std::string input_file;
    std::string output_file;
    std::string name;
    bool display_results;
    bool show_progress;

public:
    // Konstruktor domyślny
    ConfigDataDto();

    // Settery
    void setTestMode(bool value);
    void setAlgorithmType(const std::string& value);
    void setTimeMax(int value);
    void setDynamicTabuList(bool value);
    void setTabuListSize(int value);
    void setMaxIterNoImprove(int value);
    void setProcentOpt(double value);
    void setGenNeighbour(const std::string& value);
    void setInputFile(const std::string& value);
    void setOutputFile(const std::string& value);
    void setName(const std::string& value);
    void setDisplayResults(bool value);
    void setShowProgress(bool value);

    // Gettery
    bool getTestMode() const;
    std::string getAlgorithmType() const;
    int getTimeMax() const;
    bool getDynamicTabuList() const;
    int getTabuListSize() const;
    int getMaxIterNoImprove() const;
    double getProcentOpt() const;
    std::string getGenNeighbour() const;
    std::string getInputFile() const;
    std::string getOutputFile() const;
    std::string getName() const;
    bool getDisplayResults() const;
    bool getShowProgress() const;

    // Debug: Wyświetlenie wszystkich danych
    void printDebugInfo() const {
        std::cout << "=== ConfigDataDto Debug Info ===" << std::endl;
        std::cout << "Test mode: " << (test_mode ? "true" : "false") << std::endl;
        std::cout << "Algorithm type: " << algorithm_type << std::endl;
        std::cout << "Time max: " << time_max << std::endl;
        std::cout << "Dynamic tabu list: " << (dynamic_tabu_list ? "true" : "false") << std::endl;
        std::cout << "Tabu list size: " << tabu_list_size << std::endl;
        std::cout << "Max iter no improve: " << max_iter_no_improve << std::endl;
        std::cout << "Procent opt: " << procent_opt << std::endl;
        std::cout << "Gen neighbour: " << genNeighbour << std::endl;
        std::cout << "Input file: " << input_file << std::endl;
        std::cout << "Output file: " << output_file << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Display results: " << (display_results ? "true" : "false") << std::endl;
        std::cout << "Show progress: " << (show_progress ? "true" : "false") << std::endl;
        std::cout << "=================================" << std::endl;
    }


    void debug();
};

#endif //CONFIGDATADTO_H
