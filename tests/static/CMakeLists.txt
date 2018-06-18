# Copyright (c) 2018, Matheus Izvekov <mizvekov@gmail.com>
# All rights reserved.

add_library(float_test float.cc)
target_link_libraries(float_test fp)

add_library(int_test int.cc)
target_link_libraries(int_test fp)

add_library(ranged_test ranged.cc)
target_link_libraries(ranged_test ranged fp)

add_library(ranged_fp_test ranged_fp.cc)
target_link_libraries(ranged_fp_test ranged fp)

add_library(ranged_fp_very_nested_test ranged_fp_very_nested.cc)
target_link_libraries(ranged_fp_very_nested_test ranged fp)
