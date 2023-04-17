#include "node.h"
#include <algorithm> // std::find_if, std::remove_if

bool Node::has(int id) const {
    if (!present_) return false;
    return std::find_if(
        neighbors_.begin(), neighbors_.end(),
        [id](int id1) { return id == id1; }
    ) != neighbors_.end();
}

void Node::remove(int id) {
    neighbors_.erase(std::remove_if(
        neighbors_.begin(), neighbors_.end(),
        [id](int id1) { return id == id1; }
    ), neighbors_.end());
}

bool Node::operator==(const Node &other) const {
    
    if (id_ != other.id_) return false;
    if (present_ != other.present_) return false;
    if (neighbors_.size() != other.neighbors_.size()) return false;

    for (int i = 0; i < neighbors_.size(); ++i) {
        if (neighbors_[i] != other.neighbors_[i]) {
            return false;
        }
    }
    return true;
}
