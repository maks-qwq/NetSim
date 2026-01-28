#include "factory.hpp"

template <typename Node>
typename NodeCollection<Node>::iterator 
NodeCollection<Node>::find_by_id(ElementID id){
    return std::find_if(begin(), end(), [id](const auto& package) {
        return package.get_id() == id;});
}

template <typename Node>
typename NodeCollection<Node>::iterator 
NodeCollection<Node>::find_by_id(ElementID id) const {
    return std::find_if(begin(), end(), [id](const auto& package) {
        return package.get_id() == id;});
}

void NodeCollection<Node>::remove_by_id(ElementID id){
    auto it = find_by_id(id);
    if (it != end()) {
        nodes_.erase(it);
    }
}