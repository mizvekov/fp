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

#include <cstdint>
#include <limits>
#include <cmath>
#include <fp/fp.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

using namespace fp;
using namespace fp::constants;

/********** fp instances must be standard layout iff base_type is standard layout *************************************/
static_assert(std::is_standard_layout<fp_t<float,0>>::value, "fp_t<float> must be standard layout");
/**********************************************************************************************************************/

/********************** Construction and cast tests *******************************************************************/
#define TEST(T,E,V) static_assert((decltype(V))(fp_t<T,E>(V)) == V, "")

TEST(float, 16, float(M_PI));

TEST(float, 8, 1.375f);
TEST(float, -8, 2.625f);
TEST(float, 0, 0.625);

#undef TEST
/**********************************************************************************************************************/

/******* Relational operators tests ***********************************************************************************/
static_assert(make_fp<10>(8.3333) == make_fp(8.3333), "");
static_assert(make_fp<10>(8.6666) == make_fp<8>(8.6666), "");
static_assert(make_fp<10>(8.3333) != make_fp<7>(8.3334), "");
static_assert(make_fp<10>(8.25) < make_fp<16>(10.25), "");
static_assert(make_fp<10>(4.375) <= make_fp<16>(4.375), "");
static_assert(make_fp<10>(4.750) >= make_fp<16>(4.750), "");
/**********************************************************************************************************************/

/**************** Unary operators tests *******************************************************************************/
static_assert(+make_fp<10>(4.750) == make_fp<16>(4.750), "");
static_assert(-make_fp<10>(4.750) == make_fp(-4.750), "");
static_assert(decltype(+make_fp<10>(0))::exp{} == 10, "");
static_assert(std::is_same<decltype(+fp_t<float,8>())::base_type, decltype(+float())>::value, "");
/**********************************************************************************************************************/

/*********************** Binary operators tests ***********************************************************************/
static_assert(make_fp<16>(5.25) + make_fp(4.75) == make_fp(10), "");
static_assert(make_fp(7) == make_fp(1) + make_fp<16>(6), "");
static_assert(make_fp<16>(5.25) - make_fp(4.75) == make_fp<4>(0.5), "");
static_assert(make_fp<14>(7) - make_fp<12>(5) == make_fp<16>(2.0), "");
static_assert(make_fp<4>(4.75) * make_fp<8>(5.25) == make_fp(24.9375), "");
static_assert(make_fp<16>(24.9375) / make_fp<8>(5.25) == make_fp(4.75), "");

static_assert(make_fp<10>(8.0) == make_fp<4>(80) / make_fp<6>(10), "");
static_assert(make_fp<16>(18.5) == make_fp<16>(185.0) / make_fp(10), "");

static_assert( (make_fp<8>(8.25) << int_<2>) == fp_t<int>(33), "");
static_assert( (make_fp<8>(33.0) >> int_<2>) == fp_t<float>(8.25), "");

static_assert(std::is_same<decltype(
	fp_t<float,8>() + fp_t<double,8>())::base_type,
	decltype(float()+double())
	>::value, "");

static_assert(std::is_same<decltype(fp_t<double,8>() * fp_t<float,8>())::base_type, double>::value, "");
/**********************************************************************************************************************/

class float_test {
	static constexpr fp_t<float,10> t1 = 1;
	static constexpr fp_t<double,12> t2 = 2;
	static constexpr auto t3 = make_fp<16>(7.25);
	static constexpr auto t4 = make_fp(3) + t1 + t3 - t2;
	static_assert(t4 == make_fp(9.25), "");
	static_assert(std::is_same<decltype(t4)::base_type, double>::value, "");
	static_assert(decltype(t4)::exp{} == 16, "");
	static constexpr auto t5 = make_fp<4>(2.75) * make_fp<8>(10.25);
	static_assert(std::is_same<decltype(t5)::base_type, double>::value, "");
	static_assert(decltype(t5)::exp{} == 12, "");
	static_assert(t5 == make_fp<10>(2.75 * 10.25), "");
};
