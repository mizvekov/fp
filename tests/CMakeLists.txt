# Copyright (c) 2014, Matheus Izvekov <mizvekov@gmail.com>
# All rights reserved.

INCLUDE(ExternalProject)
FIND_PACKAGE(Git REQUIRED)

ExternalProject_Add(
	catch
	PREFIX ${CMAKE_BINARY_DIR}/catch
	GIT_REPOSITORY https://github.com/philsquared/Catch.git
	TIMEOUT 10
	UPDATE_COMMAND ${GIT_EXECUTABLE} pull
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	LOG_DOWNLOAD ON
)
ExternalProject_Get_Property(catch source_dir)
INCLUDE_DIRECTORIES(. ${source_dir}/include)

ADD_SUBDIRECTORY(static)

ADD_LIBRARY(catch_main catch_main.cc)
TARGET_LINK_LIBRARIES(catch_main c++abi)
ADD_DEPENDENCIES(catch_main catch)

ADD_EXECUTABLE(simple simple.cc)
TARGET_LINK_LIBRARIES(simple catch_main)
ADD_TEST(simple simple)

FIND_PACKAGE(Hg)
IF(HG_FOUND)
	ExternalProject_Add(
		bounded_integer
		PREFIX ${CMAKE_BINARY_DIR}/bounded_integer
		HG_REPOSITORY ssh://hg@bitbucket.org/davidstone/bounded_integer
		TIMEOUT 10
		UPDATE_COMMAND ${HG_EXECUTABLE} pull
		PATCH_COMMAND ""
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		LOG_DOWNLOAD ON
	)
	ExternalProject_Get_Property(bounded_integer source_dir)
	INCLUDE_DIRECTORIES(${source_dir})

	ADD_EXECUTABLE(bounded_integer_test bounded_integer.cc)
	TARGET_LINK_LIBRARIES(bounded_integer_test catch_main)
	ADD_DEPENDENCIES(bounded_integer_test bounded_integer)
	ADD_TEST(bounded_integer bounded_integer_test)
ENDIF()

FIND_PACKAGE(Boost 1.56.0)

IF(Boost_FOUND)
	INCLUDE_DIRECTORIES(Boost_INCLUDE_DIRS)

	ADD_EXECUTABLE(rational rational.cc)
	TARGET_LINK_LIBRARIES(rational catch_main)
	ADD_TEST(rational rational)

	ADD_EXECUTABLE(cpp_int cpp_int.cc)
	TARGET_LINK_LIBRARIES(cpp_int catch_main)
	ADD_TEST(cpp_int cpp_int)

	ADD_EXECUTABLE(cpp_bin_float cpp_bin_float.cc)
	TARGET_LINK_LIBRARIES(cpp_bin_float catch_main)
	ADD_TEST(cpp_bin_float cpp_bin_float)
ENDIF()
