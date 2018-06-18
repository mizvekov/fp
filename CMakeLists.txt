# Copyright (c) 2018, Matheus Izvekov <mizvekov@gmail.com>
# All rights reserved.

cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(fp LANGUAGES CXX)
include(CTest)

add_library(fp INTERFACE)
target_sources(fp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/fp/fp.hpp)
target_include_directories(fp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(fp INTERFACE cxx_std_14)

add_subdirectory(tests)
