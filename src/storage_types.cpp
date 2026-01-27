#include "storage_types.hpp"

Package PackageQueue::pop(){
    if (package_list_.empty()) {
        throw std::logic_error("Pusta kolejka");
    }
    Package package;

    if (type_ == PackageQueueType::FIFO){
        package = std::move(package_list_.front());
        package_list_.pop_front();
    } else{
        package = std::move(package_list_.back());
        package_list_.pop_back();
    }
    return package;
}