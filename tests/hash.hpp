#include "test_util.hpp"

#include <string>
#include <memory>
#include <cassert>

namespace hash_test_impl{
	struct A {};
	struct B {};
}

namespace std {
	template <>
	struct hash<hash_test_impl::B> {
	  size_t operator()(hash_test_impl::B const&) { return 0; }
	};
}

template <class T>
inline void test_hash_enabled_for_type(){
	static_assert( requires{ std::hash<T>{}; } );
}

template <class T>
inline void test_hash_disabled_for_type(){
	static_assert( not requires{ std::hash<T>{}; } );
}

inline void hash()
{
	using namespace hash_test_impl;
	using swl::optional;
	
    const std::size_t nullopt_hash =
        std::hash<optional<double>>{}(optional<double>{});

	/* 
    {
        optional<B> opt;
        ASSERT_NOT_NOEXCEPT(std::hash<optional<B>>()(opt));
        ASSERT_NOT_NOEXCEPT(std::hash<optional<const B>>()(opt));
    } */ 

    {
        typedef int T;
        optional<T> opt;
        assert(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = 2;
        assert(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
        typedef std::string T;
        optional<T> opt;
        assert(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = std::string("123");
        assert(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
        typedef std::unique_ptr<int> T;
        optional<T> opt;
        assert(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = std::unique_ptr<int>(new int(3));
        assert(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
      test_hash_enabled_for_type<optional<int> >();
      test_hash_enabled_for_type<optional<int*> >();
      test_hash_enabled_for_type<optional<const int> >();
      test_hash_enabled_for_type<optional<int* const> >();

      test_hash_disabled_for_type<optional<A>>();
      test_hash_disabled_for_type<optional<const A>>();
	
      test_hash_enabled_for_type<optional<B>>();
      test_hash_enabled_for_type<optional<const B>>();
    }
}
