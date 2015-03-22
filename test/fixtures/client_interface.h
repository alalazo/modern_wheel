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
 * @file client_interface.h
 * 
 * @brief Dummy client interface to test DLManager
 * 
 * @author Massimiliano Culpo
 *
 * Created on March 18, 2015, 8:39 AM
 */

#ifndef CLIENT_INTERFACE_H_20150318
#define	CLIENT_INTERFACE_H_20150318

#include <mwheel/plugin.h>

#include <memory>

namespace mwheel {
namespace test {

class ClientInterface {
public:
  using clone_type = std::shared_ptr<ClientInterface>;
  EXPOSE_INTERFACE_FACTORY(ClientInterface, std::string);

  virtual int get() = 0;
  virtual clone_type clone() = 0;

  virtual ~ClientInterface() {
  }

};

}
}

#endif	/* CLIENT_INTERFACE_H_20150318 */

