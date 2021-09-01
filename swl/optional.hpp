// ====================================================================
// Developed in 2021 by Jean-Baptiste Vallon Hoarau
// github : groundswellaudio 
// jeanbaptiste.vallon@gmail.com
// ====================================================================

#ifndef SWL_CPP_LIBRARY_OPTIONAL_HPP
#define SWL_CPP_LIBRARY_OPTIONAL_HPP

#include <initializer_list>
#include <type_traits>
#include <memory>
#include <exception>
#include <functional>
#include <compare>

namespace swl {

#define SWL_MOV(x) static_cast<std::remove_reference_t<decltype(x)>&&>(x)
#define SWL_FWD(x) static_cast<decltype(x)&&>(x)

struct bad_optional_access : std::exception {
	const char* what() const noexcept override {
		return "bad_optional_access in swl::optional.";
	}
};

struct nullopt_t{};
struct in_place_t{};

inline static constexpr nullopt_t nullopt;
inline static constexpr in_place_t in_place;

template <class T>
class optional {

	static constexpr bool has_move_assign = std::is_move_assignable_v<T> && std::is_move_constructible_v<T>;
	static constexpr bool has_trivial_move_assign = std::is_trivially_move_assignable_v<T> && std::is_trivially_move_constructible_v<T>;
	static constexpr bool has_trivial_copy = std::is_trivially_copy_assignable_v<T> && std::is_trivially_copy_constructible_v<T>;
	static constexpr bool has_copy = std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>;

	union {
		char dummy;
		T value_;
	};

	bool active;
	
	// helpers for the optional<U> ctor
	template <class... Args>
	static constexpr bool constructible_from_one = (std::is_constructible_v<T, Args> || ...);
	
	template <class... Args>
	static constexpr bool convertible_from_one = (std::is_convertible_v<Args, T> || ...);
	
	template <class... Args>
	static constexpr bool assignable_from_one = (std::is_assignable_v<T, Args> || ...);
	
	// X is just a cref qualified U
	template <class X, class U>
	static constexpr bool opt_ctor_req = 
		std::is_constructible_v<T, X>
		 and not constructible_from_one<optional<U>&, const optional<U>&, optional<U>&&, const optional<U>&&>
		 and not convertible_from_one<optional<U>&, const optional<U>&, optional<U>&&, const optional<U>&&>;
	
	template <class X, class U>
	static constexpr bool opt_assign_req = 
		std::is_assignable_v<T&, X>
		 and opt_ctor_req<X, U> 
		 and not assignable_from_one<optional<U>&, optional<U>&&, const optional<U>&, const optional<U>&&>;
	
	// we need to zero-init the union when constructing a constexpr optional
	constexpr void maybe_init_union(){
		if (std::is_constant_evaluated())
			dummy = 0;
	}
	
	public : 
	
	constexpr optional() noexcept : 
	optional{nullopt}
	{}
	
	constexpr optional(nullopt_t) noexcept : 
	dummy{0}, active{false}
	{}
	
	constexpr optional(const optional& o) 
		requires std::is_trivially_copy_constructible_v<T>
	= default; 

	constexpr optional(const optional& o) 
		requires (std::is_copy_constructible_v<T> and not std::is_trivially_copy_constructible_v<T>)
	: active{o.active}
	{
		if (not o.active){
			maybe_init_union();
			return;
		}
		this->construct_from(o.value_);
	}
	
	constexpr optional(optional&& o)
		requires std::is_trivially_move_constructible_v<T>
	= default;
	
	constexpr optional(optional&& o) 
		noexcept(std::is_nothrow_move_constructible_v<T>)
		requires (std::is_move_constructible_v<T> and not std::is_trivially_move_constructible_v<T>)
	: active{o.active}
	{
		if (not o.active){
			maybe_init_union();
			return;
		}
		this->construct_from(SWL_FWD(o.value_));
	}
	
	// in place ctors
	
