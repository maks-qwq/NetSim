#pragma once

#include <map>
#include <memory>
#include <algorithm>

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
    bool is_consistent(); //sprawdzanie spójności sieci
    void do_deliveries(); //dokonywanie ewentualnych dostaw na rampy
    void do_package_passing(); //dokonywanie ewentualnego przekazywania półproduktów
    void do_work(); //dokonywanie ewentualnego przetwarzania półproduktów przez robotników

    void add_ramp(Ramp&& r) { ramps_.add(std::move(r)); }
    void remove_ramp(ElementID id) { ramps_.remove_by_id(id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramps_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramps_.cend(); }

    void add_ramp(Worker&& r) { workers_.add(std::move(r)); }
    void remove_ramp(ElementID id) { workers_.remove_by_id(id); }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return workers_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return workers_.cend(); }

    void add_ramp(Storehouse&& r) { storehouses_.add(std::move(r)); }
    void remove_ramp(ElementID id) { storehouses_.remove_by_id(id); }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouses_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouses_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouses_.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouses_.cend(); }



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
    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id); //pomocnicza metoda do usuwania odbiorcy 
};

void load_factory_structure(const std::string& filename, Factory& f);
void save_factory_structure(const std::string& filename, const Factory& f);
