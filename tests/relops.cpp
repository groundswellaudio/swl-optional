#include "test_util.hpp"

int main() {
  swl::optional<int> o1{4};
  swl::optional<int> o2{42};
  swl::optional<int> o3{};

  {
    assert(!(o1 == o2));
    assert(o1 == o1);
    assert(o1 != o2);
    assert(!(o1 != o1));
    assert(o1 < o2);
    assert(!(o1 < o1));
    assert(!(o1 > o2));
    assert(!(o1 > o1));
    assert(o1 <= o2);
    assert(o1 <= o1);
    assert(!(o1 >= o2));
    assert(o1 >= o1);
  }

  {
    assert(!(o1 == swl::nullopt));
    assert(!(swl::nullopt == o1));
    assert(o1 != swl::nullopt);
    assert(swl::nullopt != o1);
    assert(!(o1 < swl::nullopt));
    assert(swl::nullopt < o1);
    assert(o1 > swl::nullopt);
    assert(!(swl::nullopt > o1));
    assert(!(o1 <= swl::nullopt));
    assert(swl::nullopt <= o1);
    assert(o1 >= swl::nullopt);
    assert(!(swl::nullopt >= o1));

    assert(o3 == swl::nullopt);
    assert(swl::nullopt == o3);
    assert(!(o3 != swl::nullopt));
    assert(!(swl::nullopt != o3));
    assert(!(o3 < swl::nullopt));
    assert(!(swl::nullopt < o3));
    assert(!(o3 > swl::nullopt));
    assert(!(swl::nullopt > o3));
    assert(o3 <= swl::nullopt);
    assert(swl::nullopt <= o3);
    assert(o3 >= swl::nullopt);
    assert(swl::nullopt >= o3);
  }

  {
    assert(!(o1 == 1));
    assert(!(1 == o1));
    assert(o1 != 1);
    assert(1 != o1);
    assert(!(o1 < 1));
    assert(1 < o1);
    assert(o1 > 1);
    assert(!(1 > o1));
    assert(!(o1 <= 1));
    assert(1 <= o1);
    assert(o1 >= 1);
    assert(!(1 >= o1));

    assert(o1 == 4);
    assert(4 == o1);
    assert(!(o1 != 4));
    assert(!(4 != o1));
    assert(!(o1 < 4));
    assert(!(4 < o1));
    assert(!(o1 > 4));
    assert(!(4 > o1));
    assert(o1 <= 4);
    assert(4 <= o1);
    assert(o1 >= 4);
    assert(4 >= o1);
  }

  swl::optional<std::string> o4{"hello"};
  swl::optional<std::string> o5{"xyz"};

  {
    assert(!(o4 == o5));
    assert(o4 == o4);
    assert(o4 != o5);
    assert(!(o4 != o4));
    assert(o4 < o5);
    assert(!(o4 < o4));
    assert(!(o4 > o5));
    assert(!(o4 > o4));
    assert(o4 <= o5);
    assert(o4 <= o4);
    assert(!(o4 >= o5));
    assert(o4 >= o4);
  }

  {
    assert(!(o4 == swl::nullopt));
    assert(!(swl::nullopt == o4));
    assert(o4 != swl::nullopt);
    assert(swl::nullopt != o4);
    assert(!(o4 < swl::nullopt));
    assert(swl::nullopt < o4);
    assert(o4 > swl::nullopt);
    assert(!(swl::nullopt > o4));
    assert(!(o4 <= swl::nullopt));
    assert(swl::nullopt <= o4);
    assert(o4 >= swl::nullopt);
    assert(!(swl::nullopt >= o4));

    assert(o3 == swl::nullopt);
    assert(swl::nullopt == o3);
    assert(!(o3 != swl::nullopt));
    assert(!(swl::nullopt != o3));
    assert(!(o3 < swl::nullopt));
    assert(!(swl::nullopt < o3));
    assert(!(o3 > swl::nullopt));
    assert(!(swl::nullopt > o3));
    assert(o3 <= swl::nullopt);
    assert(swl::nullopt <= o3);
    assert(o3 >= swl::nullopt);
    assert(swl::nullopt >= o3);
  }

  {
    assert(!(o4 == "a"));
    assert(!("a" == o4));
    assert(o4 != "a");
    assert("a" != o4);
    assert(!(o4 < "a"));
    assert("a" < o4);
    assert(o4 > "a");
    assert(!("a" > o4));
    assert(!(o4 <= "a"));
    assert("a" <= o4);
    assert(o4 >= "a");
    assert(!("a" >= o4));

    assert(o4 == "hello");
    assert("hello" == o4);
    assert(!(o4 != "hello"));
    assert(!("hello" != o4));
    assert(!(o4 < "hello"));
    assert(!("hello" < o4));
    assert(!(o4 > "hello"));
    assert(!("hello" > o4));
    assert(o4 <= "hello");
    assert("hello" <= o4);
    assert(o4 >= "hello");
    assert("hello" >= o4);
  }
}
