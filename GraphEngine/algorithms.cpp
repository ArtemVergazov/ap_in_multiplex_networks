#include "algorithms.h"
#include <algorithm> // std::min
#include <stdexcept> // std::runtime_error
#include <string>
#include <cstddef> // std::size_t

// A fast BFS node generator
std::set<int> plainBfs(const Graph &G, int source) {
        
    std::set<int> seen{};
    std::set<int> nextlevel{ source };
    while (nextlevel.size()) {
        std::set<int> thislevel = nextlevel;
        nextlevel.clear();
        for (auto v : thislevel) {
            if (G[v].present_ && seen.find(v) == seen.end()) {
                seen.insert(v);
                for (auto neighb : G[v].neighbors_) {
                    nextlevel.insert(neighb);
                }
            }
        }
    }
    return seen;
}

std::vector<std::set<int>> connectedComponents(const Graph &G) {

    std::vector<std::set<int>> res{};
    std::set<int> seen{};
    for (int i = 0; i < G.size(); ++i) {
        if (G[i].present_ && seen.find(i) == seen.end()) {
            auto c = plainBfs(G, i);
            for (auto ci : c) {
                seen.insert(ci);
            }
            res.push_back(c);
        }
    }
    return res;
}

#pragma comment(linker, "/STACK:100000000000000")
void findArticulationPointsRecursively(
    const Graph &network, int node, int &time, std::vector<int> &parent,
    std::vector<int> &lowValue, std::vector <int> &discoveryTime,
    std::vector<bool> &visited, std::vector <bool> &articulationPoints
) {

    int children = 0;
    visited[node] = true;
    discoveryTime[node] = lowValue[node] = ++time;

    for (int i = 0; i < network[node].degree(); ++i) {

        int neighbor = network[node][i];

        if (network[neighbor].present_) {

            if (false == visited[neighbor]) {

                ++children;
                parent[neighbor] = node;

                findArticulationPointsRecursively(
                    network, neighbor, time, parent,
                    lowValue, discoveryTime,
                    visited, articulationPoints
                );

                lowValue[node] = std::min(lowValue[node], lowValue[neighbor]);


                if (-1 == parent[node] && children > 1)
                    articulationPoints[node] = true;

                if (-1 != parent[node] && lowValue[neighbor] >= discoveryTime[node])
                    articulationPoints[node] = true;

            }
            else if (neighbor != parent[node]) {
                lowValue[node] = std::min(lowValue[node], discoveryTime[neighbor]);
            }
        }
    }
}

std::vector<int> articulationPoints(const Graph &G) {
    
    int time;

    std::vector<bool> visited(G.size(), false);
    std::vector<bool> artPoints(G.size(), false);
    std::vector<int> parent(G.size(), -1);
    std::vector<int> lowValue(G.size());
    std::vector<int> discoveryTime(G.size());

    for (int i = 0; i < G.size(); ++i) {
        if (false == visited[i] && G[i].present_) {

            time = 0;
            findArticulationPointsRecursively(
                G, i, time, parent,
                lowValue, discoveryTime,
                visited, artPoints
            );
        }
    }

    std::vector<int> res;
    for (int i = 0; i < G.size(); ++i) {
        if (artPoints[i]) {
            res.push_back(i);
        }
    }

    return res;
}

std::set<int> articulationPointsMultiplex(const Graph &G1, const Graph &G2) {
    auto ap1 = articulationPoints(G1);
    auto ap2 = articulationPoints(G2);

    std::set<int> res(ap1.begin(), ap1.end());
    res.insert(ap2.begin(), ap2.end());
    return res;
}

int findComponent(int node, const std::vector<std::set<int>> &connectedComponents) {
    for (std::size_t i = 0; i < connectedComponents.size(); ++i) {
        if (connectedComponents[i].find(node) != connectedComponents[i].end()) {
            return i;
        }
    }
    std::string msg = "Component for node " + std::to_string(node) + " not found";
    throw std::runtime_error(msg);
}

void cascadeRemoveLinks(Graph &Ga, Graph &Gb) {

    bool keepGoing = true;
    while (keepGoing) {

        keepGoing = false;

        auto clustersA = connectedComponents(Ga);
        for (int i = 0; i < Gb.size(); ++i) {
            if (!Gb[i].present_) continue;
            for (int j : Gb[i].neighbors_) {
                if (!Gb[j].present_) continue;
                if (findComponent(i, clustersA) != findComponent(j, clustersA)) {
                    keepGoing = true;
                    Gb.removeEdge(i, j);
                }
            }
        }

        auto clustersB = connectedComponents(Gb);
        for (int i = 0; i < Ga.size(); ++i) {
            if (!Ga[i].present_) continue;
            for (int j : Ga[i].neighbors_) {
                if (!Ga[j].present_) continue;
                if (findComponent(i, clustersB) != findComponent(j, clustersB)) {
                    keepGoing = true;
                    Ga.removeEdge(i, j);
                }
            }
        }
    }
}

int maxComponentSize(const Graph &G) {
    auto comps = connectedComponents(G);
    if (comps.empty()) return 0;
    int max = comps[0].size();
    for (std::size_t i = 1; i < comps.size(); ++i) {
        int size = comps[i].size();
        if (size > max) {
            max = size;
        }
    }
    return max;
}

std::pair<std::set<int>, int> removeArticulationPoints(Graph &G1, Graph &G2) {
    
    //cascadeRemoveLinks(G1, G2);

    auto artPoints = articulationPointsMultiplex(G1, G2);
    int gccSize = maxComponentSize(G1);

    for (int i : artPoints) {
        if (G1[i].present_) {
            G1.removeNode(i);
        }
        if (G2[i].present_) {
            G2.removeNode(i);
        }
    }

    return std::make_pair(artPoints, gccSize);
}
