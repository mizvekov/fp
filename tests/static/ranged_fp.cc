/*
 * Copyright (c) 2018, Matheus Izvekov <mizvekov@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
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

#undef NL
};
