#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package::Package() {
    if (freed_IDs.empty()) {
        if (assigned_IDs.empty()) {
            id_ = 1;
        } else {
            id_ = *std::prev(assigned_IDs.end()) + 1;
        }
    } else {
        id_ = *freed_IDs.begin();
        freed_IDs.erase(*freed_IDs.begin());
    }
    assigned_IDs.insert(id_);
}

Package& Package::operator=(Package&& package) noexcept {
    if (this == &package)
        return *this;
    id_ = package.id_;
    return *this;
}
Package::~Package() {
    freed_IDs.insert(id_);
    assigned_IDs.erase(id_);
}