#pragma once

#include "types.hpp"
#include <set>

class Package {
public:
    Package(); //konstruktor domyślny
    explicit Package(ElementID id): id_(id) { assigned_IDs.insert(id_); } //konstruktor z wybraniem id
    Package(Package&& package) noexcept: id_(package.id_) {} //konstruktor kopiujący
    Package& operator=(Package&&) noexcept; //konstruktor przenoszący

    ElementID get_id() const {return id_; } //wyciągnięcie id

    ~Package(); //destruktor z usunieciem id

private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs; //póla przypisanych id
    static std::set<ElementID> freed_IDs; //zwolnione id
};
