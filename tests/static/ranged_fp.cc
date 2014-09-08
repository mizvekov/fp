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

struct ranged_fp_test {
	template<class T, int E = 0,
		T LOW = std::numeric_limits<T>::lowest(),
		T MAX = std::numeric_limits<T>::max()>
		using fp_ranged = fp_t<ranged_t<T, LOW, MAX>, E>;

#define NL(x) std::numeric_limits<decltype(x)>

	static constexpr auto test1 = fp_ranged<int, 1, -20, 50>{8};
	static constexpr auto test2 = -test1;
	static_assert(int(NL(test2)::lowest()) == -50, "");
	static_assert(int(NL(test2)::max()) == 20, "");
	static_assert(int(test2) == -8, "");

	static constexpr auto test3 = test1 + test2;
	static_assert(int(NL(test3)::lowest()) == -70, "");
	static_assert(int(NL(test3)::max()) == 70, "");
	static_assert(test3 == fp_ranged<int>(0), "");
	static_assert(test3 >= fp_ranged<int>(0), "");

	static constexpr auto test4 = test1 - test2;
	static_assert(int(NL(test4)::lowest()) == -40, "");
	static_assert(int(NL(test4)::max()) == 100, "");
	static_assert(test4 == fp_ranged<int>(16), "");
	static_assert(test4 <= fp_ranged<int>(16), "");
	static_assert(test4 <= fp_ranged<int>(17), "");

	static constexpr auto test5 = test1 * test2;
	static_assert(int(NL(test5)::lowest()) == -2500, "");
	static_assert(int(NL(test5)::max()) == 1000, "");
	static_assert(test5 == fp_ranged<int>(-64), "");
	static_assert(test5 < fp_ranged<int>(-63), "");
	static_assert(test5 > fp_ranged<int>(-1251), "");
	static_assert(test5 < fp_ranged<int>(501), "");

	static constexpr auto test6 = fp_ranged<int, 1, 1, 20>{4};
	static constexpr auto test7 = test1 / test6;
	static_assert(int(NL(test7)::lowest()) == -20, "");
	static_assert(int(NL(test7)::max()) == 50, "");
	static_assert(test7 == fp_ranged<int>(2), "");
	static_assert(test7 != fp_ranged<int>(3), "");
	static_assert(test7 != fp_ranged<int>(50), "");
	static_assert(test7 != fp_ranged<int>(-20), "");
	static_assert(test7 != fp_ranged<int>(-21), "");
	static_assert(test7 > fp_ranged<int>(0), "");
	static_assert(test7 >= fp_ranged<int>(-20), "");
	static_assert(test7 <= fp_ranged<int>(50), "");

	static constexpr auto test8  = fp_ranged<unsigned, 1, 4096, 8192>{ 1024 };
	static constexpr auto test9  = ranged_t<unsigned, 0, 10>{ 5 };

	static constexpr auto test10 = test8 >> test9;
	static_assert(unsigned(NL(test10)::lowest()) == 4u, "");
	static_assert(unsigned(NL(test10)::max()) == 8192u, "");
	static_assert(test10 == fp_ranged<unsigned>(32u), "");

	static constexpr auto test11 = test8 << test9;
	static_assert(unsigned(NL(test11)::lowest()) == 4096u, "");
	static_assert(unsigned(NL(test11)::max()) == 8388608u, "");
	static_assert(test11 == fp_ranged<unsigned>(32768u), "");

	static constexpr auto test12 = fp_ranged<int8_t, 1, 10, 20>(2) + fp_ranged<int8_t, 4>(1);
	static_assert(int(NL(test12)::lowest()) == -118, "");
	static_assert(int(NL(test12)::max()) == 147, "");
	static_assert(test12 == fp_ranged<int>(3), "");

	static constexpr auto test13 = make_fp<2>(10) << ranged_t<int, 3, 3>{};
	static_assert(test13 == make_fp(80), "");
	static_assert(decltype(test13)::exp{} == -1, "");

#undef NL
};
