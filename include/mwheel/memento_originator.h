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
 * @file memento_originator.h
 * 
 * @brief Permits to checkpoint and recover the state of an object
 * 
 * @author Massimiliano Culpo
 *
 * Created on April 2, 2015, 11:22 AM
 */

#ifndef MEMENTO_ORIGINATOR_H_20150402
#define	MEMENTO_ORIGINATOR_H_20150402

#include <memory>

namespace mwheel {

/**
 * @brief Creates checkpoint objects and recover a previous state from them
 * 
 * @tparam T type of the memento (may be abstract)
 */
template< class T >
class MementoOriginator {
public:
  /// Type of the memento associated with the interface
  using memento_type = std::shared_ptr< T >;
  
  /**
   * @brief Creates a memento of the appropriate type
   * 
   * @return opaque token that contains information on the state of the object
   */
  virtual memento_type create_memento() const = 0;
  
  /**
   * @brief Set the state of the object from a token
   * 
   * @param[in] token memento of the appropriate type
   */
  virtual void set_state(memento_type token) = 0;
  
  /**
   * @brief The infamous virtual destructor
   */
  virtual ~MementoOriginator() {
  }

};

}

#endif	/* MEMENTO_ORIGINATOR_H_20150402 */

