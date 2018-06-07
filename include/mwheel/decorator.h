// Modern Wheel : all the things that shouldn't be reinvented from one project to the other
//
// The MIT License (MIT)
//
// Copyright (C) 2018  Federico Ficarelli (nazavode.github.io)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


/// @file decorator.h
/// @brief A tentative, strict C++11 callable decorator.
///  
/// Disclaimer: with C++14 generic lambdas at hand, stuff like this becomes
/// intolerably bloated.
/// 
/// This code has been imported from somewhere else, so if you're fond
/// of git history please refer to its original location:
/// https://gist.github.com/nazavode/dd5055fb84b46ee83f2c35e1815a6c21
/// 
/// @author Federico Ficarelli


#ifndef DECORATOR_H_20180607
#define DECORATOR_H_20180607

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mwheel {

/// @brief Signature type for a callable to be allowed as decorator callback
/// @todo Just keep it simple, at least for now. A natural improvement would
/// be to have an "after" callback that is provided the target's return value
/// for filtering/tracing purposes.
using callback_type = void();

namespace detail {

template <typename>
struct decorator;

template <typename ReturnType, typename... ArgTypes>
struct decorator<ReturnType(ArgTypes...)> {
    using target_ptr_type = std::function<ReturnType(ArgTypes...)>;
    using callback_ptr_type = std::function<callback_type>;

    // clang-format off

    /// @brief Constructs and wraps a target callable
    /// @param[in] callable the target callable to be wrapped
    /// @param[in] before the callable to be invoked prior to target invocation
    /// @param[in] after the callable to be invoked after target invocation
    decorator(target_ptr_type callable,
              callback_ptr_type before = nullptr,
              callback_ptr_type after = nullptr)
        noexcept(std::is_nothrow_move_constructible<target_ptr_type>::value &&
                 std::is_nothrow_move_constructible<callback_ptr_type>::value)
        : m_callable(std::move(callable)),
          m_before(std::move(before)),
          m_after(std::move(after)){};

    

    /// @brief Calls the wrapped callable forwarding all parameters
    /// This operator has the same signature as wrapped's,
    /// instantiation on wrong calls fails at the higher level so
    /// compilation errors should be much more intuitive.
    /// @todo add a static assertion when std::is_invokable is available
    template <typename R = ReturnType>
    typename std::enable_if<!std::is_same<R, void>::value, R>::type
    operator()(ArgTypes&&... args) const {
        if (m_before) m_before();
        auto result = m_callable(std::forward<ArgTypes>(args)...);
        if (m_after) m_after();
        return result;
    }

    template <typename R = ReturnType>
    typename std::enable_if<std::is_same<R, void>::value, R>::type
    operator()(ArgTypes&&... args) const {
        if (m_before) m_before();
        m_callable(std::forward<ArgTypes>(args)...);
        if (m_after) m_after();
    }

    // clang-format on

   private:
    target_ptr_type m_callable;
    callback_ptr_type m_before;
    callback_ptr_type m_after;
};

template <typename>
struct callable_traits;

// callable_traits<functors (including lambdas)>
template <typename T>
struct callable_traits : public callable_traits<decltype(&T::operator())> {};

// callable_traits<references>
template <typename T>
struct callable_traits<T&>
    : public callable_traits<typename std::remove_reference<T>::type> {};

// callable_traits<pointers>
template <typename T>
struct callable_traits<T*>
    : public callable_traits<typename std::remove_pointer<T>::type> {};

// decorators (decorator chaining)
template <typename T>
struct callable_traits<decorator<T>> : public callable_traits<T> {};

// callable_traits<member functions>
template <typename ReturnType, typename ObjectType, typename... ArgTypes>
struct callable_traits<ReturnType (ObjectType::*)(ArgTypes...) const>
    : public callable_traits<ReturnType(ArgTypes...)> {};

// callable_traits<abstract callable signature>
// (e.g.: callable_traits<F>::type is meant to be used as T in
// std::function<T>)
template <typename ReturnType, typename... ArgTypes>
struct callable_traits<ReturnType(ArgTypes...)> {
    static constexpr auto arity = sizeof...(ArgTypes);

    using type = ReturnType(ArgTypes...);

    template <std::size_t i>
    struct arg {
        static_assert(i < arity, "argument index out of arity bounds");
        using type = typename std::tuple_element<i, std::tuple<ArgTypes...>>::type;
    };
};

}  // namespace detail

/// @brief
template <typename F>
using callable_t = typename detail::callable_traits<F>::type;

/// @brief
template <typename F, std::size_t narg>
using argument_t = typename detail::callable_traits<F>::template arg<narg>::type;

/// @brief
template <typename F>
using decorator_t = detail::decorator<callable_t<F>>;

// clang-format off

/// @brief Wraps a callable instance in a decorator
/// @param[in] callable the target callable to be wrapped
/// @param[in] callbacks callables to be used as `before` and `after`
///            fences for the returned `decorator`
template <typename F, typename... ArgTypes>
auto make_decorator(F&& callable, ArgTypes&&... callbacks)
    noexcept(std::is_nothrow_constructible<decorator_t<F>>::value)
    -> decorator_t<F>
{
    return {std::forward<F>(callable), std::forward<ArgTypes>(callbacks)...};
}
// clang-format on

}  // namespace mwheel

#endif
