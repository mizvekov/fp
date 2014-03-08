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

template<class T, int E = 0> struct fp {
	using base_type = T;
	using promoted_type = decltype(+T());
	static constexpr auto exp = E;

	static_assert(std::is_integral<base_type>{}, "base_type must be integral");

	// Public Constructors
	fp() = default;
	constexpr fp(const T &b, bool) : base(b) {}
	template<class B>         constexpr fp(const B        &b) : base(scale<T,E     >(b     )) {}
	template<class B, int Eb> constexpr fp(const fp<B,Eb> &b) : base(scale<T,E - Eb>(b.base)) {}
	//

	template<int Er> constexpr auto vshift() const { return fp<T, E - Er>(base, {}); }

	// Operators Implementation
	template<class U, class = std::enable_if_t< std::is_arithmetic<U>{} >>
	constexpr operator U() const { return scale<U,-E>(base); }

	template<class B> constexpr auto& operator =(const B &b) { return base = fp<T,E>(b).base, *this; }

#define DECL template<class A, class B> friend constexpr
	DECL auto operator+(const A &a, const B &b) { return bin_op<max,std::plus      >(proxy<B>(a), proxy<A>(b)); }
	DECL auto operator-(const A &a, const B &b) { return bin_op<max,std::minus     >(proxy<B>(a), proxy<A>(b)); }
	DECL auto operator*(const A &a, const B &b) { return bin_op<sum,std::multiplies>(proxy<B>(a), proxy<A>(b)); }
	DECL auto operator/(const A &a, const B &b) { return bin_op<sub,std::divides   >(proxy<B>(a), proxy<A>(b)); }
	DECL auto operator%(const A &a, const B &b) { return bin_op<max,std::modulus   >(proxy<B>(a), proxy<A>(b)); }
	DECL auto operator&(const A &a, const B &b) { return bin_op<max,std::bit_and   >(proxy<B>(a), proxy<A>(b)); }
	DECL auto operator|(const A &a, const B &b) { return bin_op<max,std::bit_or    >(proxy<B>(a), proxy<A>(b)); }
	DECL auto operator^(const A &a, const B &b) { return bin_op<max,std::bit_xor   >(proxy<B>(a), proxy<A>(b)); }

	DECL bool operator==(const A &a, const B &b) { return bin_op_raw<min,std::equal_to>(proxy<B>(a), proxy<A>(b)); }
	DECL bool operator <(const A &a, const B &b) { return bin_op_raw<min,std::less    >(proxy<B>(a), proxy<A>(b)); }

	DECL bool operator<=(const A &a, const B &b) { return a == b || a <  b; }
	DECL bool operator!=(const A &a, const B &b) { return !(a == b); }
	DECL bool operator >(const A &a, const B &b) { return !(a <= b); }
	DECL bool operator>=(const A &a, const B &b) { return !(a <  b); }
#undef DECL

	template<class B> constexpr auto operator<<(const B &b) const { return fp<promoted_type,E>(base << b, {}); }
	template<class B> constexpr auto operator>>(const B &b) const { return fp<promoted_type,E>(base >> b, {}); }

	constexpr auto operator+() const { return fp<promoted_type,E>(+base, {}); }
	constexpr auto operator-() const { return fp<promoted_type,E>(-base, {}); }
	constexpr auto operator~() const { return fp<promoted_type,E>(~base, {}); }
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
	template<class A, int Ea> friend struct fp;

	template<class A, class B> using conv_type = decltype(A() + B());

	// Exponent Operations
	template<int Ea, int Eb> struct max { enum { e = Ea > Eb ? Ea : Eb, ea = e - Ea, eb = e - Eb }; };
	template<int Ea, int Eb> struct min { enum { e = Ea < Eb ? Ea : Eb, ea = e - Ea, eb = e - Eb }; };
	template<int Ea, int Eb> struct sum { enum { e = Ea + Eb, ea = 0, eb = 0 }; };
	template<int Ea, int Eb> struct sub { enum { e = Ea - Eb, ea = 0, eb = 0 }; };
	//

	// proxy overloads
	template<class B, class A> static constexpr fp<typename B::base_type,B::exp> proxy(const A &a) { return a; }
	template<class B, class A, int Ea> static constexpr const auto& proxy(const fp<A,Ea> &a) { return a; }
	//

	// exp2 metafunction
	template<int I, typename Ta> struct exp2 {
		static constexpr Ta value = Ta(I > 0 ? 2.0 : 0.5) * exp2<I + (I > 0 ? -1 : 1), Ta>::value;
	};
	template<typename Ta> struct exp2<0, Ta> { static constexpr Ta value = 1.0; };
	//

	// Scaling overload set
	template<class U, int Er, class Ta, class Tb = conv_type<Ta,U>>
	static constexpr std::enable_if_t<std::is_integral<U>{}, U>
	scale(const Ta &a, std::enable_if_t<std::is_integral<Tb>{}>* = 0)
	{ return Er > 0 ? Tb(a) << Er : Tb(a) >> -Er; }

	template<class U, int Er, class Ta, class Tb = conv_type<Ta,U>>
	static constexpr std::enable_if_t<std::is_floating_point<U>{}, U>
	scale(const Ta &a, std::enable_if_t<std::is_floating_point<Tb>{}>* = 0)
	{ return Tb(a) * exp2<Er,Tb>::value; }

	// Optional rounding should be performed here, but unfortunately std::lround is not mandated to be constexpr.
	template<class U, int Er, class Ta, class Tb = conv_type<Ta,U>>
	static constexpr std::enable_if_t<std::is_integral<U>{}, U>
	scale(const Ta &a, std::enable_if_t<std::is_floating_point<Tb>{}>* = 0)
	{ return /*std::llround*/(scale<Tb,Er>(a)); }
	//

	// Generic Binary Operator Implementation
	template<template<int,int> class EOP, template<class> class BOP, class A, int Ea, class B, int Eb>
	static constexpr auto bin_op_raw(const fp<A,Ea> &a, const fp<B,Eb> &b) {
		using rt = conv_type<A, B>;
		return BOP<rt>()(scale<rt,EOP<Ea,Eb>::ea>(a.base), scale<rt,EOP<Ea,Eb>::eb>(b.base));
	}
	template<template<int,int> class EOP, template<class> class BOP, class A, int Ea, class B, int Eb>
	static constexpr auto bin_op(const fp<A,Ea> &a, const fp<B,Eb> &b) {
		using rt = conv_type<A, B>;
		return fp<rt, EOP<Ea,Eb>::e>(bin_op_raw<EOP,BOP>(a, b), {});
	}
	//

	base_type base;
};

#endif
