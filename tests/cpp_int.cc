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

#include <fp/adapters/cpp_int.hpp>

TEST_CASE( "boost multiprecision cpp_int", "[cpp_int]" ) {
	using namespace fp;
	using namespace fp::constants;
	namespace mp = boost::multiprecision;

	SECTION( "construction and cast" ) {
		auto x = make_fp<2>(mp::int128_t{ 33 });
		REQUIRE( double(x) == 33. );
	}
	SECTION( "addition" ) {
		auto x = make_fp<0>(mp::int128_t{ 33 }) >> int_<3>;
		auto y = make_fp<2>(mp::int128_t{ 17 }) >> int_<2>;
		auto z = x + y;
		REQUIRE( double(z) == ((33. / 8.) + (17. / 4.)) );
	}
	SECTION( "subtraction" ) {
		auto x = make_fp(mp::int128_t{ 449 }) >> int_<5>;
		auto y = make_fp(mp::int128_t{ 353 }) >> int_<4>;
		auto z = x - y;
		REQUIRE( double(z) == ((449. / 32.) - (353. / 16.)) );
	}
	SECTION( "multiplication" ) {
		auto x = make_fp(mp::int128_t{ 557 }) >> int_<6>;
		auto y = make_fp(mp::int128_t{ 839 }) >> int_<7>;
		auto z = x * y;
		REQUIRE( double(z) == ((557. / 64.) * (839. / 128.)) );
	}
	SECTION( "division" ) {
		auto x = make_fp<64>(mp::int128_t{ 881 }) >> int_<3>;
		auto y = make_fp<0>(mp::int128_t{ 11 }) >> int_<1>;
		auto z = x / y;
		REQUIRE( double(z) == ((881. / 8.) / (11. / 2.)) );
	}
	SECTION( "modulus" ) {
		auto x = make_fp<32>(mp::int128_t{ 881 }) >> int_<4>;
		auto y = make_fp<0>(mp::int128_t{ 12 });
		auto z = x % y;
		REQUIRE( double(z) == std::fmod((881. / 16.), 12.) );
	}

}
