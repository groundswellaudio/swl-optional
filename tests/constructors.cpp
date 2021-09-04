#include "test_util.hpp"
#include <vector>

struct foo {
  foo() = default;
  foo(foo&) = delete;
  foo(foo&&) {};
};

int main(){
  swl::optional<int> o1;
  assert(!o1);

  swl::optional<int> o2 = swl::nullopt;
  assert(!o2);

  swl::optional<int> o3 = 42;
  assert(*o3 == 42);

  swl::optional<int> o4 = o3;
  assert(*o4 == 42);

  swl::optional<int> o5 = o1;
  assert(!o5);

  swl::optional<int> o6 = std::move(o3);
  assert(*o6 == 42);

  swl::optional<short> o7 = 42;
  assert(*o7 == 42);

  swl::optional<int> o8 = o7;
  assert(*o8 == 42);

  swl::optional<int> o9 = std::move(o7);
  assert(*o9 == 42);
	
/* 
  {
    swl::optional<int &> o;
    assert(!o);

    swl::optional<int &> oo = o;
    assert(!oo);
  }
  {
    auto i = 42;
    swl::optional<int &> o = i;
    assert(o);
    assert(*o == 42);

    swl::optional<int &> oo = o;
    assert(oo);
    assert(*oo == 42);
  }
  */ 

  std::vector<foo> v;
  v.emplace_back();
  swl::optional<std::vector<foo>> ov = std::move(v);
  assert(ov->size() == 1);
}
