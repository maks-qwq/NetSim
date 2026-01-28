#include "factory.hpp"

template <typename Node>
void NodeCollection<Node>::remove_by_id(ElementID id) {
    auto it = find_by_id(id);
    if (it != end()) {
        nodes_.erase(it);
    }
}

void Factory::do_deliveries(Time t) {
    for (auto &ramp : ramps_)
        ramp.deliver_goods(t);
}