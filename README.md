C++14 Fixed Point Library
=========================

This is a C++14 header-only fixed-point arithmetic library.

It's purpose is to wrap another type and provide fixed point arithmetic
support on top of it.

It's designed to be able to wrap on top of all the built-in arithmetic types
(integers and floating point types) as well as user defined arithmetic types.

Examples of other arithmetic types which are tested and
preliminary adapters are provided:
* boost `rational`
* boost multiprecision `cpp_int` and `cpp_bin_float`
* David Stone's `bounded_integer`

The following is a sample of what can be achieved by employing this library:
```c++
using namespace fp;

auto x = make_fp<4, int >(3.25);
auto y = make_fp<8, char>(0.75);
auto z = x * y;                  // now z is of type fp_t<int,12>
std::cout << double(z);          // prints 2.4375

// using David Stone's bounded_integer
using namespace bounded::literal;
using namespace fp::constants; // import int_ shorthand for std::integral_constant

// create a bounded fixed point integer with range [1, 100]
// with initial value 30, then perform a virtual right shift
// by 3.
auto x = fp_t<bounded::integer<1, 100>, 0>{  30_bi } >> int_<3>;
// now x holds 3.75 and has range [0.125, 12.5]

//ditto, range [2, 300], initial value 150, virtual right shift by 4
auto y = fp_t<bounded::integer<2, 300>, 0>{ 150_bi } >> int_<4>;
// now y holds 9.375 and has range [0.125, 18.75]

auto z = x + y;

std::cout << double(z); // prints 13.125
// below it shows that type information of the underlying type is not lost
std::cout << double(std::numeric_limits<decltype(z)>::min()); // prints 0.25
std::cout << double(std::numeric_limits<decltype(z)>::max()); // prints 31.25

```

It needs at least clang 3.4 to compile.
Unit tests are included, and these can be built using CMake.

Example building and running tests on unix system:
```sh
cmake -DCMAKE_CXX_COMPILER=clang++ <path to source>
make
ctest
```

### How to use

This is a single-header include only library, building is only required for tests.
Just `#include <fp/fp.hpp>` and everything is inside namespace `fp`. The main
type is `fp::fp_t` and there is also a `fp::make_fp` helper which does construction
with type deduction. The namespace `fp::constants` contains shorthands `int_` and
`uint_` which can be used for virtual shifting.

Fixed Point Library Description
------------------

It implements a template class type `fp_t<T, E>` where `T` is an underlying
arithmetic type and `E` is an integer representing the binary point position.
From now on these will be referred as the underlying type and the exponent,
for `T` and `E` respectively.

It supports casting between integral and floating point types
and the whole set of arithmetic, bitwise and relational operators.
All operations are constexpr themselves, although they are only usable as
such if the underlying type also implements them as constexpr.

A virtual shift may be performed by using a `std::integral_constant` as a
shift amount with the usual shift operators `<<` and `>>`. Convenience aliases
`int_` and `uint_` are provided under namespace `fp::constants`

All operations preserve the resulting type of the underlying type's operation.

For example, if you have an instance `a` with type `fp_t<A, EXP_A>`, and an instance
b with type `fp_t<B, EXP_B>`, and you multiply them together, the type of the expression
`a * b` will be `fp_t<decltype(A{} * B{}), EXP_A + EXP_B>`. In the specific case that
`A` is `char`, `B` is `int` and `EXP_A` and `EXP_B` are `2` and `3`, the resulting
type will be `fp_t<int, 5>`, since `char{} * int{}` has type `int`, according to normal
C++ conversion rules.

Similar thing happens with unary operations, the expression `+fp_t<A, EXP_A>{}`
has type `fp_t<decltype(+A{}), EXP_A>`.

The result of a virtual shift has underlying type unchanged, and just the
exponent is modified. The expression `fp_t<A, EXP_A>{} >> int_<SA>` has
type `fp_t<A, EXP_A + SA>`.

All the unary operations preserve the exponent, and in the case of
binary operations, the exponent of the result depends on the operation
being performed.

For addition, subtraction, modulus and the bitwise operators, the result has
exponent equal to the greatest of the exponents of the operands, and the
operation is carried out as if the operand with the smallest exponent was
cast to that same exponent. For example, the result of the expression 
`fp_t<int, 4>(1) + fp_t<int, 8>(2)` has type `fp_t<int, 8>`

For multiplication and division, the exponent of the result is equal to the
sum and subtraction of the exponents of the operands respectively.
This implies the expression `fp_t<int, 4>{} * fp_t<int, 8>{}` has type
`fp_t<int, 12>`, and with division instead, the resulting type would be
`fp_t<int, -4>`

When using the relational operators between *fp_t* numbers with different
exponents, the operand with the greatest exponent is converted so it has the
same exponent as the other one, and they are then compared.
This avoids a more expensive operation, and means that only least
significant bits are discarded.

#### Requirements on the underlying type

At a minimum, the underlying type must be default constructible,
copy constructible and support left and right shift by a
`std::integral_constant`. The other operators are optional, and if
they are not supported, the composed type will not support them.
For example, if there is no modulus operator implemented for `T`,
then `fp_t<T>` will not support modulus either.

License
-------

ISC

Copyright (c) 2018, Matheus Izvekov <mizvekov@gmail.com>

All rights reserved.
