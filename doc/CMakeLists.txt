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

function(JOIN VALUES GLUE OUTPUT)
  string (REPLACE ";" "${GLUE}" _TMP_STR "${VALUES}")
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

FIND_PACKAGE(Doxygen 1.8.6 REQUIRED)

SET( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in     )
SET( doxyfile_logo        ${CMAKE_CURRENT_SOURCE_DIR}/Wagon_wheel_at_Black_Creek_Pioneer_Village.jpg )
SET( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile              )
SET( doxy_html_index_file ${CMAKE_CURRENT_BINARY_DIR}/html/index.html )
SET( doxy_output_root     ${CMAKE_CURRENT_BINARY_DIR}                 ) # Pasted into Doxyfile.in
SET( doxy_input           ${PROJECT_SOURCE_DIR}/src
                          ${PROJECT_SOURCE_DIR}/include               ) # Pasted into Doxyfile.in
#SET( doxy_examples        ${PROJECT_SOURCE_DIR}/examples              ) # Pasted into Doxyfile.in
SET( doxy_extra_files     ${PROJECT_SOURCE_DIR}/README.md   ) # Pasted into Doxyfile.in
#SET( doxy_exclude "${PROJECT_SOURCE_DIR}/include/HandlerMap.h" )
## If any header or source is modified, then rebuild the documentation
FOREACH( dir ${doxy_input} ${doxy_examples} )
  FILE( GLOB_RECURSE tmp_headers ${dir}/*.h   )
  FILE( GLOB_RECURSE tmp_sources ${dir}/*.cpp )
  SET ( file_dependencies ${file_dependencies} ${tmp_headers} ${tmp_sources} )
ENDFOREACH()
# Fix value lists before substitution...
JOIN("${doxy_input}" " " doxy_input)
JOIN("${doxy_exclude}" " " doxy_exclude)
CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )    

ADD_CUSTOM_COMMAND( OUTPUT ${doxy_html_index_file}
                    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                    # The following should be ${doxyfile} only but it will break the dependency.
                    # The optimal solution would be creating a custom_command for ${doxyfile}
                    # generation but I still have to figure out how...
                    MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
                    DEPENDS 
                    ${doxy_extra_files} ${ModernWheel_INSTALLED_HEADERS} 
                    COMMENT "Generating HTML documentation")

ADD_CUSTOM_TARGET( doc ALL DEPENDS ${doxy_html_index_file} )

INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc )  

INSTALL( 
  FILES ${PROJECT_SOURCE_DIR}/LICENSE
  DESTINATION share/license
)