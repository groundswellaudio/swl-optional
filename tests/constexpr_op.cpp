#include "test_util.hpp"

int main(){
  {
    constexpr swl::optional<int> o2{};
    constexpr swl::optional<int> o3 = {};
    constexpr swl::optional<int> o4 = swl::nullopt;
    constexpr swl::optional<int> o5 = {swl::nullopt};
    constexpr swl::optional<int> o6(swl::nullopt);

    static_assert(!o2);
    static_assert(!o3);
    static_assert(!o4);
    static_assert(!o5);
    static_assert(!o6);
  }

  {
    constexpr swl::optional<int> o1 = 42;
    constexpr swl::optional<int> o2{42};
    constexpr swl::optional<int> o3(42);
    constexpr swl::optional<int> o4 = {42};
    constexpr int i = 42;
    constexpr swl::optional<int> o5 = std::move(i);
    constexpr swl::optional<int> o6{std::move(i)};
    constexpr swl::optional<int> o7(std::move(i));
    constexpr swl::optional<int> o8 = {std::move(i)};

    static_assert(*o1 == 42);
    static_assert(*o2 == 42);
    static_assert(*o3 == 42);
    static_assert(*o4 == 42);
    static_assert(*o5 == 42);
    static_assert(*o6 == 42);
    static_assert(*o7 == 42);
    static_assert(*o8 == 42);
  }
}
