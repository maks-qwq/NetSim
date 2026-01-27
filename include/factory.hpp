
#ifndef IMPLEMENTATION_INFINITY_HPP
#define IMPLEMENTATION_INFINITY_HPP


#include <map>
#include <memory>

#include "nodes.hpp"

template <typename Node>
class NodeCollection {
public:
    using container_t = std::map<ElementID, Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node);
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    iterator find_by_id(ElementID id);
    const_iterator find_by_id(ElementID id) const;

private:
    container_t nodes_;
};

class Factory {
public:
    NodeCollection<Ramp>& ramps();
    NodeCollection<Worker>& workers();
    NodeCollection<Storehouse>& storehouses();

    const NodeCollection<Ramp>& ramps() const;
    const NodeCollection<Worker>& workers() const;
    const NodeCollection<Storehouse>& storehouses() const;

private:
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
};

void load_factory_structure(const std::string& filename, Factory& f);
void save_factory_structure(const std::string& filename, const Factory& f);


#endif 