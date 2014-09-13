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

#pragma once

#include <functional>
#include <type_traits>
#include <limits>
#include <algorithm>

namespace fp {

namespace detail {
	// exp2 metafunction
	template<int I, class A> struct mexp2 {
		static constexpr A value = A(I > 0 ? 2.0 : 0.5) * mexp2<I + (I > 0 ? -1 : 1), A>::value;
	};
	template<class A> struct mexp2<0,A> { static constexpr A value = 1.0; };
	//
	template<class T, class B, B V>
	constexpr auto builtin_const_shift(const std::enable_if_t<std::is_integral<T>{},T> &a) {
		return V > 0 ? a * mexp2<V,T>::value : a / mexp2<-V,T>::value;
	}
	template<class T, class B, B V>
	constexpr auto builtin_const_shift(const std::enable_if_t<std::is_floating_point<T>{},T> &a) {
		return a * mexp2<V,T>::value;
	}
};

namespace operators { // constant shift for builtin arithmetic types
	template<class T, class B, B V, class = std::enable_if_t<std::is_arithmetic<T>{}>>
	constexpr auto operator<<(const T &a, const std::integral_constant<B,V> &b) {
		return detail::builtin_const_shift<T,B,+V>(a);
	}
	template<class T, class B, B V, class = std::enable_if_t<std::is_arithmetic<T>{}>>
	constexpr auto operator>>(const T &a, const std::integral_constant<B,V> &b) {
		return detail::builtin_const_shift<T,B,-V>(a);
	}
};

namespace detail {
	using namespace operators;

	template<intmax_t B, class T>
	static constexpr auto const_shift(const T &a, ::std::enable_if_t<B >= 0>* = 0) {
		return a << ::std::integral_constant<decltype(B), B>{};
	}
	template<intmax_t B, class T>
	static constexpr auto const_shift(const T &a, ::std::enable_if_t<B <  0>* = 0) {
		return a >> ::std::integral_constant<decltype(B), -B>{};
	}
};

namespace detail {
	template<class       > struct is_std_int_const                                : ::std::false_type {};
	template<class T, T V> struct is_std_int_const<::std::integral_constant<T,V>> : ::std::true_type  {};
};

namespace constants {
	template<         int I> constexpr auto  int_ = std::integral_constant<         int, I>{};
	template<unsigned int I> constexpr auto uint_ = std::integral_constant<unsigned int, I>{};
};

template<class> struct is_fp : std::false_type {};

template<class T, intmax_t E = 0, class BASE = decltype(detail::const_shift<E>(T{}))> struct fp_t {
	using underlying_type = T;
	using base_type = BASE;
	using exp = std::integral_constant<decltype(E), E>;

	// Public Constructors
	fp_t() = default;

	constexpr fp_t(const base_type &b, bool): base(b) {}

	template<class B> constexpr fp_t(const B &b, std::enable_if_t<!is_fp<B>{}>* = 0) :
		base(detail::const_shift<E>(b)) {}

	template<class B>
	constexpr fp_t(const B &b, std::enable_if_t< is_fp<B>{}>* = 0) :
		base(detail::const_shift<E - typename B::exp{}>(b.base)) {}
	//

	// Operators Implementation
	template<class U, class = std::enable_if_t< !is_fp<U>{} >>
	explicit constexpr operator U() const { return detail::const_shift<-E>(U(base)); }

	template<class B> constexpr auto& operator =(const B &b) { return base = fp_t<T,E>(b).base, *this; }

#define DECL template<class B, class = std::enable_if_t< is_fp<B>{} > > constexpr
	DECL auto operator+(const B &b) const { return bin_op<emax,std::plus      <>>(*this, b); }
	DECL auto operator-(const B &b) const { return bin_op<emax,std::minus     <>>(*this, b); }
	DECL auto operator*(const B &b) const { return bin_op<esum,std::multiplies<>>(*this, b); }
	DECL auto operator/(const B &b) const { return bin_op<esub,std::divides   <>>(*this, b); }
	DECL auto operator%(const B &b) const { return bin_op<emax,std::modulus   <>>(*this, b); }
	DECL auto operator&(const B &b) const { return bin_op<emax,std::bit_and   <>>(*this, b); }
	DECL auto operator|(const B &b) const { return bin_op<emax,std::bit_or    <>>(*this, b); }
	DECL auto operator^(const B &b) const { return bin_op<emax,std::bit_xor   <>>(*this, b); }

	DECL bool operator==(const B &b) const {
		return bin_op_raw<emin,std::equal_to<>,E,typename B::exp{}>(base, b.base);
	}
	DECL bool operator <(const B &b) const {
		return bin_op_raw<emin,std::less    <>,E,typename B::exp{}>(base, b.base);
	}

	// These implement the remaining operators based on the previous ones
	DECL bool operator<=(const B &b) const { return *this == b || *this <  b; }
	DECL bool operator!=(const B &b) const { return !(*this == b); }
	DECL bool operator >(const B &b) const { return !(*this <= b); }
	DECL bool operator>=(const B &b) const { return !(*this <  b); }

	DECL auto& operator+=(const B &b) { return *this = *this + b; }
	DECL auto& operator-=(const B &b) { return *this = *this - b; }
	DECL auto& operator*=(const B &b) { return *this = *this * b; }
	DECL auto& operator/=(const B &b) { return *this = *this / b; }
	DECL auto& operator%=(const B &b) { return *this = *this % b; }
#undef DECL

	template<class B> constexpr auto& operator<<=(const B &b) { return *this = *this << b; }
	template<class B> constexpr auto& operator>>=(const B &b) { return *this = *this >> b; }

	constexpr auto& operator++()    { return *this += fp_t<T,0>(1); }
	constexpr auto& operator--()    { return *this -= fp_t<T,0>(1); }
	constexpr auto  operator++(int) { auto ret = *this; return ++*this, ret; }
	constexpr auto  operator--(int) { auto ret = *this; return ++*this, ret; }
	//

#define DECL template<class B, std::enable_if_t< detail::is_std_int_const<B>{}>* = nullptr> constexpr
	DECL auto operator<<(const B & ) const { return make<E - B{}>(base     ); }
	DECL auto operator>>(const B & ) const { return make<E + B{}>(base     ); }
#undef  DECL
#define DECL template<class B, std::enable_if_t<!detail::is_std_int_const<B>{}>* = nullptr> constexpr
	DECL auto operator<<(const B &b) const { return make<E      >(base << b); }
	DECL auto operator>>(const B &b) const { return make<E      >(base >> b); }
#undef  DECL

	constexpr auto operator+() const { return make<E>(+base); }
	constexpr auto operator-() const { return make<E>(-base); }
	constexpr auto operator~() const { return make<E>(~base); }
	//
private:
	template<class, intmax_t, class> friend struct fp_t;
	friend std::numeric_limits<fp_t>;

	template<intmax_t Ea, class A> static constexpr auto make(const A &a) {
		return fp_t<decltype(detail::const_shift<-Ea>(a)), Ea, A>{ a, false };
	}

	// Exponent Operations
	template<intmax_t Ea, intmax_t Eb> struct emax { enum { e = std::max(Ea,Eb), ea = e , eb = e  }; };
	template<intmax_t Ea, intmax_t Eb> struct emin { enum { e = std::min(Ea,Eb), ea = e , eb = e  }; };
	template<intmax_t Ea, intmax_t Eb> struct esum { enum { e = Ea + Eb        , ea = Ea, eb = Eb }; };
	template<intmax_t Ea, intmax_t Eb> struct esub { enum { e = Ea - Eb        , ea = Ea, eb = Eb }; };
	//

	// Generic Binary Operator Implementation
	template<template<intmax_t,intmax_t> class EOP, class BOP,
		intmax_t Ea, intmax_t Eb, class A, class B>
	static constexpr auto bin_op_raw(const A &a, const B &b) {
		return BOP{}(
			detail::const_shift<EOP<Ea,Eb>::ea - Ea>(a),
			detail::const_shift<EOP<Ea,Eb>::eb - Eb>(b)
		);
	}
	template<template<intmax_t,intmax_t> class EOP, class BOP, class A, class B>
	static constexpr auto bin_op(const A &a, const B &b) {
		return make<EOP<typename A::exp{}, typename B::exp{}>::e>(
			bin_op_raw<EOP,BOP, typename A::exp{}, typename B::exp{}>(a.base, b.base)
		);
	}
	//

	base_type base;
};

template<class T, intmax_t E, class BASE> struct is_fp<fp_t<T,E,BASE>> : std::true_type {};

template<class T, intmax_t E> static constexpr auto make_fp(const fp_t<T,E> &a) { return a; }
template<int E = 0, class T> static constexpr fp_t<T,E> make_fp(const T &a) { return a; }

}; //namespace fp

