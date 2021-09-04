#include "test_util.hpp"
#include <type_traits>

int main() {
    static_assert(std::is_trivially_copy_constructible<swl::optional<int>>::value);
    static_assert(std::is_trivially_copy_assignable<swl::optional<int>>::value);
    static_assert(std::is_trivially_move_constructible<swl::optional<int>>::value);
    static_assert(std::is_trivially_move_assignable<swl::optional<int>>::value);
    static_assert(std::is_trivially_destructible<swl::optional<int>>::value);

    {
        struct T {
            T(const T&) = default;
            T(T&&) = default;
            T& operator=(const T&) = default;
            T& operator=(T&&) = default;
            ~T() = default;
        };
        static_assert(std::is_trivially_copy_constructible<swl::optional<T>>::value);
        static_assert(std::is_trivially_copy_assignable<swl::optional<T>>::value);
        static_assert(std::is_trivially_move_constructible<swl::optional<T>>::value);
        static_assert(std::is_trivially_move_assignable<swl::optional<T>>::value);
        static_assert(std::is_trivially_destructible<swl::optional<T>>::value);
    }

    {
        struct T {
            T(const T&){}
            T(T&&) {};
            T& operator=(const T&) { return *this; }
            T& operator=(T&&) { return *this; };
            ~T(){}
        };
        static_assert(!std::is_trivially_copy_constructible<swl::optional<T>>::value);
        static_assert(!std::is_trivially_copy_assignable<swl::optional<T>>::value);
        static_assert(!std::is_trivially_move_constructible<swl::optional<T>>::value);
        static_assert(!std::is_trivially_move_assignable<swl::optional<T>>::value);
        static_assert(!std::is_trivially_destructible<swl::optional<T>>::value);
    }
	
	
	// ====================
	
    static_assert(std::is_copy_constructible<swl::optional<int>>::value);
    static_assert(std::is_copy_assignable<swl::optional<int>>::value);
    static_assert(std::is_move_constructible<swl::optional<int>>::value);
    static_assert(std::is_move_assignable<swl::optional<int>>::value);
    static_assert(std::is_destructible<swl::optional<int>>::value);

    {
        struct T {
            T(const T&) = default;
            T(T&&) = default;
            T& operator=(const T&) = default;
            T& operator=(T&&) = default;
            ~T() = default;
        };
        static_assert(std::is_copy_constructible<swl::optional<T>>::value);
        static_assert(std::is_copy_assignable<swl::optional<T>>::value);
        static_assert(std::is_move_constructible<swl::optional<T>>::value);
        static_assert(std::is_move_assignable<swl::optional<T>>::value);
        static_assert(std::is_destructible<swl::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)=delete;
            T(T&&)=delete;
            T& operator=(const T&)=delete;
            T& operator=(T&&)=delete;
        };
        static_assert(!std::is_copy_constructible<swl::optional<T>>::value);
        static_assert(!std::is_copy_assignable<swl::optional<T>>::value);
        static_assert(!std::is_move_constructible<swl::optional<T>>::value);
        static_assert(!std::is_move_assignable<swl::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)=delete;
            T(T&&)=default;
            T& operator=(const T&)=delete;
            T& operator=(T&&)=default;
        };
        static_assert(!std::is_copy_constructible<swl::optional<T>>::value);
        static_assert(!std::is_copy_assignable<swl::optional<T>>::value);
        static_assert(std::is_move_constructible<swl::optional<T>>::value);
        static_assert(std::is_move_assignable<swl::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)=default;
            T(T&&)=delete;
            T& operator=(const T&)=default;
            T& operator=(T&&)=delete;
        };
        static_assert(std::is_copy_constructible<swl::optional<T>>::value);
        static_assert(std::is_copy_assignable<swl::optional<T>>::value);
    }
}
