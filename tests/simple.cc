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

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

TEST_CASE( "fp tests", "[fp]" ) {
	using namespace fp;
	using namespace fp::constants;

	SECTION( "simple operations part 1" ) {
		fp_t<uint16_t,8> a = 1.25;

		REQUIRE( double(a) == 1.250 );

		a += fp_t<uint16_t,8>(0.25);
		REQUIRE( double(a) == 1.500 );

		a += fp_t<uint16_t,4>(0.125);
		REQUIRE( double(a) == 1.625 );

		--a;
		REQUIRE( double(a) == 0.625 );
	}

	SECTION( "simple operations part 2" ) {
		fp_t<int16_t,8> b;
		b = 0.75;
		REQUIRE( double(b) == 0.750 );

		b -= fp_t<int16_t,8>(1.25);
		REQUIRE( double(b) == -0.500 );

		b++;
		REQUIRE( double(b) == 0.500 );
	}

	SECTION( "multiplication" ) {
		fp_t<uint16_t,8> a = 0.625;
		fp_t<uint16_t,8> b = 0.500;
		auto c = a * b;
		REQUIRE( double(c) == 0.3125 );
	}

	SECTION( "precision" ) {
		REQUIRE( fabs(double(M_PI) - double(fp_t<uint64_t,32>(M_PI))) <= 2e-10 );
	}
}
