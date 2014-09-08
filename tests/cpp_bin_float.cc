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
