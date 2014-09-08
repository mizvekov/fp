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

#include <boost/multiprecision/cpp_bin_float.hpp>

namespace boost { namespace multiprecision {
	template<unsigned Digits, backends::digit_base_type base, class Allocator, class Exp, Exp Min, Exp Max, class T, T V>
	static auto operator<<(const number<cpp_bin_float<Digits,base,Allocator,Exp,Min,Max>> &a,
	                       const ::std::integral_constant<T,V> &) {
		return ldexp(a, +V);
	}

	template<unsigned Digits, backends::digit_base_type base, class Allocator, class Exp, Exp Min, Exp Max, class T, T V>
	static auto operator>>(const number<cpp_bin_float<Digits,base,Allocator,Exp,Min,Max>> &a,
	                       const ::std::integral_constant<T,V> &) {
		return ldexp(a, -V);
	}

	template<unsigned ADigits, backends::digit_base_type Abase, class AAllocator, class AExp, AExp AMin, AExp AMax,
	         unsigned BDigits, backends::digit_base_type Bbase, class BAllocator, class BExp, BExp BMin, BExp BMax>
	static auto operator%(const number<cpp_bin_float<ADigits,Abase,AAllocator,AExp,AMin,AMax>> &a,
	                      const number<cpp_bin_float<BDigits,Bbase,BAllocator,BExp,BMin,BMax>> &b) {
		return fmod(a, b);
	}
}; };
