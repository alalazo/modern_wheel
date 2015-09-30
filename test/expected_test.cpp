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
#include <boost/test/unit_test_suite.hpp>

using namespace std;

namespace {

mwheel::Expected<int> foo() {
  return 2;
}

}

BOOST_AUTO_TEST_SUITE(ExpectedTest)
BOOST_AUTO_TEST_CASE(ValidObject) {
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
}
BOOST_AUTO_TEST_SUITE_END()

