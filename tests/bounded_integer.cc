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

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <fp/fp.hpp>
#include <bounded/integer.hpp>

TEST_CASE( "bounded integer tests", "[fp]" ) {
	using namespace bounded::literal;
	using namespace fp;
	using namespace fp::constants;

	{
		const auto x = make_fp<8>(17_bi) << int_<2>;
		REQUIRE( double(x) == 17*4 );
	}
	{
		auto x = fp_t<bounded::integer<1, 100>, 0>{  30_bi } >> int_<3>;
		auto y = fp_t<bounded::integer<2, 300>, 0>{ 150_bi } >> int_<4>;
		auto z = x + y;
		REQUIRE( double(z) == 13.125 );
		REQUIRE( double(std::numeric_limits<decltype(z)>::min()) == 0.25 );
		REQUIRE( double(std::numeric_limits<decltype(z)>::max()) == 31.25 );
	}
	{
		const auto x = make_fp<1>(150_bi);
		const auto y = make_fp<0>(60_bi);
		auto z = x + y;
		REQUIRE( double(z) == 210 );
		REQUIRE( double(std::numeric_limits<decltype(z)>::min()) == 210 );
		REQUIRE( double(std::numeric_limits<decltype(z)>::max()) == 210 );
	}
	{
		const auto x = make_fp<4>(150_bi);
		const auto y = make_fp<0>(60_bi);
		auto z = x / y;
		REQUIRE( double(z) == 2.5 );
	}
	{
		const auto x = make_fp<4>(10_bi);
		const auto y = make_fp<0>(20_bi);
		auto z = x * y;
		REQUIRE( double(z) == 200 );
	}
	{
		const auto x = make_fp<2>(17_bi) >> int_<2>;
		const auto y = make_fp<0>(1_bi);
		auto z = x % y;
		REQUIRE( double(z) == 0.25 );
	}
	{
		const auto x = make_fp<8>(17_bi) << int_<2>;
		const auto y = make_fp<0>(25_bi) >> int_<3>;
		auto z = x / y;
		REQUIRE( double(z) == 21.75 );
	}
	{
//		const fp_t<bounded::integer<170, 200>, 2> x = 180_bi;
//		const fp_t<bounded::integer<140, 160>, 3> y = 150_bi;
//		auto z = x - y;
//		REQUIRE( z == make_fp(30_bi) );
	}
}
