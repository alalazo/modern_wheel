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
 * @file serializable_object.h
 *
 * @brief Permits to serialize objects to and from disk
 *
 * @author Massimiliano Culpo
 *
 * Created on April 2, 2015, 11:52 AM
 */

#ifndef SERIALIZABLE_OBJECT_H_20150402
#define SERIALIZABLE_OBJECT_H_20150402

#include <boost/filesystem.hpp>

namespace mwheel {

/**
 * @brief Serializes object to disk and de-serializes from disk
 */
class SerializableObject {
public:
  /**
   * @brief Serializes the state of an object
   *
   * @param[in] path name of the file where to serialize object state
   */
  virtual void serialize(const boost::filesystem::path &path) const = 0;

  /**
   * @brief De-serializes the state of an object
   *
   * @param[in] path name of the file where the state is stored
   */
  virtual void deserialize(const boost::filesystem::path &path) = 0;

  /**
   * @brief The infamous virtual destructor
   */
  virtual ~SerializableObject();
};
}

#endif /* SERIALIZABLE_OBJECT_H_20150402 */