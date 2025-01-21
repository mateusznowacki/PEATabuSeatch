#include "Graph.h"


// Implementacja metod klasy Graph
Graph::Graph(int vertices) : vertices(vertices) {
    adjacencyMatrix.resize(vertices, std::vector<int>(vertices, 0));
}
// Graph::Graph() : vertices(0) {
//     // Default constructor does nothing, empty graph with no vertices
// }

void Graph::setEdge(int i, int j, int weight) {
    adjacencyMatrix[i][j] = weight;
}

int Graph::getEdge(int i, int j) const {
    return adjacencyMatrix[i][j];
}

int Graph::getVerticesCount() const
{
    return vertices;
}

int Graph::getVertices() const {
    return vertices;
}

void Graph::display() const {
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            std::cout << adjacencyMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
