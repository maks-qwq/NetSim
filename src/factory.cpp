#include "factory.hpp"
#include <istream>

#include <sstream>
#include <string>
#include <algorithm>

/* ===== Dodawanie ===== */

void Factory::add_ramp(Ramp&& r) {
    ramps_.push_back(std::move(r));
}

void Factory::add_worker(Worker&& w) {
    workers_.push_back(std::move(w));
}

void Factory::add_storehouse(Storehouse&& s) {
    storehouses_.push_back(std::move(s));
}

/* ===== Wyszukiwanie ===== */

Ramp* Factory::find_ramp_by_id(ElementID id) {
    for (auto& r : ramps_)
        if (r.get_id() == id)
            return &r;
    return nullptr;
}

Worker* Factory::find_worker_by_id(ElementID id) {
    for (auto& w : workers_)
        if (w.get_id() == id)
            return &w;
    return nullptr;
}

Storehouse* Factory::find_storehouse_by_id(ElementID id) {
    for (auto& s : storehouses_)
        if (s.get_id() == id)
            return &s;
    return nullptr;
}

/* ===== Iteratory ===== */

Factory::ramp_iterator Factory::ramp_cbegin() const {
    return ramps_.cbegin();
}

Factory::ramp_iterator Factory::ramp_cend() const {
    return ramps_.cend();
}

Factory::worker_iterator Factory::worker_cbegin() const {
    return workers_.cbegin();
}

Factory::worker_iterator Factory::worker_cend() const {
    return workers_.cend();
}

Factory::storehouse_iterator Factory::storehouse_cbegin() const {
    return storehouses_.cbegin();
}

Factory::storehouse_iterator Factory::storehouse_cend() const {
    return storehouses_.cend();
}

/* ===== Spójność ===== */

bool Factory::is_consistent() const {
    for (const auto& r : ramps_) {
        std::list<const PackageSender*> visited;
        if (!has_reachable_storehouse(&r, visited))
            return false;
    }
    
    for (const auto& w : workers_) {
        std::list<const PackageSender*> visited;
        if (!has_reachable_storehouse(&w, visited))
            return false;
    }
    
    return true;
}

bool Factory::has_reachable_storehouse(
    const PackageSender* sender,
    std::list<const PackageSender*>& visited) const
{
    // zapobiega zapętleniu
    if (std::find(visited.begin(), visited.end(), sender) != visited.end())
        return false;

    visited.push_back(sender);

    for (const auto& [receiver, _] : sender->receiver_preferences_.get_preferences()) {

        // jeżeli to storehouse → OK
        if (dynamic_cast<Storehouse*>(receiver))
            return true;

        // jeżeli to worker → rekurencja
        if (auto w = dynamic_cast<Worker*>(receiver)) {
            if (has_reachable_storehouse(w, visited))
                return true;
        }
    }
    return false;
}

// === load factory structure ===
static std::map<std::string, std::string> parse_kv(std::string line) {
    std::map<std::string, std::string> out;
    std::istringstream iss(line);
    std::string token;
    iss >> token; // typ
    while (iss >> token) {
        auto pos = token.find('=');
        out[token.substr(0, pos)] = token.substr(pos + 1);
    }
    return out;
}

Factory load_factory_structure(std::istream& is) {
    Factory f;
    std::string line;

    while (std::getline(is, line)) {
        if (line.empty()) continue;

        if (line.rfind("LOADING_RAMP", 0) == 0) {
            auto kv = parse_kv(line);
            f.add_ramp(Ramp(
                std::stoi(kv["id"]),
                std::stoi(kv["delivery-interval"])
            ));
        }
        else if (line.rfind("WORKER",0) == 0) {
            auto kv = parse_kv(line);
            PackageQueueType qt =
                kv["queue-type"] == "FIFO" ? FIFO : LIFO;
            f.add_worker(Worker(
                std::stoi(kv["id"]),
                std::stoi(kv["processing-time"]),
                std::make_unique<PackageQueue>(qt)
            ));
        }
        else if (line.rfind("STOREHOUSE",0) == 0) {
            auto kv = parse_kv(line);
            f.add_storehouse(Storehouse(std::stoi(kv["id"])));
        }
        else if (line.rfind("LINK", 0) == 0) {
            auto kv = parse_kv(line);

            auto parse = [](const std::string& s) {
                auto p = s.find('-');
                return std::pair{s.substr(0, p), std::stoi(s.substr(p + 1))};
            };

            auto [src_t, src_id] = parse(kv["src"]);
            auto [dst_t, dst_id] = parse(kv["dest"]);

            PackageSender* src = nullptr;
            IPackageReceiver* dst = nullptr;

            if (src_t == "ramp") src = f.find_ramp_by_id(src_id);
            else src = f.find_worker_by_id(src_id);

            if (dst_t == "worker") dst = f.find_worker_by_id(dst_id);
            else dst = f.find_storehouse_by_id(dst_id);

            src->receiver_preferences_.add_receiver(dst);
        }
    }
    return f;
}


// === save factory structure ===

static std::string node_name(const IPackageReceiver* r) {
    if (auto w = dynamic_cast<const Worker*>(r))
        return "worker-" + std::to_string(w->get_id());
    if (auto s = dynamic_cast<const Storehouse*>(r))
        return "store-" + std::to_string(s->get_id());
    return "";
}

void save_factory_structure(const Factory& f, std::ostream& os) {
    // RAMPY
    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); ++it) {
        os << "LOADING_RAMP id=" << it->get_id()
           << " delivery-interval=" << it->get_delivery_interval() << "\n";
    }

    // WORKERY
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        os << "WORKER id=" << it->get_id()
           << " processing-time=" << it->get_processing_duration()
           << " queue-type="
           << (it->get_queue()->get_queue_type() == FIFO ? "FIFO" : "LIFO")
           << "\n";
    }

    // STOREHOUSE
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << "\n";
    }

    // LINKI Z RAMP
    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); ++it) {
        for (const auto& [recv, _] : it->receiver_preferences_.get_preferences()) {
            os << "LINK src=ramp-" << it->get_id()
               << " dest=" << node_name(recv) << "\n";
        }
    }

    // LINKI Z WORKERÓW
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        for (const auto& [recv, _] : it->receiver_preferences_.get_preferences()) {
            os << "LINK src=worker-" << it->get_id()
               << " dest=" << node_name(recv) << "\n";
        }
    }
}




void Factory::remove_worker(ElementID id) {
    auto it = std::find_if(workers_.begin(), workers_.end(),
        [id](const Worker& w) { return w.get_id() == id; });

    if (it == workers_.end()) return;

    Worker* removed = &(*it);

    // usuń ze wszystkich preferencji
    for (auto& r : ramps_) {
        r.receiver_preferences_.remove_receiver(removed);
    }
    for (auto& w : workers_) {
        w.receiver_preferences_.remove_receiver(removed);
    }

    workers_.erase(it);
}