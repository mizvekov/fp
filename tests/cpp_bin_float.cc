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
#include <fp/adapters/cpp_bin_float.hpp>

TEST_CASE( "boost multiprecision cpp_float_bin", "[cpp_float_bin]" ) {
	using namespace fp;
	using namespace fp::constants;
	namespace mp = boost::multiprecision;
	using float_100 = mp::number<mp::cpp_bin_float<100>>;

	SECTION( "construction and cast" ) {
		auto x = make_fp<2>(float_100{ 17.5 });
		REQUIRE( double(x) == 17.5 );
	}
	SECTION( "addition" ) {
		auto x = make_fp<-2>(float_100{ 17.5 }) << int_<3>;
		auto y = make_fp<-6>(float_100{ 20.3 }) << int_<4>;
		auto z = x + y;
		REQUIRE( double(z) == ((17.5 * 8.) + (20.3 * 16.)) );
	}
	SECTION( "subtraction" ) {
		auto x = make_fp(float_100{ 33.7 }) << int_<4>;
		auto y = make_fp(float_100{ 228.68 }) >> int_<2>;
		auto z = x - y;
		REQUIRE( double(z) == ((33.7 * 16.) - (228.68 / 4.)) );
	}
	SECTION( "multiplication" ) {
		auto x = make_fp(float_100{ 3.1415 }) << int_<1>;
		auto y = make_fp(float_100{ 100.0 }) >> int_<4>;
		auto z = x * y;
		REQUIRE( double(z) == ((3.1415 * 2.) * (100.0 / 16.)) );
	}
	SECTION( "division" ) {
		auto x = make_fp(float_100{ 881.7 }) >> int_<3>;
		auto y = make_fp(float_100{ 11.22 }) >> int_<1>;
		auto z = x / y;
		REQUIRE( double(z) == ((881.7 / 8.) / (11.22 / 2.)) );
	}
	SECTION( "modulus" ) {
		auto x = make_fp(float_100{ 881.7 }) >> int_<4>;
		auto y = make_fp(float_100{ 11.22 }) >> int_<0>;
		auto z = x % y;
		REQUIRE( double(z) == std::fmod((881.7 / 16.), (11.22 / 1.)) );
	}
}
