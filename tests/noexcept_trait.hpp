#include "test_util.hpp"

inline void noexcept_trait() {
  swl::optional<int> o1{4};
  swl::optional<int> o2{42};

  {
    assert(noexcept(o1 == swl::nullopt));
    assert(noexcept(swl::nullopt == o1));
    assert(noexcept(o1 != swl::nullopt));
    assert(noexcept(swl::nullopt != o1));
    assert(noexcept(o1 < swl::nullopt));
    assert(noexcept(swl::nullopt < o1));
    assert(noexcept(o1 <= swl::nullopt));
    assert(noexcept(swl::nullopt <= o1));
    assert(noexcept(o1 > swl::nullopt));
    assert(noexcept(swl::nullopt > o1));
    assert(noexcept(o1 >= swl::nullopt));
    assert(noexcept(swl::nullopt >= o1));
  }
  

  {
    struct nothrow_swappable {
      nothrow_swappable &swap(const nothrow_swappable &) noexcept {
        return *this;
      }
    };

    struct throw_swappable {
      throw_swappable() = default;
      throw_swappable(const throw_swappable &) {}
      throw_swappable(throw_swappable &&) {}
      throw_swappable &swap(const throw_swappable &) { return *this; }
    };

    swl::optional<nothrow_swappable> ont;
    swl::optional<throw_swappable> ot;

    assert(noexcept(ont.swap(ont)));
    assert(!noexcept(ot.swap(ot)));
  }

  {
    assert(noexcept(swl::optional<int>{}));
    assert(noexcept(swl::optional<int>{swl::nullopt}));

    struct nothrow_move {
      nothrow_move(nothrow_move &&) noexcept = default;
    };

    struct throw_move {
      throw_move(throw_move &&){};
    };

    using nothrow_opt = swl::optional<nothrow_move>;
    using throw_opt = swl::optional<throw_move>;

    assert(std::is_nothrow_move_constructible<nothrow_opt>::value);
    assert(!std::is_nothrow_move_constructible<throw_opt>::value);
  }

  {
    assert(noexcept(o1 = swl::nullopt));

    struct nothrow_move_assign {
      nothrow_move_assign() = default;
      nothrow_move_assign(nothrow_move_assign &&) noexcept = default;
      nothrow_move_assign &operator=(const nothrow_move_assign &) = default;
    };

    struct throw_move_assign {
      throw_move_assign() = default;
      throw_move_assign(throw_move_assign &&){};
      throw_move_assign &operator=(const throw_move_assign &) { return *this; }
    };

    using nothrow_opt = swl::optional<nothrow_move_assign>;
    using throw_opt = swl::optional<throw_move_assign>;

    assert(noexcept(std::declval<nothrow_opt>() = std::declval<nothrow_opt>()));
    assert(!noexcept(std::declval<throw_opt>() = std::declval<throw_opt>()));
  }

  {
    assert(noexcept(static_cast<bool>(o1)));
    assert(noexcept(o1.has_value()));
  }

  { 
  	assert(noexcept(o1.reset())); 
  }
}
