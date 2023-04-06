#include "algorithms.h"
#include <algorithm> // std::min
#include <stdexcept> // std::runtime_error
#include <string>
#include <utility> // std::pair
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

// ap = AP = articulation points
// p = parent
// disc[u] = discovery time of u
// low[u] = 'low' node of u
int dfsAP(
    const Graph &G, int u, int p,
    std::vector<int> &low, std::vector<int> &disc, std::vector<int> &ap,
    int &time
) {
    int children = 0;
    low[u] = disc[u] = ++time;
    for (const int &v : G[u].neighbors_) {
        // We don't want to go back through the same path.
        // If we go back is because we found another way back
        if (v == p || !G[v].present_) continue;
        if (!disc[v]) { // if V has not been discovered before
            ++children;
            dfsAP(G, v, u, low, disc, ap, time); // recursive DFS call
            if (disc[u] <= low[v])
                ap[u] = 1;
            low[u] = std::min(low[u], low[v]); // low[v] might be an ancestor of u
        }
        else // if v was already discovered means that we found an ancestor
            low[u] = std::min(low[u], disc[v]); // finds the ancestor with the least discovery time
    }
    return children;
}

std::vector<int> articulationPoints(const Graph &G) {
    std::vector<int> ap(G.size());
    std::vector<int> low(G.size());
    std::vector<int> disc(G.size());
    int time = 0;
    for (int u = 0; u < G.size(); ++u) {
        if (!disc[u] && G[u].present_) {
            ap[u] = dfsAP(G, u, u, low, disc, ap, time) > 1;
        }
    }
    std::vector<int> res;
    for (std::size_t i = 0; i < ap.size(); ++i) {
        if (ap[i]) {
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
    
    cascadeRemoveLinks(G1, G2);

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
