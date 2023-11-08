#pragma once
#include "node.h"
#include <string>

class Graph {
public:
    Graph(int numNodes = 0);

    int size() const { return m_size; }
    bool hasNode(int id) const;
    bool addNode(int id);
    bool addEdge(int i, int j);
    void removeEdge(int i, int j);
    void removeNode(int id);
    const Node &operator[](int i) const { return m_nodes[i]; }
    Node &operator[](int i) { return m_nodes[i]; }
    bool operator==(const Graph &other) const;

    void fromJson(std::string fileName);
    void toJson(std::string fileName);
    
    //void buildDegreeVector();
    //int degree(int i) const { return m_degree[i]; }

    //friend void detectAllIsolatedComponents(Graph &);

private:
    int m_size;

    std::vector<Node> m_nodes;
    //std::vector<int> m_degree;

    //// Set to -1 by default. Can be recalculated with the algorithms from cluster.h
    //std::vector<int> m_componentID;

    //// Undefined by default. Can be recalculated with the algorithms from cluster.h
    //std::vector<int> m_componentSize;

    //std::vector<int> m_queue;

    //int m_numComponents = 0;
    //int m_gccSize = 0;
    //int m_gccRoot = 0;
};
