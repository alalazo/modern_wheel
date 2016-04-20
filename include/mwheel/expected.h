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
 * @file expected.h
 *
 * @brief Implementation of expected as presented by A. Alexandrescu
 * at CppAndBeyond 2012
 *
 * @author Massimiliano Culpo
 *
 * Created on September 29, 2015, 11:10 PM
 */

#ifndef EXPECTED_H_20150929
#define EXPECTED_H_20150929

#include <stdexcept>
#include <typeinfo>
#include <utility>

namespace mwheel {

/**
 * @brief Either a value of type T or the exception that prevented
 * its generation
 *
 * @tparam T type of the expected value
 */
template <class T> class Expected {
public:
  /**
   * @brief Constructs from a good value
   *
   * @param[in] rhs encapsulated value
   */
  Expected(const T &rhs) : m_value(rhs), m_got_exception(false) {}

  /**
   * @brief Constructs from a good value
   *
   * @param[in] rhs encapsulated value
   */
  Expected(T &&rhs) : m_value(std::move(rhs)), m_got_exception(false) {}

  /**
   * @brief Copy constructor
   *
   * @param[in] rhs object of type expected
   */
  Expected(const Expected &rhs) : m_got_exception(rhs.m_got_exception) {
    if (m_got_exception) {
      new (&m_exception_ptr) std::exception_ptr(rhs.m_exception_ptr);
    } else {
      new (&m_value) T(rhs.m_value);
    }
  }

  /**
   * @brief Move constructor
   *
   * @param[in] rhs object of type expected
   */
  Expected(Expected &&rhs) : m_got_exception(rhs.m_got_exception) {
    if (m_got_exception) {
      new (&m_exception_ptr) std::exception_ptr(std::move(rhs.m_exception_ptr));
    } else {
      new (&m_value) T(std::move(rhs.m_value));
    }
  }

  void swap(Expected &rhs) {
    if (m_got_exception) {
      if (rhs.m_got_exception) {
        // Swap the exception pointers
        m_exception_ptr.swap(rhs.m_exception_ptr);
      } else {
        // Recurse to case below
        rhs.swap(*this);
      }
    } else {
      if (rhs.m_got_exception) {
        // Copy exception ptr to a temporary
        auto t = std::move(rhs.m_exception_ptr);
        // Swap good value with bogus
        new (&rhs.m_value) T(std::move(m_value));
        new (&m_exception_ptr) std::exception_ptr(t);
        // Swap booleans
        std::swap(m_got_exception, rhs.m_got_exception);
      } else {
        // Bring std::swap into scope for lookup availability...
        using std::swap;
        // ... and let ADL select a winner
        swap(m_value, rhs.m_value);
      }
    }
  }

  ~Expected() {
    using std::exception_ptr;
    if (m_got_exception) {
      m_exception_ptr.~exception_ptr();
    } else {
      m_value.~T();
    }
  }

  template <class E> static Expected<T> from_exception(const E &exception) {
    if (typeid(exception) != typeid(E)) {
      throw std::invalid_argument("ERROR : exception slicing detected!");
    }
    return from_exception(std::make_exception_ptr(exception));
  }

  static Expected<T> from_exception(std::exception_ptr exception_ptr = std::current_exception()) {
    Expected<T> result;
    result.m_got_exception = true;
    new (&result.m_exception_ptr) std::exception_ptr(std::move(exception_ptr));
    return result;
  }

  bool valid() const { return !m_got_exception; }

  const T &get() const {
    if (m_got_exception) {
      std::rethrow_exception(m_exception_ptr);
    } else {
      return m_value;
    }
  }

  T &get() { return const_cast<T &>(static_cast<const Expected &>(*this).get()); }

  template <class E> bool has_exception() const {
    try {
      if (m_got_exception) {
        std::rethrow_exception(m_exception_ptr);
      }
    } catch (const E &) {
      return true;
    } catch (...) {
      return false;
    }
    return false;
  }

  template <class F> static Expected from_code(F f) {
    try {
      return Expected(f());
    } catch (...) {
      return from_exception();
    }
  }

private:
  union {
    T m_value;
    std::exception_ptr m_exception_ptr;
  };
  bool m_got_exception;
  Expected() {}
};
}

#endif /* EXPECTED_H_20150929 */