	template <class U = T>
	constexpr explicit(not std::is_convertible_v<U, T>) optional(U&& u)
		requires
		(std::is_constructible_v<T, U> 
		 and not std::is_same_v<std::remove_cvref_t<U>, in_place_t>
		 and not std::is_same_v<std::remove_cvref_t<U>, optional>)
	: active{true}
	{
		this->construct_from(SWL_FWD(u));
	}
	
	template <class... Args>
		requires std::is_constructible_v<T, Args...>
	constexpr explicit optional(in_place_t, Args&&... args)
	: active{true}
	{
		this->construct_from(SWL_FWD(args)...);
	}
	
	template <class U, class... Args>
	constexpr explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args)
		requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>
	: active{true}
	{
		this->construct_from(il, SWL_FWD(args)...);
	}
	
	// conversion ctors
	
	template <class U = T> 
	constexpr explicit(not std::is_convertible_v<const U&, T>) optional(const optional<U>& o)
		requires opt_ctor_req<const U&, U>
	: active{o.has_value()}
	{
		if (not o){
			maybe_init_union();
			return;
		}
		this->construct_from(*o);
	}
	
	template <class U = T> 
	constexpr explicit(not std::is_convertible_v<U, T>) optional(optional<U>&& o)
		requires opt_ctor_req<U&&, U>
	: active{o.has_value()}
	{
		if (not o){
			maybe_init_union();
			return;
		}
		this->construct_from(*o);
	}

	// ======================================================
	// assign 

	constexpr optional& operator=(const optional& o)
		requires (has_copy and not has_trivial_copy)
	{
		switch( static_cast<char>(active) + static_cast<char>(o.active) * 2 ){
			case 0 : // no value in either
				break; 
			case 1 : // rhs is empty
				reset_no_check();
				break;
			case 2 : // this is empty
				this->construct_from(o.value());
				break;
			case 3 : 
				this->value_ = o.value();
				break;
		}
		return *this;
	}

	constexpr optional& operator=(const optional& o)
		requires has_trivial_copy
	= default;

	constexpr optional& operator=(optional&& o)
		requires has_trivial_move_assign
	= default;

	constexpr optional& operator=(optional&& o)
		requires (has_move_assign and not has_trivial_move_assign)
	{
		switch( static_cast<char>(active) + static_cast<char>(o.active) * 2 ){
			case 0 : 
				break; 
			case 1 : 
				reset_no_check();
				break;
			case 2 :
				this->construct_from(SWL_FWD(o).value());
				break;
			case 3 : 
				*this = SWL_FWD(o).value();
				break;
		}
		return *this;
	}
	
	template <class U = T>
	constexpr optional& operator=(U&& u)
		requires 
		(not std::is_same_v<std::remove_cvref_t<U>, optional>
		 and not(std::is_scalar_v<T> and std::is_same_v<T, std::decay_t<U>>)
		 and std::is_constructible_v<T, U>
		 and std::is_assignable_v<T&, U>)
	{
		if (active)
			**this = SWL_FWD(u);
		else
			this->construct_from(SWL_FWD(u));
		return *this;	
	}
	
	// conversion assign
	
	template <class U>
	constexpr optional& operator=(const optional<U>& u)
		requires opt_assign_req<const U&, U>
	{
		switch( char(active) + char(u.has_value()) * 2 ){
			case 0 : 
				break; 
			case 1 : 
				this->reset_no_check();
				break;
			case 2 :
				this->construct_from(*u);
				break;
			case 3 : 
				**this = *u;
				break;
		}
		return *this;
	}
	
	template <class U>
	constexpr optional& operator=(optional<U>&& u)
		requires opt_assign_req<U, U>
	{
		switch( char(active) + char(u.has_value()) * 2 ){
			case 0 : 
				break; 
			case 1 : 
				this->reset_no_check();
				break;
			case 2 :
				this->construct_from(*SWL_FWD(u));
				break;
			case 3 : 
				**this = *SWL_FWD(u);
				break;
		}
		return *this;
	}
	
