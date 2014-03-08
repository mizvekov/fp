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


#ifndef MIZVEKOV_FP_STATIC_TESTS_INCLUDE_HPP_INCLUDED
#define MIZVEKOV_FP_STATIC_TESTS_INCLUDE_HPP_INCLUDED

#include <cstdint>
#include <limits>
#include "fp.hpp"

/********** fp instances must be standard layout iff base_type is standard layout *************************************/
static_assert(std::is_standard_layout<fp<uint32_t,0>>::value, "fp<uint32_t> must be standard layout");
/**********************************************************************************************************************/

/********************** Construction and cast tests *******************************************************************/
#define TEST(T,E,V) static_assert((decltype(V))(fp<T,E>(V)) == V, "")
TEST(uint32_t, 16, std::numeric_limits<uint8_t>::min());
TEST(uint32_t, 16, std::numeric_limits<uint8_t>::max());
TEST(uint32_t, 16, std::numeric_limits<int8_t>::max());
TEST(uint64_t, 16, std::numeric_limits<uint16_t>::min());
TEST(uint64_t, 16, std::numeric_limits<uint16_t>::max());
TEST(uint32_t, 16, std::numeric_limits<int16_t>::max());
TEST(uint64_t, 16, std::numeric_limits<uint32_t>::min());
TEST(uint64_t, 16, std::numeric_limits<uint32_t>::max());
TEST(uint64_t, 16, std::numeric_limits<int32_t>::max());

TEST(uint32_t, 16, uint16_t(std::numeric_limits<int8_t>::max()));
TEST(uint32_t, 16, uint16_t(std::numeric_limits<int16_t>::max()));
TEST(uint32_t, 16, uint32_t(std::numeric_limits<int16_t>::max()));

TEST(uint16_t, 8, 1.375f);
TEST(uint16_t, 8, 2.625f);
TEST(uint16_t, 8, 0.625);
TEST(uint16_t, 8, 4.25);
TEST(uint16_t, 8, 6.5l);
TEST(uint16_t, 8, 8.125l);

TEST(uint8_t, -4, 1024);
#undef TEST
/**********************************************************************************************************************/

/******* Relational operators tests ***********************************************************************************/
static_assert(fp<uint16_t,10>(8.25) == 8.25, "");
static_assert(fp<uint16_t,10>(8.25) == fp<uint32_t,16>(8.25), "");
static_assert(fp<uint16_t,10>(8.25) != fp<uint32_t,16>(8.26), "");
static_assert(fp<uint16_t,10>(8.25) < fp<uint32_t,16>(10.25), "");
static_assert(fp<uint32_t,10>(23.5) < fp<uint32_t,16>(23.6), "");
static_assert(fp<uint16_t,10>(16.5) > fp<uint32_t,16>(10.25), "");
static_assert(fp<uint64_t,10>(4.375) <= fp<uint32_t,16>(4.375), "");
static_assert(fp<uint64_t,10>(4.250) <= fp<uint32_t,16>(4.375), "");
static_assert(fp<uint64_t,10>(4.750) >= fp<uint32_t,16>(4.375), "");
static_assert(fp<uint64_t,10>(4.750) >= fp<uint32_t,16>(4.750), "");
/**********************************************************************************************************************/

/**************** Unary operators tests *******************************************************************************/
static_assert(+fp<uint64_t,10>(4.750) == fp<uint32_t,16>(4.750), "");
static_assert(~fp<uint32_t,0>(0xDEADBEEF) == ~0xDEADBEEF, "");
static_assert((+fp<uint64_t,10>(0)).exp == 10, "");
static_assert((~fp<uint32_t,-5>(0)).exp == -5, "");
static_assert(std::is_same<decltype(+fp<uint8_t,8>())::base_type, decltype(+uint8_t())>::value, "");
/**********************************************************************************************************************/

