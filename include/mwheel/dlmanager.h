/**
 * 
 * Modern Wheel : all the things that shouldn't be reinvented from one project to the other
 * 
 * The MIT License (MIT)
 * 
 * Copyright (C) 2015  Massimiliano Culpo
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/**
 * @file dlmanager.h
 * 
 * @brief Class that manages the run-time loading of dynamic libraries
 * 
 * @author Massimiliano Culpo
 *
 * Created on March 17, 2015, 10:31 AM
 */

#ifndef DLMANAGER_H_20150317
#define	DLMANAGER_H_20150317

#include <mwheel/utility.h>

#include <boost/filesystem.hpp>

#include <map>

namespace mwheel {

/**
 * @brief Manages dynamic loading of shared libraries
 * 
 * @warning Current limitations:
 * - works only for POSIX systems
 * - external libraries and client application must be built with a 
 * compatible compiler (there is no C-ABI layer)
 * 
 */
class DLManager {
public:
  /// @brief Exception thrown if an error occurred when loading a shared library  
  MWHEEL_EXCEPTION(error_loading_dynamic_library);
  /// @brief Exception thrown if an error occurred when unloading a shared library
  MWHEEL_EXCEPTION(error_unloading_dynamic_library);
  /// @brief Exception thrown when trying to unload a library that was not previously loaded
  MWHEEL_EXCEPTION(library_not_loaded);
  
  /**
   * @brief Loads the shared library specified by the given path
   * 
   * @param[in] library_path path of the library to be loaded
   * 
   * @throw error_loading_dynamic_library exception thrown if an error 
   * occurred during the loading operation
   */
  void load_library(const boost::filesystem::path& library_path);
  
  /**
   * @brief Unloads the shared library specified by the given path
   * 
   * @param[in] library_path path of the library to be unloaded
   * 
   * @throw library_not_loaded exception thrown if the library was not 
   * previously loaded
   * 
   * @throw error_unloading_dynamic_library exception thrown if an error 
   * occurred during the unloading operation
   * 
   * @warning Trying to unload a library that introduces classes in the 
   * application that are currently in scope may cause memory access violations
   */
  void unload_library(const boost::filesystem::path& library_path);
  
  /**
   * @brief Unloads all the libraries loaded so far
   */
  ~DLManager();

private:
  /// Type used to store loaded libraries
  using DLMap = std::map< boost::filesystem::path, void *>;
  /// Map to store library handles once loaded
  DLMap m_dl_map;
};
}

#endif	/* DLMANAGER_H_20150317 */

