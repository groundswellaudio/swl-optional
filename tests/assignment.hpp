#include "test_util.hpp"
#include <cassert>

inline void assignement() {
    swl::optional<int> o1 = 42;
    swl::optional<int> o2 = 12;
    swl::optional<int> o3;

    o1 = o1;
    assert(*o1 == 42);

    o1 = o2;
    assert(*o1 == 12);

    o1 = o3;
    assert(!o1);

    o1 = 42;
    assert(*o1 == 42);

    o1 = swl::nullopt;
    assert(!o1);

    o1 = std::move(o2);
    assert(*o1 == 12);

    swl::optional<short> o4 = 42;

    o1 = o4;
    assert(*o1 == 42);

    o1 = std::move(o4);
    assert(*o1 == 42);
}

/* 
TEST_CASE("Assignment reference", "[assignment.ref]") {
    auto i = 42;
    auto j = 12;
    
    swl::optional<int&> o1 = i;
    swl::optional<int&> o2 = j;
    swl::optional<int&> o3;

    o1 = o1;
    assert(*o1 == 42);
    assert(&*o1 == &i);    

    o1 = o2;
    assert(*o1 == 12);

    o1 = o3;
    assert(!o1);

    auto k = 42;
    o1 = k;
    assert(*o1 == 42);
    assert(*o1 == i);
    assert(*o1 == k);        
    assert(&*o1 != &i);
    assert(&*o1 == &k);

    k = 12;
    assert(*o1 == 12);

    o1 = swl::nullopt;
    assert(!o1);

    o1 = std::move(o2);
    assert(*o1 == 12);
} */ 
