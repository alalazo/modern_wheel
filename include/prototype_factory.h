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
 * @file prototype_factory.h
 * 
 * @brief Factory that registers prototype objects 
 * 
 * @author Massimiliano Culpo
 *
 * Created on March 13, 2015, 2:00 PM
 */

#ifndef PROTOTYPE_FACTORY_H_20150313
#define	PROTOTYPE_FACTORY_H_20150313

#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

namespace mwheel {

template<
class InterfaceType,
class TagType,
class ProductType = typename InterfaceType::clone_type
>
class PrototypeFactory {
private:
  using StoredType = std::shared_ptr<InterfaceType>;
  using PrototypeMap = std::map<TagType,StoredType >;
public:

  struct TagNotRegistered : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };
  using product_type = ProductType;

  PrototypeFactory() {
    m_on_tag_not_registered = PrototypeFactory::throw_if_tag_not_registered;
  }
  
  template< class ObjectType >
  bool register_prototype(const TagType& tag, ObjectType& obj) {
    /// @todo The line below requires objects to be copy constructible
    StoredType stored = std::make_shared<ObjectType>(obj);
    return m_prototype_map.insert(typename PrototypeMap::value_type(tag, stored)).second;
  }

  bool has_tag(const TagType& tag) const {
    
  }
  
  bool unregister_prototype(const TagType& tag) {
    
  }

  template<class... ParameterTypes>
  product_type create(const TagType& tag, ParameterTypes... parameters) {
    auto iterator = m_prototype_map.find(tag);
    if (iterator == m_prototype_map.end()) {
      return m_on_tag_not_registered(tag);
    }
    return iterator->second->clone(parameters...);
  }

  template<class U>
  void on_tag_not_registered(U action) {
    m_on_tag_not_registered = action;
  }
  
  static product_type throw_if_tag_not_registered(const TagType& tag) {
    std::stringstream estream;
    estream << "ERROR : in PrototypeFactory " << typeid(PrototypeFactory).name() << std::endl;
    estream << "\ttag \"" << tag << "\" not registered in the factory" << std::endl;
    estream << "\tMaybe you forgot to register a type?" << std::endl;
    return nullptr;
  }
  
private:
  std::function< product_type (const TagType&) > m_on_tag_not_registered;
  PrototypeMap m_prototype_map;
};

}

#endif	/* PROTOTYPE_FACTORY_H_20150313 */