namespace std {

template<class T, int E, class BASE> struct numeric_limits<fp::fp_t<T,E,BASE>> {
	using type = fp::fp_t<T,E,BASE>;
	using base = numeric_limits<BASE>;

	static constexpr bool is_specialized = base::is_specialized;

	static constexpr type min()    noexcept { return { base::min()   , false }; }
	static constexpr type max()    noexcept { return { base::max()   , false }; }
	static constexpr type lowest() noexcept { return { base::lowest(), false }; }

	static constexpr int  digits       = base::digits;
	static constexpr int  digits10     = base::digits10;
	static constexpr int  max_digits10 = base::max_digits10;
	static constexpr bool is_signed    = base::is_signed;
	static constexpr bool is_integer   = base::is_integer;
	static constexpr bool is_exact     = base::is_exact;
	static constexpr int  radix        = base::radix;

	static constexpr type epsilon()     noexcept { return { base::epsilon()    , false }; }
	static constexpr type round_error() noexcept { return { base::round_error(), false }; }

	static constexpr int  min_exponent   = base::min_exponent;
	static constexpr int  min_exponent10 = base::min_exponent10;
	static constexpr int  max_exponent   = base::max_exponent;
	static constexpr int  max_exponent10 = base::max_exponent10;

	static constexpr bool has_infinity      = base::has_infinity;
	static constexpr bool has_quiet_NaN     = base::has_quiet_NaN;
	static constexpr bool has_signaling_NaN = base::has_signaling_NaN;
	static constexpr float_denorm_style has_denorm = base::has_denorm;
	static constexpr bool has_denorm_loss = base::has_denorm_loss;

	static constexpr type infinity()      noexcept { return { base::infinity()     , false }; }
	static constexpr type quiet_NaN()     noexcept { return { base::quiet_NaN()    , false }; }
	static constexpr type signaling_NaN() noexcept { return { base::signaling_NaN(), false }; }
	static constexpr type denorm_min()    noexcept { return { base::denorm_min()   , false }; }

	static constexpr bool is_iec559  = base::is_iec559;
	static constexpr bool is_bounded = base::is_bounded;
	static constexpr bool is_modulo  = base::is_modulo;

	static constexpr bool traps = base::traps;
	static constexpr bool tinyness_before = base::tinyness_before;
	static constexpr float_round_style round_style = base::round_style;
};

};
