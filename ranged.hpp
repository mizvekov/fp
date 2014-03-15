/*
 * Copyright (c) 2014, Matheus Izvekov <mizvekov@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MIZVEKOV_RANGED_INCLUDE_HPP_INCLUDED
#define MIZVEKOV_RANGED_INCLUDE_HPP_INCLUDED

#include <functional>
#include <type_traits>
#include <limits>

#include "common.hpp"

template<typename A> struct is_ranged : std::false_type {};

template<class T, T LOW = std::numeric_limits<T>::lowest(),
                  T MAX = std::numeric_limits<T>::max()> struct ranged : std::numeric_limits<ranged<T,LOW,MAX>> {
	using base_type = T;

	static_assert(std::numeric_limits<T>::is_specialized, "invalid type");
	static_assert(LOW <= MAX, "please fix your low and max values");
	static_assert(LOW >= std::numeric_limits<T>::lowest(), "low overflow");
	static_assert(MAX <= std::numeric_limits<T>::max(), "max overflow");

	// Public Constructors
	constexpr ranged() : base(lo) {};
	template<class A> constexpr ranged(const A &a) : base(a) {
		// could assert here in case 'a' was out of range
		// but I dont see how to preserve constexpr'ness
	}
	template<class A, A A_LOW, A A_MAX> constexpr ranged(const ranged<A,A_LOW,A_MAX> &a) : ranged(a.base) {
		static_assert(A_LOW >= lo, "");
		static_assert(A_MAX <= hi, "");
	}
	//

	// Operators Implementation
	explicit constexpr operator T() const { return ranged(base).base; }

	template<class B> constexpr auto& operator =(const B &b) { return base = ranged<T, lo, hi>(b).base, *this; }

#define DECL template<class B, class = std::enable_if_t< is_ranged<B>{} > > constexpr
	DECL auto operator+(const B &b) const { return bin_op<rplus      , stdx::plus      >(*this, b); }
	DECL auto operator-(const B &b) const { return bin_op<rminus     , stdx::minus     >(*this, b); }
	DECL auto operator*(const B &b) const { return bin_op<rmultiplies, stdx::multiplies>(*this, b); }
	DECL auto operator/(const B &b) const { return bin_op<rdivides   , stdx::divides   >(*this, b); }
//	DECL auto operator%(const B &b) { return ; }
//	DECL auto operator&(const B &b) { return ; }
//	DECL auto operator|(const B &b) { return ; }
//	DECL auto operator^(const B &b) { return ; }

	DECL auto operator>>(const B &b) const { return bin_op<rshift_right, stdx::shift_right>(*this, b); }
	DECL auto operator<<(const B &b) const { return bin_op<rshift_left , stdx::shift_left >(*this, b); }

	DECL bool operator==(const B &b) const { return cmp_op<requal, stdx::equal_to>(*this, b); }
	DECL bool operator <(const B &b) const { return cmp_op<rless , stdx::less    >(*this, b); }

	DECL bool operator<=(const B &b) const { return *this == b || *this <  b; }
	DECL bool operator!=(const B &b) const { return !(*this == b); }
	DECL bool operator >(const B &b) const { return !(*this <= b); }
	DECL bool operator>=(const B &b) const { return !(*this <  b); }
#undef DECL

	template<int Er, class U = T> constexpr auto scale() const {
		auto res = ::scale<Er,U>(base);
		return ranged< decltype(res), ::scale<Er,U>(lo), ::scale<Er,U>(hi) >(res);
	}

	constexpr auto operator+() const { return ranged<decltype(+base), +lo, +hi>(+base); }
	constexpr auto operator-() const { return ranged<decltype(-base), -hi, -lo>(-base); }
//	constexpr auto operator~() const { return ; }
	//

	// These implement the remaining operators based on the previous ones
	template<class B> constexpr auto& operator+=(const B &b) { return *this = *this + b; }
	template<class B> constexpr auto& operator-=(const B &b) { return *this = *this - b; }
	template<class B> constexpr auto& operator*=(const B &b) { return *this = *this * b; }
	template<class B> constexpr auto& operator/=(const B &b) { return *this = *this / b; }
	template<class B> constexpr auto& operator%=(const B &b) { return *this = *this % b; }

	template<class B> constexpr auto& operator<<=(const B &b) { return *this = *this << b; }
	template<class B> constexpr auto& operator>>=(const B &b) { return *this = *this >> b; }

	constexpr auto& operator++()    { return *this += 1; }
	constexpr auto& operator--()    { return *this -= 1; }
	constexpr auto  operator++(int) { auto ret = *this; return *this += 1, ret; }
	constexpr auto  operator--(int) { auto ret = *this; return *this -= 1, ret; }
	//
private:
	static constexpr auto lo = LOW, hi = MAX;
	template<class A, A, A> friend struct ranged;

	// Binary Operator Range Operations
#define DECL template<class A, class B, class R> struct
	DECL rplus       { static constexpr R min = A::lo + B::lo, max = A::hi + B::hi; };
	DECL rminus      { static constexpr R min = A::lo - B::hi, max = A::hi - B::lo; };
	DECL rmultiplies {
		static constexpr R min = stdx::min(A::lo * B::lo, A::lo * B::hi, A::hi * B::lo, A::hi * B::hi),
		                   max = stdx::max(A::lo * B::lo, A::lo * B::hi, A::hi * B::lo, A::hi * B::hi);
	};
	DECL rdivides {
		static_assert(B::hi > typename B::base_type(0) || B::hi < typename B::base_type(0),
		              "divisor range may not contain zero");
		static constexpr R min = stdx::min(A::lo / B::lo, A::lo / B::hi, A::hi / B::lo, A::hi / B::hi),
		                   max = stdx::max(A::lo / B::lo, A::lo / B::hi, A::hi / B::lo, A::hi / B::hi);
	};
	DECL rshift_right {
		static_assert(B::lo >= typename B::base_type(0), "cannot shift right by negative number");
		static constexpr R min = A::lo >> B::hi, max = A::hi >> B::lo;
	};
	DECL rshift_left {
		static_assert(B::lo >= typename B::base_type(0), "cannot shift left by negative number");
		static constexpr R min = A::lo << B::lo, max = A::hi << B::hi;
	};
#undef DECL
	// Compare Range Operations
#define DECL template<class A, class B> struct
	DECL requal {
		static constexpr bool always_true  = A::lo == A::hi && B::lo == B::hi && A::lo == B::lo,
	                              always_false = A::lo > B::hi || A::hi < B::lo;
	};
	DECL rless  { static constexpr bool always_true = A::hi < A::lo, always_false = A::lo >= B::hi; };
#undef DECL
	//

	// Generic Binary Operator Implementation
	template<template<class,class> class BOP, class A, A A_LOW, A A_MAX, class B, B B_LOW, B B_MAX>
	static constexpr auto bin_op_raw(const ranged<A, A_LOW, A_MAX> &a, const ranged<B, B_LOW, B_MAX> &b) {
		return BOP<decltype(a.base), decltype(b.base)>()(a.base, b.base);
	}

	template<template<class,class,class> class ROP, template<class,class> class BOP,
		class A, A A_LOW, A A_MAX, class B, B B_LOW, B B_MAX>
	static constexpr auto bin_op(const ranged<A, A_LOW, A_MAX> &a, const ranged<B, B_LOW, B_MAX> &b) {
		auto res = bin_op_raw<BOP>(a, b);
		using rop = ROP<std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, decltype(res)>;
		return ranged<decltype(res), rop::min, rop::max>(res);
	}

	template<template<class,class> class ROP, template<class,class> class BOP,
		class A, A A_LOW, A A_MAX, class B, B B_LOW, B B_MAX>
	static constexpr bool cmp_op(const ranged<A, A_LOW, A_MAX> &a, const ranged<B, B_LOW, B_MAX> &b) {
		using rop = ROP<std::decay_t<decltype(a)>, std::decay_t<decltype(b)>>;
		return rop::always_true || (!rop::always_false && bin_op_raw<BOP>(a, b));
	}
	//

	base_type base;
};

namespace std {
template<class A, A A_LOW, A A_MAX> struct numeric_limits<ranged<A,A_LOW,A_MAX>> {
	using base = numeric_limits<A>;

	static constexpr bool is_specialized = base::is_specialized;

	static constexpr A min()    noexcept { return stdx::max(A_LOW, base::min()); }
	static constexpr A max()    noexcept { return A_MAX; }
	static constexpr A lowest() noexcept { return A_LOW; }

	static constexpr int  digits       = base::digits;
	static constexpr int  digits10     = base::digits10;
	static constexpr int  max_digits10 = base::max_digits10;
	static constexpr bool is_signed    = base::is_signed;
	static constexpr bool is_integer   = base::is_integer;
	static constexpr bool is_exact     = base::is_exact;
	static constexpr int  radix        = base::radix;

	static constexpr A epsilon()     noexcept { return base::epsilon(); }
	static constexpr A round_error() noexcept { return base::round_error(); }

	static constexpr int  min_exponent   = base::min_exponent;
	static constexpr int  min_exponent10 = base::min_exponent10;
	static constexpr int  max_exponent   = base::max_exponent;
	static constexpr int  max_exponent10 = base::max_exponent10;

	static constexpr bool has_infinity      = base::has_infinity;
	static constexpr bool has_quiet_NaN     = base::has_quiet_NaN;
	static constexpr bool has_signaling_NaN = base::has_signaling_NaN;
	static constexpr float_denorm_style has_denorm = base::has_denorm;
	static constexpr bool has_denorm_loss = base::has_denorm_loss;

	static constexpr A infinity()      noexcept { return base::infinity(); }
	static constexpr A quiet_NaN()     noexcept { return base::quiet_NaN(); }
	static constexpr A signaling_NaN() noexcept { return base::signaling_NaN(); }
	static constexpr A denorm_min()    noexcept { return base::denorm_min(); }

	static constexpr bool is_iec559  = base::is_iec559;
	static constexpr bool is_bounded = base::is_bounded;
	static constexpr bool is_modulo  = base::is_modulo;

	static constexpr bool traps = base::traps;
	static constexpr bool tinyness_before = base::tinyness_before;
	static constexpr float_round_style round_style = base::round_style;
};
}

template<class A, A A_LOW, A A_MAX> struct is_ranged<ranged<A, A_LOW, A_MAX>> : std::true_type {};
template<class A, A A_LOW, A A_MAX> struct underlying<ranged<A,A_LOW,A_MAX>> { using type = underlying_t<A>; };

#endif
