#include "nodes.hpp"

/* ===== Storehouse ===== */

Storehouse::Storehouse(ElementID id) : id_(id) {}

void Storehouse::receive_package(Package&&) {}

ElementID Storehouse::get_id() const {
    return id_;
}

// ReceiverType Storehouse::get_receiver_type() const {
//     return ReceiverType::STOREHOUSE;
// }

/* ===== ReceiverPreferences ===== */

ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg)
    : pg_(pg) {}

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double p = 1.0 / (preferences_.size() + 1);
    for (auto& [_, prob] : preferences_) prob = p;
    preferences_[r] = p;
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_.erase(r);
    if (preferences_.empty()) return;
    double p = 1.0 / preferences_.size();
    for (auto& [_, prob] : preferences_) prob = p;
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double r = pg_(); //------------------------------------TUTAJ ZMIENIŁEM
    double acc = 0.0;
    for (auto& [receiver, prob] : preferences_) {
        acc += prob;
        if (r <= acc) return receiver;
    }
    return preferences_.begin()->first;
}

/* ===== PackageSender ===== */

void PackageSender::push_package(Package&& p) {
    sending_buffer_.emplace(std::move(p));
}

void PackageSender::send_package() {
    if (!sending_buffer_) return;
    auto receiver = receiver_preferences_.choose_receiver();
    receiver->receive_package(std::move(*sending_buffer_));
    sending_buffer_.reset();
}

std::optional<Package>& PackageSender::get_sending_buffer() {
    return sending_buffer_;
}

/* ===== Ramp ===== */

Ramp::Ramp(ElementID id, TimeOffset di)
    : id_(id), di_(di) {}

void Ramp::deliver_goods(Time t) {
    if ((t - 1) % di_ == 0) {
        push_package(Package());
    }
}

ElementID Ramp::get_id() const {
    return id_;
}

/* ===== Worker ===== */

Worker::Worker(ElementID id, TimeOffset pd,
               std::unique_ptr<IPackageQueue> queue)
    : id_(id), pd_(pd), queue_(std::move(queue)) {}

void Worker::receive_package(Package&& p) {
    queue_->push(std::move(p));
}

void Worker::do_work(Time t) {
    if (!processing_buffer_ && !queue_->empty()) {
        processing_buffer_.emplace(queue_->pop());
        processing_start_time_ = t;
    }

    if (processing_buffer_ &&
        t - processing_start_time_ + 1 == pd_) {
        push_package(std::move(*processing_buffer_));
        processing_buffer_.reset();
    }
}

ElementID Worker::get_id() const {
    return id_;
}

// ReceiverType Worker::get_receiver_type() const {
//     return ReceiverType::WORKER;
// }

std::optional<Package>& Worker::get_processing_buffer() {
    return processing_buffer_;
}

/* ===== Storehouse iterators ===== */

IPackageReceiver::const_iterator Storehouse::begin() const {
    return stockpile_.begin();
}
IPackageReceiver::const_iterator Storehouse::cbegin() const {
    return stockpile_.cbegin();
}
IPackageReceiver::const_iterator Storehouse::end() const {
    return stockpile_.end();
}
IPackageReceiver::const_iterator Storehouse::cend() const {
    return stockpile_.cend();
}

/* ===== ReceiverPreferences ===== */



const ReceiverPreferences::preferences_t&
ReceiverPreferences::get_preferences() const {
    return preferences_;
}

ReceiverPreferences::const_iterator ReceiverPreferences::begin() const {
    return preferences_.begin();
}

ReceiverPreferences::const_iterator ReceiverPreferences::end() const {
    return preferences_.end();
}



/* ===== Worker iterators ===== */

IPackageReceiver::const_iterator Worker::begin() const {
    return queue_->begin();
}
IPackageReceiver::const_iterator Worker::cbegin() const {
    return queue_->cbegin();
}
IPackageReceiver::const_iterator Worker::end() const {
    return queue_->end();
}
IPackageReceiver::const_iterator Worker::cend() const {
    return queue_->cend();
}

