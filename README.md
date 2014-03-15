C++14 Fixed Point Library
=========================

This is a C++14 header-only fixed-point arithmetic library.

It's purpose is to wrap another type and provide fixed point arithmetic
support on top of it.

The following is a sample of what can be achieved by employing this library:
```c++
fp<int,4> x = 3.25;
fp<char,8> y = 0.75;

auto z = x * y;
//now z is of type fp<int,12>
std::cout << double(z);
//prints 2.4375
```

Also included is a *ranged* class, implemented in the file ranged.hpp.
It's purpose is to wrap another numeric type, and provide range checking
on top of it, by implementing interval arithmetic in the type system.
It will trip a compile error in case range violations occur.
It should only be looked at as a demonstration of how the *fp* class can be
composed with other classes. At the moment, the only types that can be
safely wrapped are the built-in signed integers. When a range violation
does occur, the compiler error can be very verbose and unspecific in some
cases.

It needs at least clang 3.4 to compile, with `-std=c++1y`.
Unit tests are included, and these can be built using CMake.

Example building and running tests on unix system:
```sh
cmake -DCMAKE_CXX_COMPILER=clang++ <path to source>
make
ctest
```

Fixed Point Library Description
------------------

It implements a template class type `fp<T,E>` where `T` is an underlying
arithmetic type and `E` is an integer representing the binary point position.
From now on these will be referred as the base type and the exponent,
for `T` and `E` respectively.

The base type currently is limited to only fundamental integers and floats,
although in the future it is hoped to support user-defined types.

It supports casting between integral and floating point types
and the whole set of arithmetic, bitwise and relational operators.
Binary operators also support mixing between *fp* and integers / floats.
All operations are constexpr when possible.

A scale member template function is also provided, which performs a virtual
shift.

When mixing *fp* numbers of different base types, the following rules apply:

* In the case of binary operations, normal arithmetic conversion rules
apply between the base types of both operands. For example, the expression
`fp<char,0>(10) + fp<char,0>(20)` has type `fp<int,0>`

* In case of unary operators, the normal promotion rules apply to the
base type. For example, the expression `+fp<char,0>` has type `fp<int,0>`

In case of binary operations, the exponent of the result depends on the
operation being performed.

For addition, subtraction, modulus and the bitwise operators, the result has
exponent equal to the greatest of the exponents of the operands, and the
operation is carried out as if the operand with the smallest exponent was
cast to that same exponent. For example, the result of the expression 
`fp<int,4>(1) + fp<int,8>(2)` has type `fp<int,8>`

For multiplication and division, the exponent of the result is equal to the
sum and subtraction of the exponents of the operands respectively.
This implies the expression `fp<int,4>(1) * fp<int,8>(2)` has type
`fp<int,12>`, and with division instead, the result would be `fp<int,-4>`

When using the relational operators between *fp* numbers with different
exponents, the operand with the greatest exponent is converted so it has the
same exponent as the other one, and they are then compared.
This avoids a more expensive operation, and means that only least
significant bits are discarded.

Ranged Library Description
------------------

It implements a template class type `ranged<T,LOW,MAX>` where `T` is an underlying
arithmetic type, and `LOW` and `MAX` are non-type parameters of type `T` 
which define the closed boundary of the valid range.

When operations are performed on these types, the implementation will,
at compile time, perform interval arithmetic on the ranges, and the
type of the result will reflect the possible range of the result.

If the type gets constructed with ranges which are outside the range allowable
by the underlying type, then a compile error will occur.

If a ranged variable gets assigned to / constructed from another ranged variable which does not completely
contain that range, then a compilation error will also occur.

Examples:

```
auto test1 = ranged<int32_t, -20, 50>{ 10 };
auto test2 = ranged<int32_t, 1000, 10000>{ 2000 };
auto test3 = ranged<int32_t, -3000, 8000>{ 100 };
// will trip a compile error because the result can be as big as 4'000'000'000, which is above MAX_INT
auto test4 = test1 * test2 * test3;
```

```
auto test1 = ranged<int, -20, 50>{ 10 };
// will trip a compile error because test1 could have a value up to 50, but test2 only accepts values up to 49
ranged<int, -20, 49> test2 = test1;
```

License
-------

BSD

Copyright (c) 2014, Matheus Izvekov <mizvekov@gmail.com>

All rights reserved.
