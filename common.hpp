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

#ifndef MIZVEKOV_COMMON_INCLUDE_HPP_INCLUDED
#define MIZVEKOV_COMMON_INCLUDE_HPP_INCLUDED

namespace stdx {
#define DECL template<class T, class U>
	DECL struct plus       { constexpr auto operator()(const T& x, const U& y) { return x + y; } };
	DECL struct minus      { constexpr auto operator()(const T& x, const U& y) { return x - y; } };
	DECL struct multiplies { constexpr auto operator()(const T& x, const U& y) { return x * y; } };
	DECL struct divides    { constexpr auto operator()(const T& x, const U& y) { return x / y; } };
	DECL struct modulus    { constexpr auto operator()(const T& x, const U& y) { return x % y; } };
	DECL struct bit_and    { constexpr auto operator()(const T& x, const U& y) { return x & y; } };
	DECL struct bit_or     { constexpr auto operator()(const T& x, const U& y) { return x | y; } };
	DECL struct bit_xor    { constexpr auto operator()(const T& x, const U& y) { return x ^ y; } };

	DECL struct shift_right { constexpr auto operator()(const T& x, const U& y) { return x >> y; } };
	DECL struct shift_left  { constexpr auto operator()(const T& x, const U& y) { return x << y; } };

	DECL struct equal_to { constexpr auto operator()(const T& x, const U& y) { return x == y; } };
	DECL struct less     { constexpr auto operator()(const T& x, const U& y) { return x <  y; } };
#undef DECL

	template<class A> static constexpr A max(A a) { return a; }
	template<class A, class ...Args>
	static constexpr A max(A a, A b, Args ...args) { return max(a >= b ? a : b, args...); }

	template<class A> static constexpr A min(A a) { return a; }
	template<class A, class ...Args>
	static constexpr A min(A a, A b, Args ...args) { return min(a <= b ? a : b, args...); }
};

// exp2 metafunction
template<int I, class A> struct mexp2 {
	static constexpr A value = A(I > 0 ? 2.0 : 0.5) * mexp2<I + (I > 0 ? -1 : 1), A>::value;
};
template<class A> struct mexp2<0,A> { static constexpr A value = 1.0; };
//

// helpers for getting to the bottomost base_type
template<class T> struct underlying { using type = T; };
template<class T> using underlying_t = typename underlying<T>::type;
//

// scale overloads
template<int E, class U, class A> static constexpr auto scale(const A &a,
	std::enable_if_t<std::numeric_limits<A>::is_specialized && std::is_class<A>{}>* = 0)
{ return a.template scale<E,U>(); }

template<int E, class U, class A, class T = decltype(A() + underlying_t<U>())>
static constexpr auto scale(const A &a, std::enable_if_t<!std::is_class<A>{} && std::is_integral<T>{}>* = 0)
{ return E > 0 ? T(a) * mexp2<E,T>::value : T(a) / mexp2<-E,T>::value; }
//{ return E > 0 ? T(a) << E : T(a) >> -E; }

template<int E, class U, class A, class T = decltype(A() + underlying_t<U>())>
static constexpr auto scale(const A &a, std::enable_if_t<!std::is_class<A>{} && std::is_floating_point<T>{}>* = 0)
{ return T(a) * mexp2<E,T>::value; }
//

#endif
