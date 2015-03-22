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
 * @file internal_extension.h
 * 
 * @brief Mimics an internal extension that will be loaded at run-time
 * 
 * @author Massimiliano Culpo
 *
 * Created on March 20, 2015, 1:27 PM
 */

#ifndef INTERNAL_EXTENSION_H_20150320
#define	INTERNAL_EXTENSION_H_20150320

#include <fixtures/client_interface.h>

namespace mwheel {
namespace test {

class InternalExtension : public mwheel::test::ClientInterface {
public:

  int get() override {
    return m_int;
  }

  ClientInterface::clone_type clone() {
    return std::make_shared<InternalExtension>();
  }

private:
  int m_int = 20;
  REGISTRABLE_PRODUCT;
};


}
}

#endif	/* INTERNAL_EXTENSION_H_20150320 */

