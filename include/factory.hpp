#pragma once

#include <map>
#include <memory>
#include <algorithm>
#include <istream>
#include <optional>

#include "nodes.hpp"
#include "storage_types.hpp"

enum class NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

enum class ElementType {
    RAMP, WORKER, STOREHOUSE
};

template <typename Node>
class NodeCollection {
public:
    using container_t = std::map<ElementID, Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node) { nodes_.emplace_back(std::move(node)); } //dodanie nowego połączenia

    iterator begin() {return nodes_.begin();}
    iterator end() {return nodes_.end();}
    const_iterator begin() const {return nodes_.cbegin();}
    const_iterator end() const {return nodes_.cend();}
    const_iterator cbegin() const {return nodes_.cbegin();}
    const_iterator cend() const {return nodes_.cend();}

    NodeCollection<Node>::iterator find_by_id(ElementID id) {
        return std::find_if(begin(), end(), [id](const Node& pack) {
            return pack.get_id() == id;
        });
    }
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {
        return std::find_if(begin(), end(), [id](const Node& pack) {
            return pack.get_id() == id;
        });
    }
    void remove_by_id(ElementID id); //usuwa z kontenera po id
private:
    container_t nodes_;
};


class Factory {
public:
    // === dodawanie elementów ===
    void add_ramp(Ramp&& r);
    void add_worker(Worker&& w);
    void add_storehouse(Storehouse&& s);
    
    // === usuwanie elementów ===
    void remove_worker(ElementID id);
    
    // === wyszukiwanie po ID ===
    Ramp* find_ramp_by_id(ElementID id);
    Worker* find_worker_by_id(ElementID id);
    Storehouse* find_storehouse_by_id(ElementID id);

    // === iteratory ===
    using ramp_iterator = std::list<Ramp>::const_iterator;
    using worker_iterator = std::list<Worker>::const_iterator;
    using storehouse_iterator = std::list<Storehouse>::const_iterator;

    ramp_iterator ramp_cbegin() const;
    ramp_iterator ramp_cend() const;

    worker_iterator worker_cbegin() const;
    worker_iterator worker_cend() const;

    storehouse_iterator storehouse_cbegin() const;
    storehouse_iterator storehouse_cend() const;

    // === spójność fabryki ===
    bool is_consistent() const;

private:
    std::list<Ramp> ramps_;
    std::list<Worker> workers_;
    std::list<Storehouse> storehouses_;

    bool has_reachable_storehouse(const PackageSender* sender,
                                  std::list<const PackageSender*>& visited) const;
};

Factory load_factory_structure(std::istream& is);
void save_factory_structure(const Factory& f, std::ostream& os);

void load_factory_structure(const std::string& filename, Factory& f);
void save_factory_structure(const std::string& filename, const Factory& f);
