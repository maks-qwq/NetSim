#pragma once

#include <map>
#include <memory>

#include "nodes.hpp"
#include "storage_types.hpp"
#include <bits/stdc++.h>

enum class NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

template <typename Node>
class NodeCollection {
public:
    using container_t = std::map<ElementID, Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node) { nodes_.emplace_back(std::move(node)); } //dodanie nowego połączenia

    void remove_by_id(ElementID id); //usuwa z kontenera po id

    iterator begin() {return nodes_.begin();}
    iterator end() {return nodes_.end();}
    const_iterator begin() const {return nodes_.cbegin();}
    const_iterator end() const {return nodes_.cend();}
    const_iterator begin() const {return nodes_.cbegin();}
    const_iterator end() const {return nodes_.cend();}

    iterator find_by_id(ElementID id); //szuka elementu po id
    const_iterator find_by_id(ElementID id) const;

private:
    container_t nodes_;
};

class Factory {
public:
    bool is_consistent(); //sprawdzanie spójności sieci
    void do_deliveries(); //dokonywanie ewentualnych dostaw na rampy
    void do_package_passing(); //dokonywanie ewentualnego przekazywania półproduktów
    void do_work(); //dokonywanie ewentualnego przetwarzania półproduktów przez robotników

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
    void remove_receiver(NodeCollection<Node>& collection, ElementID id); //pomocnicza metoda do usuwania odbiorcy 
};

void load_factory_structure(const std::string& filename, Factory& f);
void save_factory_structure(const std::string& filename, const Factory& f);
