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

#include "ranged.hpp"
#include <cstdint>

using namespace fp::constants;

struct ranged_test {
#define NL(x) std::numeric_limits<decltype(x)>

	static constexpr auto test1 = ranged_t<int, -20, 50>{3};
	static constexpr auto test2 = -test1;
	static_assert(int(NL(test2)::lowest()) == -50, "");
	static_assert(int(NL(test2)::max()) == 20, "");
	static_assert(int(test2) == -3, "");

	static constexpr auto test3 = test1 + test2;
	static_assert(int(NL(test3)::lowest()) == -70, "");
	static_assert(int(NL(test3)::max()) == 70, "");
	static_assert(test3 == ranged_t<int>(0), "");
	static_assert(test3 >= ranged_t<int>(0), "");

	static constexpr auto test4 = test1 - test2;
	static_assert(int(NL(test4)::lowest()) == -40, "");
	static_assert(int(NL(test4)::max()) == 100, "");
	static_assert(test4 == ranged_t<int>(6), "");
	static_assert(test4 <= ranged_t<int>(6), "");
	static_assert(test4 <= ranged_t<int>(7), "");

	static constexpr auto test5 = test1 * test2;
	static_assert(int(NL(test5)::lowest()) == -2500, "");
	static_assert(int(NL(test5)::max()) == 1000, "");
	static_assert(test5 == ranged_t<int>(-9), "");
	static_assert(test5 < ranged_t<int>(-8), "");
	static_assert(test5 > ranged_t<int>(-2501), "");
	static_assert(test5 < ranged_t<int>(1001), "");

	static constexpr auto test6 = ranged_t<int, 1, 20>{3};
	static constexpr auto test7 = test1 / test6;
	static_assert(int(NL(test7)::lowest()) == -20, "");
	static_assert(int(NL(test7)::max()) == 50, "");
	static_assert(test7 == ranged_t<int>(1), "");
	static_assert(test7 != ranged_t<int>(2), "");
	static_assert(test7 != ranged_t<int>(50), "");
	static_assert(test7 != ranged_t<int>(-20), "");
	static_assert(test7 != ranged_t<int>(-21), "");
	static_assert(test7 > ranged_t<int>(0), "");
	static_assert(test7 >= ranged_t<int>(-20), "");
	static_assert(test7 <= ranged_t<int>(50), "");

	static constexpr auto test8  = ranged_t<unsigned, 2048, 4096>{ 1024 };
	static constexpr auto test9  = ranged_t<unsigned, 0, 10>{ 5 };

	static constexpr auto test10 = test8 >> test9;
	static_assert(unsigned(NL(test10)::lowest()) == 2u, "");
	static_assert(unsigned(NL(test10)::max()) == 4096u, "");
	static_assert(test10 == ranged_t<unsigned>(32u), "");

	static constexpr auto test11 = test8 << test9;
	static_assert(unsigned(NL(test11)::lowest()) == 2048u, "");
	static_assert(unsigned(NL(test11)::max()) == 4194304u, "");
	static_assert(test11 == ranged_t<unsigned>(32768u), "");

	static constexpr auto test12 = ranged_t<int8_t, 10, 20>(2) + ranged_t<int8_t>(1);
	static_assert(unsigned(NL(test12)::lowest()) == -118, "");
	static_assert(unsigned(NL(test12)::max()) == 147, "");
	static_assert(test12 == ranged_t<int>(3), "");

	static constexpr auto test13 = ranged_t<int, 10, 20>{ ranged_t<int, 11, 19>{15} };

	static constexpr auto test14 = test8 + uint_<5>;
	static_assert(unsigned(NL(test14)::lowest()) == 2053u, "");
	static_assert(unsigned(NL(test14)::max()) == 4101u, "");
	static_assert(test14 == ranged_t<unsigned>(1029u), "");

	static constexpr auto test15 = test8 >> int_<5>;
	static_assert(unsigned(NL(test15)::lowest()) == 64u, "");
	static_assert(unsigned(NL(test15)::max()) == 128u, "");
	static_assert(test15 == ranged_t<unsigned>(32u), "");

#undef NL
};
