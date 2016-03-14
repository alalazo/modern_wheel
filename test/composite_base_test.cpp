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

#include <mwheel/composite_base.h>

#include <boost/test/unit_test.hpp>

using namespace std;

namespace {
class Base {
public:
  virtual int get() const = 0;
  virtual ~Base() {}
};

class Get3 : public Base {
public:
  int get() const override { return 3; }
};

class Get5 : public Base {
public:
  int get() const override { return 5; }
};

class Composite : public mwheel::CompositeBase<Base> {
public:
  int get() const override {
    auto sum = 0;
    for (const auto &x : m_items) {
      sum += x->get();
    }
    return sum;
  }
};
}

BOOST_AUTO_TEST_SUITE(CompositeBaseTest)
BOOST_AUTO_TEST_CASE(BasicOperations) {
  // Create a composite
  auto get_3 = make_shared<Get3>();
  auto get_5 = make_shared<Get5>();
  auto composite = make_shared<Composite>();
  // Check push back
  BOOST_CHECK_EQUAL(composite->empty(), true);
  BOOST_CHECK_EQUAL(composite->size(), 0);
  composite->push_back(get_3);
  BOOST_CHECK_EQUAL(composite->empty(), false);
  BOOST_CHECK_EQUAL(composite->size(), 1);
  BOOST_CHECK_EQUAL(composite->get(), 3);
  composite->push_back(get_3);
  BOOST_CHECK_EQUAL(composite->empty(), false);
  BOOST_CHECK_EQUAL(composite->size(), 2);
  BOOST_CHECK_EQUAL(composite->get(), 6);
  composite->push_back(get_5);
  BOOST_CHECK_EQUAL(composite->empty(), false);
  BOOST_CHECK_EQUAL(composite->size(), 3);
  BOOST_CHECK_EQUAL(composite->get(), 11);
  // Check at method
  BOOST_CHECK_EQUAL(composite->at(0)->get(), 3);
  BOOST_CHECK_EQUAL(composite->at(1)->get(), 3);
  BOOST_CHECK_EQUAL(composite->at(2)->get(), 5);
  BOOST_CHECK_THROW(composite->at(3), std::out_of_range);
  // Check clear method
  composite->clear();
  BOOST_CHECK_EQUAL(composite->empty(), true);
  BOOST_CHECK_EQUAL(composite->size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()