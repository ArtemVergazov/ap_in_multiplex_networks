#include "graph.h"
#include "json.hpp"
#include <algorithm> // std::find_if
#include <fstream> // std::ofstream
#include <sstream> // std::istringstream
#include <iostream>

Graph::Graph(int numNodes) :
    //m_degree(numNodes), m_componentID(numNodes, -1), m_queue(numNodes),
    m_size(numNodes) {

    for (int i = 0; i < numNodes; ++i) {
        m_nodes.emplace_back(i);
    }

}

void Graph::fromJson(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;

    while (std::getline(file, line)) {
        int i, j;
        std::istringstream(line) >> i >> j;
        addEdge(i, j);
    }
}

bool Graph::hasNode(int id) const {
    for (auto node = m_nodes.begin(); node != m_nodes.end(); ++node) {
        if (node->id_ == id && node->present_) {
            return true;
        }
    }
    return false;
}

//bool Graph::addNode(int id) {
//    if (hasNode(id)) {
//        return false;
//    }
//    m_nodes.emplace_back(id);
//    return true;
//}

// Assuming i and j exist
bool Graph::addEdge(int i, int j) {

    //addNode(i);
    //addNode(j);

    if (!m_nodes[i].has(j) && i != j) {
        m_nodes[i].neighbors_.push_back(j);
        m_nodes[j].neighbors_.push_back(i);
        return true;
    }
    return false;
}

void Graph::removeEdge(int i, int j) {
    m_nodes[i].remove(j);
    m_nodes[j].remove(i);
}

void Graph::removeNode(int id) {
    for (int neighbor : m_nodes[id].neighbors_) {
        m_nodes[neighbor].remove(id);
    }
    m_nodes[id].present_ = false;
}

//void Graph::buildDegreeVector() {
//
//    for (int i = 0; i < size(); ++i) {
//        m_degree[i] = m_nodes[i].degree();
//    }
//
//}

bool Graph::operator==(const Graph &other) const {
    
    if (size() != other.size()) return false;

    for (int i = 0; i < size(); ++i) {
        if (m_nodes[i] != other[i]) {
            return false;
        }
    }
    return true;
}

void Graph::toJson(std::string fileName) {

    using json = nlohmann::json;
    json output;
    
    for (int i = 0; i < size(); ++i) {
        if (m_nodes[i].present_) {
            output[std::to_string(i)] = m_nodes[i].neighbors_;
        }
    }

    std::ofstream o(fileName);
    o << std::setw(4) << output << std::endl;

}
