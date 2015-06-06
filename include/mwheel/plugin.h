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
 * @file plugin.h
 *
 * @brief Contains macro to simplify the creation of plug-ins that are
 * loadable at run-time
 *
 * @author Massimiliano Culpo
 *
 * Created on March 22, 2015, 10:26 PM
 */

#ifndef PLUGIN_H_20150322
#define PLUGIN_H_20150322

#include <mwheel/singleton.h>
#include <mwheel/prototype_factory.h>

#include <vector>

/**
 * @brief Must be used inside the public part of an interface to
 * expose the type `factory_type`
 */
#define MWHEEL_EXPOSE_INTERFACE_FACTORY(InterfaceType, TagType)                                    \
  using factory_type = mwheel::Singleton<mwheel::PrototypeFactory<InterfaceType, TagType>>

/**
 * @brief Must be used in the private section of a concrete product to
 * make it registrable
 */
#define MWHEEL_REGISTRABLE_PRODUCT static bool m_is_registered

/**
 * @brief Preamble to register a product into a factory
 */
#define MWHEEL_REGISTER_PRODUCT_START(ProductType)                                                 \
bool ProductType::m_is_registered(

/**
 * @brief Preamble to register a plugin product into a factory
 */
#define MWHEEL_REGISTER_PLUGIN_PRODUCT_START(ProductType)                                          \
  namespace {                                                                                      \
  mwheel::implementation::PluginUnloader unloader;                                                 \
  }                                                                                                \
  MWHEEL_REGISTER_PRODUCT_START(ProductType)

/**
 * @brief Epilogue of the registration for a product
 */
#define MWHEEL_REGISTER_PRODUCT_END() )

/**
 * @brief Epilogue of the registration for a plugin product
 */
#define MWHEEL_REGISTER_PLUGIN_PRODUCT_END() MWHEEL_REGISTER_PRODUCT_END()

/**
 * @brief Registers a tag/object pair into the factory
 */
#define MWHEEL_REGISTER_TAG_OBJECT_PAIR(tag_value, object)                                         \
  factory_type::get_instance().register_prototype(tag_value, object)

/**
 * @brief Registers a tag/plugin-object pair into the factory
 */
#define MWHEEL_REGISTER_TAG_PLUGIN_OBJECT_PAIR(tag_value, object)                                  \
  MWHEEL_REGISTER_TAG_OBJECT_PAIR(tag_value, object)                                               \
  &&unloader.on_unload([]() { factory_type::get_instance().unregister_prototype(tag_value); })

/**
 * @brief Must be used in the implementation file of a concrete product that
 * will be part of a plug-in library that will be loaded/unloaded at run-time
 */
#define MWHEEL_REGISTER_PLUGIN_PRODUCT(ProductType, tag_value)                                     \
  MWHEEL_REGISTER_PLUGIN_PRODUCT_START(ProductType)                                                \
  MWHEEL_REGISTER_TAG_PLUGIN_OBJECT_PAIR(tag_value, make_shared<ProductType>())                    \
  MWHEEL_REGISTER_PLUGIN_PRODUCT_END()

/**
 * @brief Must be used in the implementation file of a concrete product that
 * will be part of a plug-in library that will be linked at compile-time
 */
#define MWHEEL_REGISTER_PRODUCT(ProductType, tag_value)                                            \
  MWHEEL_REGISTER_PRODUCT_START(ProductType)                                                       \
  MWHEEL_REGISTER_TAG_OBJECT_PAIR(tag_value, make_shared<ProductType>())                           \
  MWHEEL_REGISTER_PRODUCT_END()

namespace mwheel {
/**
 * @brief Implementation details that should be hidden to end-users
 */
namespace implementation {

/**
 * @brief Triggers a custom action in its destructor
 */
class PluginUnloader {
public:
  /**
   * @brief Sets an action to be performed at destruction time
   *
   * @param[in] action custom action with signature compatible with `void (void)`
   *
   * @return always true
   */
  template <class T> bool on_unload(T action) {
    m_callback.push_back(action);
    return true;
  }

  /**
   * @brief The infamous destructor
   */
  ~PluginUnloader() {
    for (const auto &x : m_callback) {
      x();
    }
  }

private:
  /// Type of the custom callback
  using callback_type = std::function<void(void)>;
  /// Custom callback
  std::vector<callback_type> m_callback;
};
}
}

#endif /* PLUGIN_H_20150322 */
