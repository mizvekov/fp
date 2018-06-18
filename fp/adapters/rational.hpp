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

#pragma once

#include <boost/rational.hpp>

namespace boost {

template<typename T> static auto operator<<(const boost::rational<T> &a, int b) { return a * boost::rational<T>(T(1) << b, T(1)); }
template<typename T> static auto operator>>(const boost::rational<T> &a, int b) { return a * boost::rational<T>(T(1), T(1) << b); }

};

namespace std {

template<class T> struct numeric_limits<boost::rational<T>> {
	using type = boost::rational<T>;
	using base = numeric_limits<T>;

	static_assert(base::is_specialized, "no std::numeric_limits specialization for base type");

	static constexpr bool is_specialized = true;

	static constexpr type min()    noexcept { return { 1, base::max() }; }
	static constexpr type max()    noexcept { return { base::max() }; }
	static constexpr type lowest() noexcept { return { base::lowest() }; }

	static constexpr int  digits       = base::digits;
	static constexpr int  digits10     = base::digits10;
	static constexpr int  max_digits10 = base::max_digits10;
	static constexpr bool is_signed    = base::is_signed;
	static constexpr bool is_integer   = base::is_integer;
	static constexpr bool is_exact     = base::is_exact;
	static constexpr int  radix        = base::radix;

	static constexpr type epsilon()     noexcept { return min(); }
	static constexpr type round_error() noexcept { return { 1 }; }

	static constexpr int  min_exponent   = 0;
	static constexpr int  min_exponent10 = 0;
	static constexpr int  max_exponent   = 0;
	static constexpr int  max_exponent10 = 0;

	static constexpr bool has_infinity      = base::has_infinity;
	static constexpr bool has_quiet_NaN     = base::has_quiet_NaN;
	static constexpr bool has_signaling_NaN = base::has_signaling_NaN;
	static constexpr float_denorm_style has_denorm = base::has_denorm;
	static constexpr bool has_denorm_loss = base::has_denorm_loss;

	static constexpr type infinity()      noexcept { return { base::infinity() }; }
	static constexpr type quiet_NaN()     noexcept { return { base::quiet_NaN() }; }
	static constexpr type signaling_NaN() noexcept { return { base::signaling_NaN() }; }
	static constexpr type denorm_min()    noexcept { return { base::denorm_min(), base::max() }; }

	static constexpr bool is_iec559  = base::is_iec559;
	static constexpr bool is_bounded = base::is_bounded;
	static constexpr bool is_modulo  = base::is_modulo;

	static constexpr bool traps = base::traps;
	static constexpr bool tinyness_before = base::tinyness_before;
	static constexpr float_round_style round_style = base::round_style;
};

}
