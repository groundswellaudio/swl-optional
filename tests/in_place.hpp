#pragma once

#include "test_util.hpp"
#include <tuple>
#include <vector>
#include <cassert>

inline void in_place() {
  swl::optional<int> o1{swl::in_place};
  swl::optional<int> o2(swl::in_place);
  assert(o1);
  assert(o1 == 0);
  assert(o2);
  assert(o2 == 0);

  swl::optional<int> o3(swl::in_place, 42);
  assert(o3 == 42);

  swl::optional<std::tuple<int, int>> o4(swl::in_place, 0, 1);
  assert(o4);
  assert(std::get<0>(*o4) == 0);
  assert(std::get<1>(*o4) == 1);

  swl::optional<std::vector<int>> o5(swl::in_place, {0, 1});
  assert(o5);
  assert((*o5)[0] == 0);
  assert((*o5)[1] == 1);

  swl::optional<takes_init_and_variadic> o6(swl::in_place, {0, 1}, 2, 3);
  assert(o6->v[0] == 0);
  assert(o6->v[1] == 1);
  assert(std::get<0>(o6->t) == 2);
  assert(std::get<1>(o6->t) == 3);
}
