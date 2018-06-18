# Copyright (c) 2018, Matheus Izvekov <mizvekov@gmail.com>
# All rights reserved.

add_library(ranged INTERFACE)
target_sources(ranged INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ranged.hpp)
target_include_directories(ranged INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(ranged INTERFACE cxx_std_14)

add_subdirectory(static)

include(ExternalProject)

find_package(Catch2 CONFIG REQUIRED)

add_executable(simple simple.cc)
target_link_libraries(simple fp Catch2::Catch)
add_test(simple simple)

find_package(Hg)
if(HG_FOUND)
	ExternalProject_Add(
		bounded_integer
		PREFIX ${CMAKE_BINARY_DIR}/bounded_integer
		HG_REPOSITORY https://bitbucket.org/davidstone/bounded_integer
		TIMEOUT 10
		UPDATE_COMMAND ${HG_EXECUTABLE} pull
		PATCH_COMMAND ""
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		LOG_DOWNLOAD ON
	)
	ExternalProject_Get_Property(bounded_integer source_dir)

	add_executable(bounded_integer_test bounded_integer.cc)
	target_link_libraries(bounded_integer_test fp Catch2::Catch)
	target_include_directories(bounded_integer_test PRIVATE ${source_dir}/include)
	target_compile_features(bounded_integer_test PUBLIC cxx_std_17)
	add_dependencies(bounded_integer_test bounded_integer)
	add_test(bounded_integer bounded_integer_test)
endif()

find_package(Boost 1.56.0 OPTIONAL_COMPONENTS exception)
if(TARGET Boost::boost AND TARGET Boost::exception)
	add_executable(rational rational.cc)
	target_link_libraries(rational fp Boost::boost Boost::exception Catch2::Catch)
	add_test(rational rational)

	add_executable(cpp_int cpp_int.cc)
	target_link_libraries(cpp_int fp Boost::boost Boost::exception Catch2::Catch)
	add_test(cpp_int cpp_int)

	add_executable(cpp_bin_float cpp_bin_float.cc)
	target_link_libraries(cpp_bin_float fp Boost::boost Boost::exception Catch2::Catch)
	add_test(cpp_bin_float cpp_bin_float)
endif()
