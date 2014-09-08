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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <fp/fp.hpp>
#include <bounded_integer/bounded_integer.hpp>

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
		const fp_t<bounded::integer<170, 200>, 2> x = 180_bi;
		const fp_t<bounded::integer<140, 160>, 3> y = 150_bi;
		auto z = x - y;
		REQUIRE( z == make_fp(30_bi) );
	}
}
