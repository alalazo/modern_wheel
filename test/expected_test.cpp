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
 * @file expected_test.cpp
 *
 * @brief Unit tests for Expected<T>
 *
 * @author Massimiliano Culpo
 *
 * Created on September 30, 2015, 22:50 PM
 */

#include <mwheel/expected.h>

#include <boost/test/unit_test.hpp>

using namespace std;

namespace {

mwheel::Expected<int> foo() { return 2; }

float bar(bool throw_exception = false) {
  if (throw_exception) {
    throw std::runtime_error("This is a runtime error!");
  }
  return 1.0f;
}
}

BOOST_AUTO_TEST_SUITE(ExpectedTest)
BOOST_AUTO_TEST_CASE(ConstructFromValidObject) {
  // Construct from lvalue
  auto a = foo();
  BOOST_CHECK_EQUAL(a.valid(), true);
  BOOST_CHECK_EQUAL(a.get(), 2);
  // Construct from rvalue
  auto b = mwheel::Expected<float>(2.1f);
  BOOST_CHECK_EQUAL(b.valid(), true);
  BOOST_CHECK_EQUAL(b.get(), 2.1f);
  // Copy construct
  auto c = b;
  BOOST_CHECK_EQUAL(c.valid(), true);
  BOOST_CHECK_EQUAL(c.get(), 2.1f);
  // Move construct
  auto d = std::move(b);
  BOOST_CHECK_EQUAL(d.valid(), true);
  BOOST_CHECK_EQUAL(d.get(), 2.1f);
  // An exception is expected
  auto e = mwheel::Expected<std::runtime_error>(std::runtime_error("Exception is the good value"));
  BOOST_CHECK_EQUAL(e.valid(), true);
  // Construct from code
  auto f = mwheel::Expected<float>::from_code([]() { return bar(); });
  BOOST_CHECK_EQUAL(f.valid(), true);
  BOOST_CHECK_EQUAL(f.get(), 1.0f);
}
BOOST_AUTO_TEST_CASE(ConstructFromException) {
  // Construct from an exception directly
  auto a = mwheel::Expected<float>::from_exception(std::invalid_argument("Invalid argument!"));
  BOOST_CHECK_EQUAL(a.has_exception<std::invalid_argument>(), true);
  BOOST_CHECK_EQUAL(a.has_exception<std::runtime_error>(), false);
  BOOST_CHECK_EQUAL(a.valid(), false);
  BOOST_CHECK_THROW(a.get(), std::invalid_argument);
  // Construct from code
  auto b = mwheel::Expected<float>::from_code([]() { return bar(true); });
  BOOST_CHECK_EQUAL(b.valid(), false);
  BOOST_CHECK_EQUAL(b.has_exception<std::runtime_error>(), true);
  BOOST_CHECK_EQUAL(b.has_exception<std::exception>(), true);
  BOOST_CHECK_THROW(b.get(), std::runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()
