#include "test_util.hpp"

#include <tuple>
#include <vector>

int main() {
  auto o1 = swl::make_optional(42);
  auto o2 = swl::optional<int>(42);

  constexpr bool is_same = std::is_same<decltype(o1), swl::optional<int>>::value;
  assert(is_same);
  assert(o1 == o2);

  auto o3 = swl::make_optional<std::tuple<int, int, int, int>>(0, 1, 2, 3);
  assert(std::get<0>(*o3) == 0);
  assert(std::get<1>(*o3) == 1);
  assert(std::get<2>(*o3) == 2);
  assert(std::get<3>(*o3) == 3);

  auto o4 = swl::make_optional<std::vector<int>>({0, 1, 2, 3});
  assert(o4.value()[0] == 0);
  assert(o4.value()[1] == 1);
  assert(o4.value()[2] == 2);
  assert(o4.value()[3] == 3);

  auto o5 = swl::make_optional<takes_init_and_variadic>({0, 1}, 2, 3);
  assert(o5->v[0] == 0);
  assert(o5->v[1] == 1);
  assert(std::get<0>(o5->t) == 2);
  assert(std::get<1>(o5->t) == 3);
	
  /* 
  auto i = 42;
  auto o6 = swl::make_optional<int&>(i);
  assert((std::is_same<decltype(o6), swl::optional<int&>>::value));
  assert(o6);
  assert(*o6 == 42); */ 
}
