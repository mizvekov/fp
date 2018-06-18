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
