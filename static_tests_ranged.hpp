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


#ifndef MIZVEKOV_RANGED_STATIC_TESTS_INCLUDE_HPP_INCLUDED
#define MIZVEKOV_RANGED_STATIC_TESTS_INCLUDE_HPP_INCLUDED

#include "ranged.hpp"
#include <cstdint>

struct ranged_test {
	static constexpr auto test1 = ranged<int, -20, 50>{3};

	static constexpr auto test2 = -test1;
	static_assert(test2.lowest() == -50, "");
	static_assert(test2.max() == 20, "");
	static_assert(int(test2) == -3, "");

	static constexpr auto test3 = test1 + test2;
	static_assert(test3.lowest() == -70, "");
	static_assert(test3.max() == 70, "");
	static_assert(test3 == ranged<int>(0), "");
	static_assert(test3 >= ranged<int>(0), "");

	static constexpr auto test4 = test1 - test2;
	static_assert(test4.lowest() == -40, "");
	static_assert(test4.max() == 100, "");
	static_assert(test4 == ranged<int>(6), "");
	static_assert(test4 <= ranged<int>(6), "");
	static_assert(test4 <= ranged<int>(7), "");

	static constexpr auto test5 = test1 * test2;
	static_assert(test5.lowest() == -2500, "");
	static_assert(test5.max() == 1000, "");
	static_assert(test5 == ranged<int>(-9), "");
	static_assert(test5 < ranged<int>(-8), "");
	static_assert(test5 > ranged<int>(-2501), "");
	static_assert(test5 < ranged<int>(1001), "");

	static constexpr auto test6 = ranged<int, 1, 20>{3};
	static constexpr auto test7 = test1 / test6;
	static_assert(test7.lowest() == -20, "");
	static_assert(test7.max() == 50, "");
	static_assert(test7 == ranged<int>(1), "");
	static_assert(test7 != ranged<int>(2), "");
	static_assert(test7 != ranged<int>(50), "");
	static_assert(test7 != ranged<int>(-20), "");
	static_assert(test7 != ranged<int>(-21), "");
	static_assert(test7 > ranged<int>(0), "");
	static_assert(test7 >= ranged<int>(-20), "");
	static_assert(test7 <= ranged<int>(50), "");

	static constexpr auto test8  = ranged<unsigned, 2048, 4096>{ 1024 };
	static constexpr auto test9  = ranged<unsigned, 0, 10>{ 5 };

	static constexpr auto test10 = test8 >> test9;
	static_assert(test10.lowest() == 2u, "");
	static_assert(test10.max() == 4096u, "");
	static_assert(test10 == ranged<unsigned>(32u), "");

	static constexpr auto test11 = test8 << test9;
	static_assert(test11.lowest() == 2048u, "");
	static_assert(test11.max() == 4194304u, "");
	static_assert(test11 == ranged<unsigned>(32768u), "");

	static constexpr auto test12 = ranged<int8_t, 10, 20>(2) + ranged<int8_t>(1);
	static_assert(test12.lowest() == -118, "");
	static_assert(test12.max() == 147, "");
	static_assert(test12 == ranged<int>(3), "");

	static constexpr auto test13 = ranged<int, 10, 20>{ ranged<int, 11, 19>{15} };
};

#endif
