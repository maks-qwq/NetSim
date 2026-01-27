#pragma once

#include "package.hpp"
#include <iostream>
#include <list>
#include <stdexcept>

enum PackageQueueType {
    LIFO,
    FIFO
};

class IPackageStockpile {
public:
    virtual void push(Package&& package) = 0; //dodaje na koniec listy nowy element
    virtual bool empty() = 0; //sprawdza czy lista jest pusta 
    virtual size_t size() = 0; //sprawdza ile elementów w liście

    using const_iterator = std::list<Package>::const_iterator; //dodanie aliasu

    virtual const_iterator begin() const = 0; //iteratory
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;
};


class IPackageQueue: public IPackageStockpile{
public:
    virtual Package pop() = 0; //usuwa elment z listy w zależności od typu
    virtual PackageQueueType get_queue_type() const = 0; //zwraca typ kolejki (LIFO/FIFO)
    ~IPackageQueue() override = default;
};


class PackageQueue: public IPackageQueue{
public:
    explicit PackageQueue(PackageQueueType type) : type_(type) {} 

    void push(Package&& package) override {package_list_.emplace_back(std::move(package));}

    bool empty() override{return package_list_.empty();}

    size_t size() override{return package_list_.size();}
    
    Package pop() override;

    PackageQueueType get_queue_type() const override { return type_;}
    
    const_iterator begin() const override { return package_list_.cbegin(); }
    const_iterator end() const override { return package_list_.cend(); }
    const_iterator cbegin() const override { return package_list_.cbegin(); }
    const_iterator cend() const override { return package_list_.cend(); }

    ~PackageQueue() override = default;
private:
    PackageQueueType type_;
    std::list<Package> package_list_;
};
