#include "test_util.hpp"

inline void nullopt() {
  swl::optional<int> o1 = swl::nullopt;
  swl::optional<int> o2{swl::nullopt};
  swl::optional<int> o3(swl::nullopt);
  swl::optional<int> o4 = {swl::nullopt};

  assert(!o1);
  assert(!o2);
  assert(!o3);
  assert(!o4);
}
