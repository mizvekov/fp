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
