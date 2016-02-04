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

CMAKE_MINIMUM_REQUIRED( VERSION 3.2.0 )

PROJECT( 
  ModernWheel
  VERSION 0.1
  LANGUAGES CXX
)

###################
## Build options ##
###################

#### Default build type
IF( NOT CMAKE_BUILD_TYPE )
  SET (CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, RelWithDebInfo will be the default." FORCE)
ENDIF()

#### Default documentation
IF( NOT BUILD_DOCUMENTATION )
  SET (BUILD_DOCUMENTATION OFF CACHE STRING
      "If ON the documentation will be built (requires Doxygen). Dafault OFF" FORCE)
ENDIF()

#### Shared library build
SET(BUILD_SHARED_LIBS ON)

SET(CMAKE_CXX_STANDARD 14)

#### Compiler flags
IF( CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET(CXX_WARNING_LEVEL "-Wall -Werror -pedantic")
  SET(CXX_CUSTOM_LINK_FLAGS "-Wl,--no-as-needed")
  SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
  SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")
ELSEIF( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" ) 
  SET(CXX_WARNING_LEVEL "-Wall -Werror -pedantic -Wdocumentation")
  SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
  SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")
ELSEIF( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
  SET(CXX_WARNING_LEVEL "/W4")
  SET(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi")
  SET(CMAKE_CXX_FLAGS_RELEASE "/O2 -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /zi")
ELSE()
  MESSAGE(ERROR "Detected an unsupported compiler")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_LEVEL} ${CXX_CUSTOM_LINK_FLAGS}")


#### Output log
STRING( TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" SPECIFIC_BUILD)
MESSAGE("############ COMPILER OPTIONS ############")
MESSAGE("Compiler ID : ${CMAKE_CXX_COMPILER_ID}")
MESSAGE("Compiler path : ${CMAKE_CXX_COMPILER}")
MESSAGE("Compiler version : ${CMAKE_CXX_COMPILER_VERSION}")
MESSAGE("CXX flags (base) : ${CMAKE_CXX_FLAGS}")
MESSAGE("CXX flags (build-type) : ${${SPECIFIC_BUILD}}")
MESSAGE("############ COMPILER OPTIONS ############")

##################
## Dependencies ##
##################

ADD_DEFINITIONS( "-DBOOST_ALL_NO_LIB" )

FIND_PACKAGE( Boost 1.55 REQUIRED COMPONENTS filesystem system  )

ADD_SUBDIRECTORY( "include" )
ADD_SUBDIRECTORY( "src" )

IF( BUILD_DOCUMENTATION )
  ADD_SUBDIRECTORY( "doc" )
ENDIF()

OPTION( BUILD_UNIT_TEST "Enable the build of unit tests" ON )
IF( BUILD_UNIT_TEST )
  ENABLE_TESTING()
  ADD_SUBDIRECTORY( "test" )
ENDIF()

###############
## Packaging ##
###############

#SET(CPACK_GENERATOR "TGZ;DEB")
#SET(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/packages")
#SET(CPACK_PACKAGE_VERSION_MAJOR "${ModernWheel_VERSION_MAJOR}")
#SET(CPACK_PACKAGE_VERSION_MINOR "${ModernWheel_VERSION_MINOR}")
#SET(CPACK_SOURCE_IGNORE_FILES "/CVS/;/.git/;.#;/#;.*~;cscope.*;/nbproject/;/build*/")
#SET(CPACK_PACKAGE_CONTACT "alalazo")
#INCLUDE( CPack )
