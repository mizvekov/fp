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

#ifndef MIZVEKOV_FP_INCLUDE_HPP_INCLUDED
#define MIZVEKOV_FP_INCLUDE_HPP_INCLUDED

#include <functional>
#include <type_traits>
#include <cmath>

#include "common.hpp"

template<class> struct is_fp : std::false_type {};

template<class T, int E = 0> struct fp : std::numeric_limits<fp<T,E>> {
	using base_type = T;
	static constexpr auto exp = E;

	static_assert(std::numeric_limits<T>::is_specialized, "base_type must specialize std::numeric_limits");

	// Public Constructors
	fp() = default;
	constexpr fp(const T &b, bool) : base(b) {}
	template<class B>         constexpr fp(const B        &b) : base(::scale<E     ,T>(b     )) {}
	template<class B, int Eb> constexpr fp(const fp<B,Eb> &b) : base(::scale<E - Eb,T>(b.base)) {}
	//

	template<int Er,class=void> constexpr auto scale() const { return fp<T, E - Er>(base, {}); }

	// Operators Implementation
	template<class U, class = std::enable_if_t< !is_fp<U>{} >>
	explicit constexpr operator U() const { return fp<U,0>(*this).base; }

	template<class B> constexpr auto& operator =(const B &b) { return base = fp<T,E>(b).base, *this; }

#define DECL template<class B, class = std::enable_if_t< is_fp<B>{} > > constexpr
	DECL auto operator+(const B &b) const { return bin_op<emax,stdx::plus      >(*this, b); }
	DECL auto operator-(const B &b) const { return bin_op<emax,stdx::minus     >(*this, b); }
	DECL auto operator*(const B &b) const { return bin_op<esum,stdx::multiplies>(*this, b); }
	DECL auto operator/(const B &b) const { return bin_op<esub,stdx::divides   >(*this, b); }
	DECL auto operator%(const B &b) const { return bin_op<emax,stdx::modulus   >(*this, b); }
	DECL auto operator&(const B &b) const { return bin_op<emax,stdx::bit_and   >(*this, b); }
	DECL auto operator|(const B &b) const { return bin_op<emax,stdx::bit_or    >(*this, b); }
	DECL auto operator^(const B &b) const { return bin_op<emax,stdx::bit_xor   >(*this, b); }

	DECL bool operator==(const B &b) const { return bin_op_raw<emin,stdx::equal_to>(*this, b); }
	DECL bool operator <(const B &b) const { return bin_op_raw<emin,stdx::less    >(*this, b); }

	DECL bool operator<=(const B &b) const { return *this == b || *this <  b; }
	DECL bool operator!=(const B &b) const { return !(*this == b); }
	DECL bool operator >(const B &b) const { return !(*this <= b); }
	DECL bool operator>=(const B &b) const { return !(*this <  b); }

	template<class B> constexpr auto operator<<(const B &b) const { return make_fp_raw<E>(base << b); }
	template<class B> constexpr auto operator>>(const B &b) const { return make_fp_raw<E>(base >> b); }

	constexpr auto operator+() const { return make_fp_raw<E>(+base); }
	constexpr auto operator-() const { return make_fp_raw<E>(-base); }
	constexpr auto operator~() const { return make_fp_raw<E>(~base); }
	//

	// These implement the remaining operators based on the previous ones
	DECL auto& operator+=(const B &b) { return *this = *this + b; }
	DECL auto& operator-=(const B &b) { return *this = *this - b; }
	DECL auto& operator*=(const B &b) { return *this = *this * b; }
	DECL auto& operator/=(const B &b) { return *this = *this / b; }
	DECL auto& operator%=(const B &b) { return *this = *this % b; }

	template<class B> constexpr auto& operator<<=(const B &b) { return *this = *this << b; }
	template<class B> constexpr auto& operator>>=(const B &b) { return *this = *this >> b; }
#undef DECL

	constexpr auto& operator++()    { return *this += fp<T,0>(1); }
	constexpr auto& operator--()    { return *this -= fp<T,0>(1); }
	constexpr auto  operator++(int) { auto ret = *this; return ++*this, ret; }
	constexpr auto  operator--(int) { auto ret = *this; return ++*this, ret; }
	//
private:
	template<class, int> friend struct fp;
	friend std::numeric_limits<fp>;

	// Exponent Operations
	template<int Ea, int Eb> struct emax { enum { e = stdx::max(Ea,Eb), ea = e , eb = e  }; };
	template<int Ea, int Eb> struct emin { enum { e = stdx::min(Ea,Eb), ea = e , eb = e  }; };
	template<int Ea, int Eb> struct esum { enum { e = Ea + Eb         , ea = Ea, eb = Eb }; };
	template<int Ea, int Eb> struct esub { enum { e = Ea - Eb         , ea = Ea, eb = Eb }; };
	//

	template<int Ea, class A> static constexpr fp<A,Ea> make_fp_raw(const A &a) { return { a, {} }; }
	template<class A> using promote = decltype(+A());

	// Generic Binary Operator Implementation
	template<template<int,int> class EOP, template<class,class> class BOP, class A, int Ea, class B, int Eb>
	static constexpr auto bin_op_raw(const fp<A,Ea> &a, const fp<B,Eb> &b) {
		auto sa = ::scale<EOP<Ea,Eb>::ea - Ea, promote<A>>(a.base);
		auto sb = ::scale<EOP<Ea,Eb>::eb - Eb, promote<A>>(b.base);
		return BOP<decltype(sa),decltype(sb)>()(sa, sb);
	}
	template<template<int,int> class EOP, template<class,class> class BOP, class A, int Ea, class B, int Eb>
	static constexpr auto bin_op(const fp<A,Ea> &a, const fp<B,Eb> &b) {
		return make_fp_raw<EOP<Ea,Eb>::e>(bin_op_raw<EOP,BOP>(a, b));
	}
	//

	base_type base;
};

