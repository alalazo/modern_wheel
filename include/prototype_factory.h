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

/**
 * @brief Defines a way to map values to types, and defers instantiation
 * of concrete objects until run-time
 * 
 * The prototype factory permits to register objects:
 * - that implement the same abstract interface
 * - that fulfill the prototype contract
 * 
 * To fulfill the prototype contract the abstract interface must expose 
 * at least a virtual method named `clone` which takes an arbitrary number 
 * of parameters and return an object of type ProductType
 * 
 * The action that will be taken in case the creation of a non registered type
 * is queried can be customized. The default is to throw an exception of type 
 * PrototypeFactory::tag_not_registered.
 * 
 * @tparam InterfaceType interface type common to all the registered objects
 * @tparam TagType type of the values that will be associated with each registered object
 * @tparam ProductType type returned by the `clone` function
 */    
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
  /// Exception thrown by default when trying to create a type that was not registered
  struct tag_not_registered : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };
  /// Type of the product
  using product_type = ProductType;

  /**
   * @brief Default constructor 
   */
  PrototypeFactory() {
    m_on_tag_not_registered = PrototypeFactory::throw_if_tag_not_registered;
  }
  
  /**
   * @brief Registers an object in the factory
   * 
   * @param[in] tag tag associated with the object
   * @param[in] obj object to be registered
   * 
   * @return true if the registration was successful, false otherwise
   */
  template< class ObjectType >
  bool register_prototype(const TagType& tag, const ObjectType& obj) {
    /// @warning The current implementation requires objects to be copy constructible
    StoredType stored = std::make_shared<ObjectType>(obj);
    return m_prototype_map.insert(typename PrototypeMap::value_type(tag, stored)).second;
  }

  /**
   * @brief Predicate that checks whether a tag is registered or not
   * 
   * @param[in] tag tag to be queried
   * 
   * @return true if the tag was registered in the factory, false otherwise
   */
  bool has_tag(const TagType& tag) const {
    auto it = m_prototype_map.find(tag);
    if( it != m_prototype_map.end() ) {      
      // If found return true
      return true;
    }
    return false;
  }
  
  /**
   * @brief Unregister an object from the factory
   * 
   * @param[in] tag tag associated with the object to be unregistered
   * 
   * @return true if one object is removed, false otherwise
   */
  bool unregister_prototype(const TagType& tag) {
    auto nremoved = m_prototype_map.erase(tag);
    if (nremoved == 1) {
      return true;
    }
    return false;
  }

  /**
   * @brief Creates an object on based on a requested tag
   * 
   * If the tag was not registered a call to a customizable function is made.
   * The default behavior is to throw an exception of type PrototypeFactory::tag_not_registered.
   * 
   * @param[in] tag tag associated with the object to be created
   * @param[in] parameters parameters needed for the creation
   * 
   * @return instance of the created object
   */
  template<class... ParameterTypes>
  product_type create(const TagType& tag, ParameterTypes... parameters) {
    auto iterator = m_prototype_map.find(tag);
    if (iterator == m_prototype_map.end()) {
      return m_on_tag_not_registered(tag);
    }
    return iterator->second->clone(parameters...);
  }

  /**
   * @brief Sets which action will be taken in case a tag was not 
   * found during creation
   * 
   * The action must be a callable object of whatever type which:
   * - returns an object of type PrototypeFactory::product_type
   * - takes as the only parameter the tag that was not found
   * 
   * @param[in] action generic function to be called 
   */
  template<class U>
  void on_tag_not_registered(U action) {
    m_on_tag_not_registered = action;
  }
  
  /**
   * @brief Default behavior for the action to be performed when a tag 
   * was not found during the creation of an object
   * 
   * @param[in] tag the tag that was not found
   * 
   * @throw PrototypeFactory::tag_not_registered always
   * 
   * @return nothing
   */
  static product_type throw_if_tag_not_registered(const TagType& tag) {
    std::stringstream estream;
    estream << "ERROR : in PrototypeFactory " << typeid(PrototypeFactory).name() << std::endl;
    estream << "\ttag \"" << tag << "\" not registered in the factory" << std::endl;
    estream << "\tMaybe you forgot to register a type?" << std::endl;
    throw tag_not_registered( estream.str() );
    return nullptr;
  }
  
private:
  /// Generalized function to be called when a tag is not found during creation
  std::function< product_type (const TagType&) > m_on_tag_not_registered;
  /// Map that stores tag,object pairs
  PrototypeMap m_prototype_map;
};

}

#endif	/* PROTOTYPE_FACTORY_H_20150313 */

