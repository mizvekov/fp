C++14 Fixed Point Library
=========================

This is a C++14 header-only fixed-point arithmetic library.

With this library, the following examples become possible:

```c++
fp<int,4> x = 3.25;
fp<char,8> y = 0.75;

auto z = x * y;
//now z is of type fp<int,12>
std::cout << double(z);
//prints 2.4375
```

It needs at least clang 3.4 to compile, with `-std=c++1y`.
Unit tests are included, and these can be built using CMake.

Example building and running tests on unix system:
```sh
cmake -DCMAKE_CXX_COMPILER=clang++ <path to source>
make
ctest
```

Description
------------------

It implements a template class type `fp<T,E>` where `T` is an underlying
integral type and `E` is an integer representing the binary point position.
From now on these will be referred as the base type and the exponent,
for `T` and `E` respectively.

It supports casting between integral and floating point types
and the whole set of arithmetic, bitwise and relational operators.
Binary operators also support mixing between *fp* and integers / floats.
All operations are constexpr when possible.

A vshift member template function is also provided, which performs a virtual
shift.

When a binary operation is used between *fp* numbers and
plain integers / floats, the latter is implicitly converted to the type
of the first one.
For example, the expression `fp<int,8>(4) + 1.25` is equivalent
to `fp<int,8>(4) + fp<int,8>(1.25)`

When mixing *fp* numbers of different base types, the following rules apply:

* In the case of binary operations, normal arithmetic conversion rules
apply between the base types of both operands. For example, the expression
`fp<char,0>(10) + fp<char,0>(20)` has type `fp<int,0>`

* In the case of unary operators, the normal promotion rules apply to the
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

License
-------

BSD

Copyright (c) 2014, Matheus Izvekov <mizvekov@gmail.com>

All rights reserved.
