#
# Modern Wheel : all the things that shouldn't be reinvented from one project to the other
#
# The MIT License (MIT)
# 
# Copyright (C) 2015  Massimiliano Culpo
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# 

ADD_SUBDIRECTORY( "fixtures" )

FIND_PACKAGE( Boost 1.55 REQUIRED COMPONENTS unit_test_framework )

CONFIGURE_FILE( dlmanager_test.cpp ${CMAKE_CURRENT_BINARY_DIR} @ONLY)
SET( 
  TEST_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/prototype_factory_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/singleton_test.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/expected_test.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/composite_base_test.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/decorator_test.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dlmanager_test.cpp
)

##########
## Dynamic library : collection of tests
ADD_LIBRARY( modern_wheel_tests ${TEST_SOURCES})
## Compile definitions
TARGET_COMPILE_DEFINITIONS(
  modern_wheel_tests
  PUBLIC BOOST_TEST_DYN_LINK
)
## Include directories
TARGET_INCLUDE_DIRECTORIES(
  modern_wheel_tests
  PUBLIC 
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/test
)
## Link
TARGET_LINK_LIBRARIES(
  modern_wheel_tests
  PUBLIC  
  internal_extension_test
  mwheel
  Boost::unit_test_framework
)
##########

##########
## Executable : dumb runner that triggers the tests that are dynamically linked
ADD_EXECUTABLE( test_modern_wheel.x test_main.cpp )
TARGET_LINK_LIBRARIES(
  test_modern_wheel.x
  modern_wheel_tests
)
##########
ADD_TEST(
  NAME run_unit_test
  COMMAND test_modern_wheel.x --report_level=detailed --build_info --log_format=XML --log_level=all --output_format=HRF --random --log_sink=test.log
)
