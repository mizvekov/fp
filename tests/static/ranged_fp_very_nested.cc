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

#include <fp/fp.hpp>
#include "ranged.hpp"
#include <cstdint>

using namespace fp;
using namespace fp::constants;

struct totem_test {

#define NL(x) std::numeric_limits<decltype(x)>

	static constexpr auto test1 = fp_t<fp_t<fp_t<ranged_t<int64_t,-4096  ,4096  >,1>,2>,3>{ 2 };
	static constexpr auto test2 = fp_t<fp_t<fp_t<ranged_t<int64_t,-131072,131072>,2>,3>,4>{ 3 };
	static constexpr auto test3 = test1 * test2;

	static_assert(int(test1) == 2, "");
	static_assert(int(test2) == 3, "");
	static_assert(int64_t(test3) == 6, "");

	static_assert(int(NL(test1)::lowest()) == -4096, "");
	static_assert(int(NL(test1)::max()) == 4096, "");
	static_assert(int(NL(test2)::lowest()) == -131072, "");
	static_assert(int(NL(test2)::max()) == 131072, "");
	static_assert(int64_t(NL(test3)::lowest()) == -536870912, "");
	static_assert(int64_t(NL(test3)::max()) == +536870912, "");

	static_assert(decltype(test3)::exp{} == 7, "");
	static_assert(decltype(test3)::underlying_type::exp{} == 5, "");
	static_assert(decltype(test3)::underlying_type::underlying_type::exp{} == 3, "");

	static constexpr auto test4 = fp_t<fp_t<fp_t<fp_t<int,6>,1>,2>,3>{ 7.640625 };
	static constexpr auto test5 = fp_t<fp_t<fp_t<fp_t<int,6>,3>,2>,1>{ 3.203125 };
	static constexpr auto test6 = test4 + test5;
	static constexpr auto test7 = test4 * test5;
	static_assert(double(test6) == 10.84375, "");
	static_assert(double(test7) == 24.473876953125, "");

#undef NL
};
