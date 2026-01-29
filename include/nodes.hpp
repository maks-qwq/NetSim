#pragma once

#include <map>
#include <memory>
#include <optional>

#include "types.hpp"
#include "package.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"

/* ===== ReceiverType ===== */

enum class ReceiverType { WORKER, STOREHOUSE };

/* ===== IPackageReceiver ===== */

class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;

    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    //virtual ReceiverType get_receiver_type() const = 0;


    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
};

/* ===== Storehouse ===== */

class Storehouse : public IPackageReceiver {
public:
    explicit Storehouse(ElementID id);

    void receive_package(Package&& p) override;

    ElementID get_id() const override;
    //ReceiverType get_receiver_type() const override;

    const_iterator begin() const override;
    const_iterator cbegin() const override;
    const_iterator end() const override;
    const_iterator cend() const override;

private:
    ElementID id_;
    PackageQueue stockpile_{PackageQueueType::FIFO};
};

/* ===== ReceiverPreferences ===== */

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator);

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    
    const preferences_t& get_preferences() const;
    const_iterator begin() const;
    const_iterator end() const;

private:
    preferences_t preferences_;
    ProbabilityGenerator pg_;
};

/* ===== PackageSender ===== */

class PackageSender {
public:
    
    ReceiverPreferences receiver_preferences_;

    void push_package(Package&& p);
    void send_package();

    std::optional<Package>& get_sending_buffer();

protected:
    std::optional<Package> sending_buffer_;
};

/* ===== Ramp ===== */

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di);

    void deliver_goods(Time t);
    ElementID get_id() const;

    TimeOffset get_delivery_interval() const;

private:
    ElementID id_;
    TimeOffset di_;
};

/* ===== Worker ===== */

class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue);

    void receive_package(Package&& p) override;
    void do_work(Time t);

    ElementID get_id() const override;
    //ReceiverType get_receiver_type() const override;

    std::optional<Package>& get_processing_buffer();

    const_iterator begin() const override;
    const_iterator cbegin() const override;
    const_iterator end() const override;
    const_iterator cend() const override;

    TimeOffset get_processing_duration() const;
    IPackageQueue* get_queue() const;

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> queue_;
    std::optional<Package> processing_buffer_;
    Time processing_start_time_{0};
};
