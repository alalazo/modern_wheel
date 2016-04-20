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
 * @file singleton_test.cpp
 *
 * @brief Unit tests for the singleton pattern
 *
 * @author Massimiliano Culpo
 *
 * Created on March 2, 2015, 3:50 PM
 */

#include <mwheel/singleton.h>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;

namespace {

struct A {

  A(int a) : m_int(a) {}

  int get() { return m_int; }

private:
  int m_int;
};
}

BOOST_AUTO_TEST_SUITE(SingletonTest)
BOOST_AUTO_TEST_CASE(ClassWithDefaultConstructor) {
  //////////
  // CLASS WITH DEFAULT CONSTRUCTOR
  using TheInt = mwheel::Singleton<int>;
  // Check the presence of default constructor
  BOOST_CHECK_EQUAL(TheInt::get_instance(), 0);
  // Check that the instance is not reset
  TheInt::set_creator([] { return make_shared<int>(10); });
  BOOST_CHECK_EQUAL(TheInt::get_instance(), 0);
  // Check that the instance is reset
  TheInt::set_creator([] { return make_shared<int>(11); });
  BOOST_CHECK_EQUAL(TheInt::get_instance(true), 11);
  //////////
}

BOOST_AUTO_TEST_CASE(ClassWithoutDefaultConstructor) {
  //////////
  // CLASS WITHOUT DEFAULT CONSTRUCTOR
  using TheA = mwheel::Singleton<A>;
  // Check that the class throws
  BOOST_CHECK_THROW(TheA::get_instance(), TheA::creator_not_set);
  // Check that the instance is reset
  TheA::set_creator([] { return make_shared<A>(11); });
  BOOST_CHECK_EQUAL(TheA::get_instance(true).get(), 11);
  //////////
}
BOOST_AUTO_TEST_SUITE_END()