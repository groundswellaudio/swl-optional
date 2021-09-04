#include "test_util.hpp"

struct move_detector {
  move_detector() = default;
  move_detector(move_detector &&rhs) { rhs.been_moved = true; }
  bool been_moved = false;
};

int main()  {
  swl::optional<int> o1 = 42;
  swl::optional<int> o2;
  const swl::optional<int> o3 = 42;

  assert(*o1 == 42);
  assert(*o1 == o1.value());
  assert(o2.value_or(42) == 42);
  assert(o3.value() == 42);
  auto success = std::is_same<decltype(o1.value()), int &>::value;
  assert(success);
  success = std::is_same<decltype(o3.value()), const int &>::value;
  assert(success);
  success = std::is_same<decltype(std::move(o1).value()), int &&>::value;
  assert(success);

  success = std::is_same<decltype(std::move(o3).value()), const int &&>::value;
  assert(success);

  swl::optional<move_detector> o4{swl::in_place};
  move_detector o5 = std::move(o4).value();
  assert(o4->been_moved);
  assert(!o5.been_moved);
}
