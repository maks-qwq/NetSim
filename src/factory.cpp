#include "factory.hpp"

/* ===== NodeCollection implementation ===== */

template <typename Node>
void NodeCollection<Node>::add(Node&& node) {
    nodes_.emplace(node.get_id(), std::move(node));
}

template <typename Node>
typename NodeCollection<Node>::iterator
NodeCollection<Node>::begin() {
    return nodes_.begin();
}

template <typename Node>
typename NodeCollection<Node>::iterator
NodeCollection<Node>::end() {
    return nodes_.end();
}

template <typename Node>
typename NodeCollection<Node>::const_iterator
NodeCollection<Node>::begin() const {
    return nodes_.begin();
}

template <typename Node>
typename NodeCollection<Node>::const_iterator
NodeCollection<Node>::end() const {
    return nodes_.end();
}

template <typename Node>
typename NodeCollection<Node>::iterator
NodeCollection<Node>::find_by_id(ElementID id) {
    return nodes_.find(id);
}

template <typename Node>
typename NodeCollection<Node>::const_iterator
NodeCollection<Node>::find_by_id(ElementID id) const {
    return nodes_.find(id);
}

/* ===== explicit template instantiation ===== */

template class NodeCollection<Ramp>;
template class NodeCollection<Worker>;
template class NodeCollection<Storehouse>;
