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

#include <boost/filesystem.hpp>

#include <map>
#include <stdexcept>

namespace mwheel {

class DLManager {
public:

  class error_loading_dynamic_library : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
  };

  class error_unloading_dynamic_library : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
  };

  void load_library(boost::filesystem::path library_path);

  ~DLManager();

private:
  std::map< boost::filesystem::path, void *> m_dl_map;
};
}

#endif	/* DLMANAGER_H_20150317 */

