#.rst:
# FindLibDL
# -------
#
# Finds the DL (dynamic linking) library
#
# This will define the following variables::
#
#   LibDL_FOUND    - True if the system has the DL library
#   LibDL_VERSION  - The version of the DL library which was found
#
# and the following imported targets::
#
#   LibDL::LibDL   - The DL library

find_path(LIBDL_INCLUDE_DIR
  NAMES "dlfcn.h"
)
find_library(LIBDL_LIBRARY
  NAMES dl
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibDL
  REQUIRED_VARS
    LIBDL_LIBRARY
    LIBDL_INCLUDE_DIR
)

# Public interface: variables
if(LibDL_FOUND)
  set(LibDL_INCLUDE_DIRS ${LIBDL_INCLUDE_DIR})
  set(LibDL_LIBRARIES ${LIBDL_LIBRARY})
endif()

# Public interface: targets
if(LibDL_FOUND AND NOT TARGET LibDL::LibDL)
  add_library(LibDL::LibDL UNKNOWN IMPORTED)
  set_target_properties(LibDL::LibDL PROPERTIES
    IMPORTED_LOCATION "${LIBDL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBDL_INCLUDE_DIR}"
  )
endif()

# Cleanup
mark_as_advanced(
  LIBDL_INCLUDE_DIR
  LIBDL_LIBRARY
)
