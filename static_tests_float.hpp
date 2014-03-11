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


#ifndef MIZVEKOV_FP_STATIC_TESTS_FLOAT_INCLUDE_HPP_INCLUDED
#define MIZVEKOV_FP_STATIC_TESTS_FLOAT_INCLUDE_HPP_INCLUDED

#include <cstdint>
#include <limits>
#include "fp.hpp"

/********** fp instances must be standard layout iff base_type is standard layout *************************************/
static_assert(std::is_standard_layout<fp<float,0>>::value, "fp<float> must be standard layout");
/**********************************************************************************************************************/

/********************** Construction and cast tests *******************************************************************/
#define TEST(T,E,V) static_assert((decltype(V))(fp<T,E>(V)) == V, "")

TEST(float, 16, float(M_PI));

TEST(float, 8, 1.375f);
TEST(float, -8, 2.625f);
TEST(float, 0, 0.625);

#undef TEST
/**********************************************************************************************************************/

/******* Relational operators tests ***********************************************************************************/
static_assert(fp<float,10>(8.3333) == 8.3333, "");
static_assert(fp<float,10>(8.6666) == fp<float,8>(8.6666), "");
static_assert(fp<float,10>(8.3333) != fp<float,7>(8.3334), "");
static_assert(fp<float,10>(8.25) < fp<float,16>(10.25), "");
static_assert(fp<float,10>(4.375) <= fp<float,16>(4.375), "");
static_assert(fp<float,10>(4.750) >= fp<float,16>(4.750), "");
/**********************************************************************************************************************/

/**************** Unary operators tests *******************************************************************************/
static_assert(+fp<float,10>(4.750) == fp<float,16>(4.750), "");
static_assert(-fp<float,10>(4.750) == -4.750, "");
static_assert((+fp<float,10>(0)).exp == 10, "");
static_assert(std::is_same<decltype(+fp<float,8>())::base_type, decltype(+float())>::value, "");
/**********************************************************************************************************************/

/*********************** Binary operators tests ***********************************************************************/
static_assert(fp<float,16>(5.25) + 4.75 == 10, "");
static_assert(7 == 1 + fp<float,16>(6), "");
static_assert(fp<float,16>(5.25) - 4.75 == fp<float,4>(0.5), "");
static_assert(fp<float,14>(7) - fp<float,12>(5) == 2.0, "");
static_assert(4.75 * fp<float,8>(5.25) == 24.9375, "");
static_assert(fp<float,16>(24.9375) / fp<float,8>(5.25) == 4.75, "");

static_assert(fp<float,10>(8.0) == fp<float,4>(80) / fp<float,6>(10), "");
static_assert(fp<float,16>(18.5) == fp<float,16>(185) / 10, "");

static_assert(fp<float,8>(8.25).vshift< 2>() == 33, "");
static_assert(fp<float,8>(33  ).vshift<-2>() == 8.25, "");

static_assert(std::is_same<decltype(
	fp<float,8>() + fp<double,8>())::base_type,
	decltype(float()+double())
	>::value, "");

static_assert(std::is_same<decltype(fp<double,8>() * fp<float,8>())::base_type, double>::value, "");
/**********************************************************************************************************************/

class float_test {
	static constexpr fp<float,10> t1 = 1;
	static constexpr fp<double,12> t2 = 2;
	static constexpr fp<double,16> t3 = 7.25;
	static constexpr auto t4 = 3 + t1 + t3 - t2;
	static_assert(t4 == 9.25, "");
	static_assert(std::is_same<decltype(t4)::base_type, double>::value, "");
	static_assert(t4.exp == 16, "");
	static constexpr auto t5 = 2.75 * fp<float,8>(10.25);
	static_assert(std::is_same<decltype(t5)::base_type, float>::value, "");
	static_assert(t5.exp == 16, "");
	static_assert(t5 == 2.75 * 10.25, "");
};

#endif
