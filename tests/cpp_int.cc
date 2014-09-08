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
