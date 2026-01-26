#pragma once

#include "types.hpp"

class Package {
public:
    Package();
    explicit Package(ElementID id);
    Package(Package&&) noexcept;
    Package& operator=(Package&&) noexcept;

    ElementID get_id() const;

    ~Package();

private:
    ElementID id_;
};
