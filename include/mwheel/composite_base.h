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
 * @file composite_base.h
 * 
 * @brief Gathers together all the basic operations of a composite object
 * 
 * @author Massimiliano Culpo
 *
 * Created on June 4, 2015, 10:06 AM
 */

#ifndef COMPOSITE_BASE_H_20150604
#define	COMPOSITE_BASE_H_20150604

#include <memory>
#include <vector>

namespace mwheel {
          
/**
 * @brief Implements all the common operations needed by a composite object
 */
template< 
class InterfaceType,
class StoredType = std::shared_ptr<InterfaceType>
>
class CompositeBase : public InterfaceType
{    
  using ContainerType = std::vector<StoredType>;    
public:
    
  /**
   * @brief Returns a reference to the element at specified location, 
   * with bounds checking
   * 
   * @param[in] position specified location
   * 
   * @throws std::out_of_range if position is out of the range of the stored items
   * 
   * @return reference to the element at specified location
   */
  typename ContainerType::const_reference at(typename ContainerType::size_type position) const
  {
    return m_items.at(position);
  }
        
  /**
   * @brief Returns a reference to the element at specified location, 
   * with bounds checking
   * 
   * @param[in] position specified location
   * 
   * @throws std::out_of_range if position is out of the range of the stored items
   * 
   * @return reference to the element at specified location
   */
  typename ContainerType::reference at(typename ContainerType::size_type position)
  {
    return const_cast<typename ContainerType::reference>( 
             static_cast<const CompositeBase&>(*this).m_items.at(position)
           );
  }
    
  /**
   * @brief Checks if the container has no elements
   * 
   * @return true if the container has no elements, false otherwise
   */
  bool empty() const /* noexcept */
  {
    return m_items.empty();
  }
    
  /**
   * @brief Returns the number of elements in the container
   * 
   * @return number of elements in the container
   */
  typename ContainerType::size_type size() const /* noexcept */
  {
    return m_items.size();
  }
    
  /**
   * @brief Removes all elements from the composite
   */
  void clear() /* noexcept */
  {
    m_items.clear();
  }
    
  /**
   * @brief Appends a new item to the end of 
   * 
   * @param[in] item item to be appended
   */
  template< class T >
  void push_back(T&& item)
  {
    m_items.push_back( std::forward<T>(item) );
  }
    
protected:
  ContainerType m_items;    
};
}

#endif	/* COMPOSITE_BASE_H_20150604 */