	// ======================================================
	// emplace

	template <class... Args>
		requires std::is_constructible_v<T, Args...>
	constexpr void emplace(Args&&... args){
		this->reset();
		this->construct_from(SWL_FWD(args)...);
	}

	template <class U, class... Args>
		requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>
	constexpr void emplace(std::initializer_list<U> li, Args&&... args){
		this->reset();
		this->construct_from(li, SWL_FWD(args)...);
	}

	// ======================================================
	// dtor

	constexpr ~optional() 
		requires std::is_trivially_destructible_v<T> 
	= default;

	constexpr ~optional() 
		requires (not std::is_trivially_destructible_v<T>)
	{
		if (!active)
			return;
		value_.~T();
	}

	// ======================================================
	// observers

	constexpr T* operator->() {
		return this->ptr();
	}

	constexpr T* operator->() const {
		return this->ptr();
	}

	constexpr T& operator*() &                  { return value_; }
	constexpr const T& operator*() const&       { return value_; }
	constexpr T&& operator*() &&                { return static_cast<T&&>(value_); }
	constexpr const T&& operator*() const&&     { return static_cast<const T&&>(value_); }

	constexpr explicit operator bool() const noexcept {
		return this->active;
	}

	constexpr bool has_value() const noexcept {
		return this->active;
	}

	constexpr auto&& value() &             { return bool(*this) ? **this : throw bad_optional_access{}; }
	constexpr auto&& value() const &       { return bool(*this) ? **this : throw bad_optional_access{}; }
	constexpr auto&& value() &&            { return bool(*this) ? static_cast<T&&>(value_) : throw bad_optional_access{}; }
	constexpr auto&& value() const &&      { return bool(*this) ? static_cast<const T&&>(value_) : throw bad_optional_access{}; }

	template <class U>
		requires (std::is_copy_constructible_v<T> && std::is_convertible_v<U&&, T>)
	constexpr T value_or(U&& u) const & {
		return bool(*this) ? **this : static_cast<T>(SWL_FWD(u));
	}

	template <class U>
		requires (std::is_move_constructible_v<T> && std::is_convertible_v<U&&, T>)
	constexpr T value_or(U&& u) && {
		return bool(*this) ? static_cast<T&&>(value_) : static_cast<T>(SWL_FWD(u));
	}

	// ======================================================
	// modifiers 
	
	constexpr void reset() noexcept {
		if (!active)
			return;
		reset_no_check();
	}
	
	// ======================================================
	// swap
	
	constexpr void swap(optional& o)
		noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_swappable_v<T>)
		requires std::is_move_constructible_v<T>
	{
		switch(char(active) + char(o.active) * 2){
			case 0 : 
				break;
			case 1 : 
				o.construct_from(SWL_MOV(**this));
				this->reset_no_check();
				break;
			case 2 : 
				this->construct_from(SWL_MOV(*o));
				o.reset_no_check();
				break;
			case 3 : 
				using std::swap;
				swap(**this, *o);
				break;
		}
	}
	
	private :

	constexpr void reset_no_check(){
		if constexpr (not std::is_trivially_destructible_v<T>){
			value_.~T();
		}
		active = false;
	}
	
	// known bug in GCC10 for construct_at : https://gcc.gnu.org/bugzilla/show_bug.cgi?id=97195
	template <class... Args>
	constexpr void construct_from(Args&&... args){
		std::construct_at(std::addressof(value_), static_cast<Args&&>(args)...);	
		active = true;
	}

	T* ptr(){ return std::addressof(value_); }
	const T* ptr() const { return std::addressof(value_); }

};

// make these specializations illformed
template <class T>
	requires 
	(std::is_same_v<std::decay_t<T>, nullopt_t> 
	 or std::is_same_v<std::decay_t<T>, in_place_t>)
class optional<T>;

template <class T>
class optional<T&>;

// =================================================================
// relational operators

