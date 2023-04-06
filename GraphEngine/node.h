#pragma once
#include <vector>

struct Node {
    int id_;
    bool present_ = true;
    std::vector<int> neighbors_;

    Node(int id) : id_(id) {}
    int operator[](int i) const { return neighbors_[i]; }
    int &operator[](int i) { return neighbors_[i]; }
    bool has(int id) const;
    int degree() const { return neighbors_.size(); }
    void remove(int id);
};
