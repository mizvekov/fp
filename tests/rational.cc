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

#include <fp/adapters/rational.hpp>

TEST_CASE( "rational tests", "[rational]" ) {
	using namespace fp;
	using namespace fp::constants;
	using boost::rational;

	SECTION( "construction and cast" ) {
		auto x = rational<int>(1,2) << 2;
		REQUIRE( boost::rational_cast<double>(x) == 2 );
	}
	SECTION( "addition" ) {
		auto x = make_fp<2>(rational<int>(1,2));
		auto y = make_fp<1>(rational<int>(2));
		REQUIRE( boost::rational_cast<double>(rational<int>(x + y)) == 2.5 );
	}
	SECTION( "subtraction" ) {
		auto x = make_fp<1>(rational<int>(6));
		auto y = make_fp<2>(rational<int>(9,2));
		REQUIRE( boost::rational_cast<double>(rational<int>(x - y)) == 1.5 );
	}
	SECTION( "multiplication" ) {
		auto x = make_fp<1>(rational<int>(12,5));
		auto y = make_fp<2>(rational<int>(15,6));
		REQUIRE( boost::rational_cast<double>(rational<int>(x * y)) == (12. * 15.) / (5. * 6.) );
	}
	SECTION( "division" ) {
		auto x = make_fp<5>(rational<int>(34,7));
		auto y = make_fp<8>(rational<int>(21,8));
		REQUIRE( boost::rational_cast<double>(rational<int>(x / y)) == (34. * 8.) / (21. * 7.) );
	}
	SECTION( "multiplication with virtual shift" ) {
		auto x = make_fp<1>(rational<int>(33,3)) << int_<2>;
		auto y = make_fp<8>(rational<int>(60,16)) >> int_<4>;
		REQUIRE( boost::rational_cast<double>(rational<int>(x * y)) == ((33. / 3.) * 4.) * ((60. / 16.) / 16.) );
	}
}
