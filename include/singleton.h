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
 * @file singleton.h
 * 
 * @brief Generic implementation of the singleton pattern
 * 
 * @author Massimiliano Culpo
 *
 * Created on March 2, 2015, 3:42 PM
 */

#ifndef SINGLETON_H_20150312
#define	SINGLETON_H_20150312

#include <mwheel_utility.h>

#include <functional>
#include <memory>
#include <sstream>
#include <typeinfo>
#include <type_traits>

/**
 * @brief Main namespace of the library
 */
namespace mwheel {

/**
 * @brief Ensures a class only has one instance, 
 * and provides a global point of access to it
 * 
 * @tparam T class that should have a single instance
 */
template< class T>
class Singleton {
public:
  /**
   * @brief Exception thrown when the creator was not set
   */
  MWHEEL_EXCEPTION(creator_not_set);  
  /// Type of the singleton creator  
  using creator_type = std::function< std::shared_ptr<T> (void) >;

  /**
   * @brief Sets the creator that will be used to construct the single instance
   * 
   * @param[in] creator anything that can bind to creator_type
   */
  template<class U>
  static void set_creator(U creator) {
    m_creator = creator;
  }

  /**
   * @brief Returns the single instance of T
   * 
   * This method performs a call to the creator function if:
   * - the singleton was not already created (lazy initialization)
   * - an explicit request to reset the singleton state is made 
   * 
   * @param[in] reset if true resets the instance state 
   * performing a call to the creator
   * 
   * @return single instance of T
   */
  static T& get_instance(bool reset = false) {
    static auto pnt = std::shared_ptr<T>(nullptr);
    if (!pnt || reset) {
      check_creator();      
      pnt = m_creator();
    }
    return *pnt;
  }

private:
  
  Singleton() = delete;  
  Singleton(const Singleton&) = delete;
  
  /**
   * @brief Checks if the creator is set to a valid value
   * 
   * @throw CreatorNotSet creator was not set to a valid value
   */
  static void check_creator() {
    if ( !m_creator ) {
      // Using auto with an std::stringstream is a PITA!
      std::stringstream estream;
      estream << "ERROR : in Singleton " << typeid(Singleton).name() << std::endl;
      estream << "\tcreator not set" << std::endl;
      estream << "\tMaybe you forgot to call \"set_creator\" before retrieving the singleton instance?" << std::endl;
      throw creator_not_set( estream.str() );
    }
  }
  /// Creator of the singleton object
  static creator_type m_creator;
};

namespace {

/**
 * @brief If the object is default constructible, initializes the creator
 * with the default constructor
 * 
 * @return creator object
 */
template< class T>
typename std::enable_if< std::is_default_constructible<T>::value, typename Singleton<T>::creator_type >::type
initialize_creator() {
  return [] {
    return std::make_shared<T>();
  };
}

/**
 * @brief If the object is not default constructible, initializes the creator
 * to nullptr
 * 
 * @return nullptr
 */
template< class T>
typename std::enable_if< !std::is_default_constructible<T>::value, typename Singleton<T>::creator_type >::type
initialize_creator() {
    return nullptr;  
}

}

template<class T>
typename Singleton<T>::creator_type Singleton<T>::m_creator = initialize_creator<T>();

}

#endif	/* SINGLETON_H_20150312 */

