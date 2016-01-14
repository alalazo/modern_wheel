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

#include <mwheel/dlmanager.h>

#include <boost/predef.h>

#include <iostream>
#include <sstream>
#include <typeinfo>
#include <utility>


#if BOOST_OS_UNIX
#include <dlfcn.h>
#elif BOOST_OS_WINDOWS
#include "Windows.h"
#else
#error "Unsupported OS. Supported OS are : Unix-like, Windows"
#endif

using namespace std;

namespace mwheel {

void DLManager::load_library(const boost::filesystem::path &library_path) {
#if BOOST_OS_UNIX
  auto handle = dlopen(library_path.c_str(), RTLD_LAZY);
  auto error_message = dlerror();
  if (error_message) {
    stringstream estream;
    estream << "ERROR : cannot load shared library " << library_path << endl;
    estream << "\t" << error_message << endl;
    throw error_loading_dynamic_library(estream.str());
  }
#elif BOOST_OS_WINDOWS
  auto handle = LoadLibrary( library_path.string().c_str() ); // Always ensure a conversion to const char * (boost::filesystem may be const wchar_t *)
  if (!handle) {
	auto error_message = GetLastError();
	stringstream estream;
	estream << "ERROR : cannot load shared library " << library_path << endl;
	estream << "\t" << error_message << endl;
	throw error_loading_dynamic_library(estream.str());
  }
#endif
  m_dl_map.insert(make_pair(library_path, handle));
}

void DLManager::unload_library(const boost::filesystem::path &library_path) {
  auto it = DLMap::iterator(m_dl_map.find(library_path));
  // The library was not found
  if (it == m_dl_map.end()) {
    stringstream estream;
    estream << "ERROR : cannot trying to unload a shared library that was not previously loaded "
            << library_path << endl;
    estream << "\tDid you use a wrong name for the library to be unloaded?" << endl;
    throw library_not_loaded(estream.str());
  }
#if BOOST_OS_UNIX
  // Close the library
  if (dlclose(it->second)) {
    auto error_message = dlerror();
    stringstream estream;
    estream << "ERROR : cannot unload shared library " << library_path << endl;
    estream << "\t" << error_message << endl;
    throw error_unloading_dynamic_library(estream.str());
  }
#elif BOOST_OS_WINDOWS
  // Close the library
  if (FreeLibrary(static_cast<HMODULE>(it->second))) {
	  auto error_message = GetLastError();
	  stringstream estream;
	  estream << "ERROR : cannot unload shared library " << library_path << endl;
	  estream << "\t" << error_message << endl;
	  throw error_unloading_dynamic_library(estream.str());
  }
#endif
  m_dl_map.erase(it);
}

DLManager::~DLManager() {
#if BOOST_OS_UNIX
  for (auto &x : m_dl_map) {
    dlclose(x.second);
    auto error_message = dlerror();
    if (error_message) {
      cerr << "ERROR : cannot unload shared library " << x.first << endl;
      cerr << "\t" << error_message << endl;
    }
  }
#elif BOOST_OS_WINDOWS
  for (auto &x : m_dl_map) {
	FreeLibrary(static_cast<HMODULE>(x.second));
	auto error_message = GetLastError();
	if (error_message) {
	  cerr << "ERROR : cannot unload shared library " << x.first << endl;
	  cerr << "\t" << error_message << endl;
	}
  }
#endif
}
}
