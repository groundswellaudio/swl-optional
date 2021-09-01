#include "test_util.hpp"
#include <utility>
#include <tuple>
#include <cassert>

inline void emplace() {
    swl::optional<std::pair<std::pair<int,int>, std::pair<double, double>>> i;
    i.emplace(std::piecewise_construct, std::make_tuple(0,2), std::make_tuple(3,4));
    assert(i->first.first == 0);
    assert(i->first.second == 2);
    assert(i->second.first == 3);
    assert(i->second.second == 4);    
}