namespace std {
template<class T, int E> struct numeric_limits<fp<T,E>> {
	using type = fp<T,E>;
	using base = numeric_limits<T>;

	static constexpr bool is_specialized = base::is_specialized;

	static constexpr type min()    noexcept { return { base::min()   , {} }; }
	static constexpr type max()    noexcept { return { base::max()   , {} }; }
	static constexpr type lowest() noexcept { return { base::lowest(), {} }; }

	static constexpr int  digits       = base::digits;
	static constexpr int  digits10     = base::digits10;
	static constexpr int  max_digits10 = base::max_digits10;
	static constexpr bool is_signed    = base::is_signed;
	static constexpr bool is_integer   = base::is_integer;
	static constexpr bool is_exact     = base::is_exact;
	static constexpr int  radix        = base::radix;

	static constexpr type epsilon()     noexcept { return { base::epsilon()    , {} }; }
	static constexpr type round_error() noexcept { return { base::round_error(), {} }; }

	static constexpr int  min_exponent   = base::min_exponent;
	static constexpr int  min_exponent10 = base::min_exponent10;
	static constexpr int  max_exponent   = base::max_exponent;
	static constexpr int  max_exponent10 = base::max_exponent10;

	static constexpr bool has_infinity      = base::has_infinity;
	static constexpr bool has_quiet_NaN     = base::has_quiet_NaN;
	static constexpr bool has_signaling_NaN = base::has_signaling_NaN;
	static constexpr float_denorm_style has_denorm = base::has_denorm;
	static constexpr bool has_denorm_loss = base::has_denorm_loss;

	static constexpr type infinity()      noexcept { return { base::infinity()     , {} }; }
	static constexpr type quiet_NaN()     noexcept { return { base::quiet_NaN()    , {} }; }
	static constexpr type signaling_NaN() noexcept { return { base::signaling_NaN(), {} }; }
	static constexpr type denorm_min()    noexcept { return { base::denorm_min()   , {} }; }

	static constexpr bool is_iec559  = base::is_iec559;
	static constexpr bool is_bounded = base::is_bounded;
	static constexpr bool is_modulo  = base::is_modulo;

	static constexpr bool traps = base::traps;
	static constexpr bool tinyness_before = base::tinyness_before;
	static constexpr float_round_style round_style = base::round_style;
};
}

template<class T, int E> struct is_fp<fp<T, E>> : std::true_type {};
template<class T, int E> struct underlying<fp<T,E>> { using type = underlying_t<T>; };

#endif
