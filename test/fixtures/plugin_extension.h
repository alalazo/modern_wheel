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
 * @file plugin_extension.h
 *
 * @brief Mimics an external extension that will be loaded at run-time
 *
 * @author Massimiliano Culpo
 *
 * Created on March 18, 2015, 8:47 AM
 */

#ifndef PLUGIN_EXTENSION_H
#define PLUGIN_EXTENSION_H

#include <fixtures/client_interface.h>

#include <algorithm>
#include <functional>
#include <vector>

namespace mwheel {
namespace test {

class PluginExtension : public ClientInterface {
public:
  int get() override;

  ClientInterface::clone_type clone() override;

private:
  int m_int = 10;
  MWHEEL_REGISTRABLE_PRODUCT;
};
}
}

#endif /* PLUGIN_EXTENSION_H */
