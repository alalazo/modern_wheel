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

  Base::clone_type clone() override {
    return make_shared<DerivedA>();
  }

  int get() override {
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
  // Registering a type under a new tag should work
  BOOST_CHECK_EQUAL(factory.register_prototype("DerivedA",a),true);
  BOOST_CHECK_EQUAL(factory.register_prototype("SDerivedA",make_shared<DerivedA>()),true);
  // Trying to register under the same tag should fail
  BOOST_CHECK_EQUAL(factory.register_prototype("DerivedA",a),false);  
  BOOST_CHECK_EQUAL(factory.register_prototype("SDerivedA",make_shared<DerivedA>()),false);
  // Check that the correct object is created
  auto obj = factory.create("DerivedA");
  BOOST_CHECK_EQUAL(obj->get(),1);  
  auto sobj = factory.create("DerivedA");
  BOOST_CHECK_EQUAL(sobj->get(),1);  
  // Unregister an existent tag should work
  BOOST_CHECK_EQUAL(factory.has_tag("DerivedA"),true);
  BOOST_CHECK_EQUAL(factory.unregister_prototype("DerivedA"),true);  
  // Unregister a non-existent tag should fail
  BOOST_CHECK_EQUAL(factory.has_tag("DerivedA"),false);
  BOOST_CHECK_EQUAL(factory.unregister_prototype("DerivedA"),false);  
  // Check the default behavior with throws
  BOOST_CHECK_THROW(factory.create("DerivedA"),FactoryType::tag_not_registered);
  // Check customization of on_tag_not_registered
  factory.on_tag_not_registered([](FactoryType::tag_type tag){ return nullptr; });
  BOOST_CHECK_EQUAL(factory.create("DerivedA"),FactoryType::product_type(nullptr));  
}
BOOST_AUTO_TEST_SUITE_END()