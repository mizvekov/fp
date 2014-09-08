# Copyright (c) 2014, Matheus Izvekov <mizvekov@gmail.com>
# All rights reserved.

PROJECT(fp CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++1y -stdlib=libc++ -O0" CACHE STRING "" FORCE)

INCLUDE_DIRECTORIES(.)

ENABLE_TESTING()
ADD_SUBDIRECTORY(tests)
