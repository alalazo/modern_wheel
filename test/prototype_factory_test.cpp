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

#include <mwheel/prototype_factory.h>

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
  virtual ~Base() {
  }
};

struct DerivedA : public Base{

  Base::clone_type clone() override {
    return make_shared<DerivedA>();
  }

  int get() override {
    return 1;
  }

};

class BaseMultiParms {
public:
  using clone_type = shared_ptr<BaseMultiParms>;
  virtual clone_type clone(int a) = 0;
  virtual clone_type clone(int a, int b) = 0;
  virtual int get() = 0;
};

class DerivedSum : public BaseMultiParms {
public:
  explicit DerivedSum(int a) : m_a(a){}
    
  BaseMultiParms::clone_type clone(int a) override {
    return make_shared<DerivedSum>(a);
  }
  
  BaseMultiParms::clone_type clone(int a, int b) override {
    return make_shared<DerivedSum>(a+b);
  }
  
  int get() override {
    return m_a;
  }
  
private:
  int m_a = 0;
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
  // Check the construction of the list of products
  auto products = factory.product_list();
  BOOST_CHECK_EQUAL(products.size(),2);
  BOOST_CHECK_EQUAL(products[0],"DerivedA");
  BOOST_CHECK_EQUAL(products[1],"SDerivedA");  
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
BOOST_AUTO_TEST_CASE(MultipleParameters)
{
  // Create a factory and register a type
  using FactoryType = mwheel::PrototypeFactory<BaseMultiParms,int>;
  FactoryType factory;
  BOOST_CHECK_EQUAL(factory.register_prototype(0,make_shared<DerivedSum>(10)),true);
  // Check creation using multiple parameters
  auto obja = factory.create(0,3);
  BOOST_CHECK_EQUAL(obja->get(),3);  
  auto objb = factory.create(0,3,6);
  BOOST_CHECK_EQUAL(objb->get(),9);    
}
BOOST_AUTO_TEST_SUITE_END()