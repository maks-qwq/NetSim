#include "netsim.hpp"
#include <iostream>
#include "package.hpp"

int main(){

    Package p1(100);

    std::cout << p1.get_id();

    return 0;
}