template <class T, class U>
constexpr bool operator==(const optional<T>& lhs, const optional<U>& rhs)
	requires requires { bool(*lhs == *rhs); }
{	
	if (lhs.has_value() != rhs.has_value())
		return false;
	if (not lhs.has_value())
		return true;
	return *lhs == *rhs;
}

template <class T, class U>
constexpr bool operator!=(const optional<T>& lhs, const optional<U>& rhs)
	requires requires { bool(*lhs != *rhs); }
{	
	if (lhs.has_value() != rhs.has_value())
		return true;
	if (not lhs)
		return false;
	return *lhs != *rhs;
}

template <class T, class U>
constexpr bool operator<(const optional<T>& lhs, const optional<U>& rhs)
	requires requires { bool(*lhs < *rhs); }
{	
	if (not lhs)
		return true;
	if (not rhs)
		return false;
	return *lhs < *rhs;
}

template <class T, class U>
constexpr bool operator>(const optional<T>& lhs, const optional<U>& rhs)
	requires requires { bool(*lhs > *rhs); }
{	
	if (not lhs)
		return false;
	if (not rhs)
		return true;
	return *lhs > *rhs;
}

template <class T, class U>
constexpr bool operator<=(const optional<T>& lhs, const optional<U>& rhs)
	requires requires { bool(*lhs <= *rhs); }
{	
	if (not lhs)
		return true;
	if (not rhs)
		return false;
	return *lhs <= *rhs;
}

template <class T, class U>
constexpr bool operator>=(const optional<T>& lhs, const optional<U>& rhs)
	requires requires { bool(*lhs >= *rhs); }
{	
	if (not lhs)
		return false;
	if (not rhs)
		return true;
	return *lhs >= *rhs;
}

// =================================================================
// comparisons with nullopt

template <class T>
constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept {
	return not x;
}

template <class T>
constexpr std::strong_ordering operator<=>(const optional<T>& x, nullopt_t) noexcept {
	return bool(x) <=> false;
}

// =================================================================
// comparisons with T

#define MAKE_OP(OP, DEFAULT1, DEFAULT2) \
template <class T, class U> \
constexpr bool operator OP (const optional<T>& lhs, const U& rhs) \
	requires requires { bool(*lhs OP rhs); } \
{\
	return bool(lhs) ? *lhs OP rhs : DEFAULT1; \
} \
\
template <class T, class U> \
constexpr bool operator OP (const T& lhs, const optional<U>& rhs) \
	requires requires { bool(lhs OP *rhs); } \
{\
	return bool(rhs) ? lhs OP *rhs : DEFAULT2; \
}\

MAKE_OP(==, false, false)
MAKE_OP(!=, true, true)
MAKE_OP(<, true, false)
MAKE_OP(>, false, true)
MAKE_OP(<=, true, false)
MAKE_OP(>=, false, true)

// =================================================================
// specialized algorithms

template <class T>
	requires (std::is_move_constructible_v<T> && std::is_swappable_v<T>)
constexpr void swap(optional<T>& x, optional<T>& y)
	noexcept(noexcept(x.swap(y)))
{
	x.swap(y);
}

template <class T>
constexpr auto make_optional(T&& v){
	return optional<std::decay_t<T>>{SWL_FWD(v)};
}

template <class T, class... Args>
constexpr auto make_optional(Args&&... args){
	return optional<T>{in_place, SWL_FWD(args)...};
}

template <class T, class U, class... Args>
constexpr auto make_optional(std::initializer_list<U> il, Args&&... args){
	return optional<T> {in_place, il, SWL_FWD(args)...};
}

} // SWL

// =================================================================
// hash support
namespace std {
	template <class T>
		requires requires { std::hash<std::decay_t<T>>{}; }
	struct hash<swl::optional<T>> {
		constexpr std::size_t operator()(const swl::optional<T>& o) const {
			if (not o)
				return -1;
			return std::hash<std::decay_t<T>>{}(*o);
		}
	};
} // STD

#undef SWL_MOV
#undef SWL_FWD

#endif // header guards