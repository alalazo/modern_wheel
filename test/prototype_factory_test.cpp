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

#include <prototype_factory.h>

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <string>

using namespace std;

namespace {

class Base {
public:
  using clone_type = shared_ptr<Base>;
  virtual clone_type clone() = 0;
  virtual int get() = 0;
};

struct DerivedA : public Base{

  Base::clone_type clone() {
    return make_shared<DerivedA>();
  }

  virtual int get() {
    return 1;
  }

};

}

BOOST_AUTO_TEST_SUITE(PrototypeFactoryTest)
BOOST_AUTO_TEST_CASE(NoParameters)
{
  // Create a factory
  using FactoryType = mwheel::PrototypeFactory<Base,string>;
  FactoryType factory;
  // Register types
  DerivedA a;
  BOOST_CHECK_EQUAL(factory.register_prototype("DerivedA",a),true);
  // Check that the correct object is created
  auto obj = factory.create("DerivedA");
  BOOST_CHECK_EQUAL(obj->get(),1);
  // Check 
}
BOOST_AUTO_TEST_SUITE_END()