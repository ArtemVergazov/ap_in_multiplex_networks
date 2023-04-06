#include "graph.h"
#include <algorithm> // std::find_if

Graph::Graph(int numNodes) :
    //m_degree(numNodes), m_componentID(numNodes, -1), m_queue(numNodes),
    m_size(numNodes) {

    for (int i = 0; i < numNodes; ++i) {
        m_nodes.emplace_back(i);
    }

}

// Degree vector is in inconsistent state after this.
// It is the user's responsibility to rebuild the degree vector afterwards.
bool Graph::addEdge(int i, int j) {

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
