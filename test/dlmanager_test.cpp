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

#include <dlmanager.h>

#include <fixtures/client_interface.h>

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(DLManagerTest)
BOOST_AUTO_TEST_CASE(DynamicLibraryLoading)
{
  using TheFactory = mwheel::test::ClientInterface::factory_type;
  mwheel::DLManager manager;
  // Load an external plug-in
  auto external_path = boost::filesystem::path("@CMAKE_CURRENT_BINARY_DIR@/fixtures/libplugin_extension_test.so");
  manager.load_library(external_path);
  auto external_object = TheFactory::get_instance().create("PluginExtension");
  BOOST_CHECK_EQUAL(external_object->get(),10);
  // Load an internal extension
  auto internal_object = TheFactory::get_instance().create("InternalExtension");
  BOOST_CHECK_EQUAL(internal_object->get(),20);
  // Check error on non existent library
  BOOST_CHECK_THROW(manager.load_library("this_does_not_exist.so"),mwheel::DLManager::error_loading_dynamic_library);
}
BOOST_AUTO_TEST_CASE(DynamicLibraryUnloading)
{
  auto external_path = boost::filesystem::path("@CMAKE_CURRENT_BINARY_DIR@/fixtures/libplugin_extension_test.so");  
  mwheel::DLManager manager;
  // Unload non-existent library
  BOOST_CHECK_THROW(manager.unload_library("this_does_not_exist.so"),mwheel::DLManager::library_not_loaded);
  // Unload loaded library
  manager.load_library(external_path);
  BOOST_CHECK_NO_THROW( manager.unload_library(external_path) );
}
BOOST_AUTO_TEST_SUITE_END()