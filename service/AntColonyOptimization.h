#ifndef ANT_COLONY_OPTIMIZATION_H
#define ANT_COLONY_OPTIMIZATION_H

#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <iostream>

class AntColonyOptimization {
private:
    int n;                                     // Number of nodes in the graph
    float evaporationRate;                     // Pheromone evaporation rate
    int iterations;                            // Maximum number of iterations
    float alpha;                               // Influence of pheromone
    float beta;                                // Influence of heuristic (1/distance)
    float Q;                                   // Pheromone quantity deposited by each ant
    int type;                                  // CAS (0) or DAS (1)
    int opt;                                   // Optimal cost for termination condition
    int numOfAnts;                             // Number of ants

    std::vector<std::vector<float>> pheromones; // Pheromone matrix
    std::mt19937 gen;                          // Random number generator

    // Helper function to calculate probabilities for the next node
    std::vector<float> calculateProbabilities(
        const std::vector<std::vector<int>>& graph,
        int currentVertex,
        const std::vector<bool>& ifVisited
    );

    // Helper function to get the next node based on probabilities
    int getNextNode(const std::vector<float>& probabilities);

    // Helper function to update the pheromones
    void updatePheromones(
        const std::vector<int>& route,
        int weight,
        std::vector<std::vector<float>>& currentPheromones
    );

public:
    // Constructor
    AntColonyOptimization(
        int nodes,
        float evaporationRate,
        int iterations,
        float alpha,
        float beta,
        int type,
        float Q,
        int opt
    );

    // Solve the Traveling Salesman Problem using ACO
    // (all parameters are passed in the constructor; only graph is needed here)
    std::vector<int> solve(const std::vector<std::vector<int>>& graph);

    // Getters
    int getNumberOfNodes()  { return n; }
    float getEvaporationRate()  { return evaporationRate; }
    int getIterations()  { return iterations; }
    float getAlpha()  { return alpha; }
    float getBeta()  { return beta; }
    float getPheromoneQ()  { return Q; }
    int getType()  { return type; }
    int getOptimalCost()  { return opt; }
    int getNumberOfAnts()  { return numOfAnts; }

    // Setters
    void setNumberOfNodes(int nodes) { n = nodes; }
    void setEvaporationRate(float rate) { evaporationRate = rate; }
    void setIterations(int iter) { iterations = iter; }
    void setAlpha(float a) { alpha = a; }
    void setBeta(float b) { beta = b; }
    void setPheromoneQ(float q) { Q = q; }
    void setType(int t) { type = t; }
    void setOptimalCost(int optimalCost) { opt = optimalCost; }
    void setNumberOfAnts(int ants) { numOfAnts = ants; }

};

#endif // ANT_COLONY_OPTIMIZATION_H