/*********************** Binary operators tests ***********************************************************************/
static_assert(fp<uint32_t,16>(5.25) + 4.75 == 10, "");
static_assert(7 == 1 + fp<uint32_t,16>(6), "");
static_assert(fp<uint32_t,16>(5.25) - 4.75 == fp<uint8_t,4>(0.5), "");
static_assert(fp<uint32_t,14>(7) - fp<uint32_t,12>(5) == 2.0, "");
static_assert(4.75 * fp<uint32_t,8>(5.25) == 24.9375, "");
static_assert(fp<uint32_t,16>(24.9375) / fp<uint32_t,8>(5.25) == 4.75, "");
static_assert(fp<uint32_t,16>(18.5) % fp<uint32_t,16>(4.25) == 1.5, "");

static_assert(fp<uint32_t,10>(8.0) == fp<uint16_t,4>(80) / fp<uint32_t,6>(10), "");
static_assert(fp<uint32_t,16>(18.5) == fp<uint32_t,16>(185) / 10, "");

static_assert((fp<uint32_t,8>(0xDEAD) & 0x7777) == 0x5625, "");
static_assert((fp<uint16_t,8>(9.640625) & fp<uint16_t,8>(0xFF, {})) == 0.640625, "");
static_assert((fp<uint32_t,8>(0x5625) | 0x8888) == 0xDEAD, "");
static_assert((fp<uint32_t,8>(0xDEAD) ^ 0x6042) == 0xBEEF, "");

static_assert(33 == (fp<uint32_t,8>(8.25) << 2), "");
static_assert((fp<uint32_t,8>(33) >> 2) == 8.25, "");

static_assert(fp<uint32_t,8>(8.25).vshift< 2>() == 33, "");
static_assert(fp<uint32_t,8>(33  ).vshift<-2>() == 8.25, "");

static_assert(std::is_same<decltype(
	fp<uint64_t,8>() + fp<uint8_t,8>())::base_type,
	decltype(uint64_t()+uint8_t())
	>::value, "");
static_assert(std::is_same< decltype(fp<uint16_t,8>() << 2ULL)::base_type, decltype(uint16_t() << 2ULL)>::value, "");

static_assert((fp<uint64_t,10>(1) + fp<uint8_t,8>(2)).exp == 10, "");
static_assert((fp<uint64_t,12>(1) - fp<uint8_t,6>(2)).exp == 12, "");
static_assert((fp<uint64_t,12>(1) * fp<uint8_t,6>(2)).exp == 18, "");
static_assert((fp<uint64_t,12>(1) / fp<uint8_t,4>(2)).exp == 8, "");
static_assert((fp<uint64_t,12>(1) % fp<uint8_t,6>(2)).exp == 12, "");
static_assert((fp<uint64_t,8>(1) & fp<uint8_t,6>(2)).exp == 8, "");
static_assert((fp<uint64_t,5>(1) | fp<uint8_t,13>(2)).exp == 13, "");
static_assert((fp<uint64_t,-3>(1) ^ fp<uint8_t,-2>(2)).exp == -2, "");

static_assert((fp<uint64_t,12>(1) << 2).exp == 12, "");
static_assert((fp<uint64_t,10>(1) >> 2).exp == 10, "");
/**********************************************************************************************************************/

static constexpr fp<uint16_t,10> t1 = 1;
static constexpr fp<uint32_t,12> t2 = 2;
static constexpr fp<uint64_t,16> t3 = 7.25;
static constexpr auto t4 = 3 + t1 + t3 - t2;
static_assert(t4 == 9.25, "");
static_assert(std::is_same<decltype(t4)::base_type, uint64_t>::value, "");
static_assert(t4.exp == 16, "");

static constexpr auto t5 = 2.75 * fp<uint32_t,8>(10.25);
static_assert(std::is_same<decltype(t5)::base_type, uint32_t>::value, "");
static_assert(t5.exp == 16, "");
static_assert(t5 == 2.75 * 10.25, "");

#endif
