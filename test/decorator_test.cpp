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

#include <mwheel/decorator.h>

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <functional>
#include <type_traits>

struct Counter {
    int call_count = 0;

    void poke(int) {
        ++call_count;
    }

    void poke() {}
};

template <typename T>
auto free_function_template(T value) -> decltype(value * 2) {
    return {value * 2};
}

template <typename T>
auto inc_free_function_template(T value) -> decltype(value + 1) {
    return {value + 1};
}

void free_function_nop() {}

void free_function_overloaded(int i) {}

void free_function_overloaded(float i) {}

BOOST_AUTO_TEST_SUITE(DecoratorTest)

BOOST_AUTO_TEST_CASE(TraitsFreeFunctionTemplate) {
    using func_int_t = decltype(free_function_template<int>);
    using func_const_int_t = decltype(free_function_template<const int>);
    using func_const_int_ref_t = decltype(free_function_template<const int&>);
    using func_volatile_const_int_ref_t = decltype(free_function_template<volatile const int&>);
    // callable_t
    constexpr auto type_is_same_int = std::is_same<
        mwheel::detail::callable_traits<func_int_t>::type,
        mwheel::callable_t<func_int_t>>::value;
    BOOST_CHECK_EQUAL(type_is_same_int, true);
    constexpr auto type_is_same_const_int = std::is_same<
        mwheel::detail::callable_traits<func_const_int_t>::type,
        mwheel::callable_t<func_const_int_t>>::value;
    BOOST_CHECK_EQUAL(type_is_same_const_int, true);
    constexpr auto type_is_same_const_int_ref = std::is_same<
        mwheel::detail::callable_traits<func_const_int_ref_t>::type,
        mwheel::callable_t<func_const_int_ref_t>>::value;
    BOOST_CHECK_EQUAL(type_is_same_const_int_ref, true);
    constexpr auto type_is_same_volatile_const_int_ref = std::is_same<
        mwheel::detail::callable_traits<func_volatile_const_int_ref_t>::type,
        mwheel::callable_t<func_volatile_const_int_ref_t>>::value;
    BOOST_CHECK_EQUAL(type_is_same_volatile_const_int_ref, true);
    // arity
    constexpr auto arity_int = mwheel::detail::callable_traits<func_int_t>::arity;
    BOOST_CHECK_EQUAL(arity_int, 1);
    constexpr auto arity_const_int = mwheel::detail::callable_traits<func_const_int_t>::arity;
    BOOST_CHECK_EQUAL(arity_const_int, 1);
    constexpr auto arity_const_int_ref = mwheel::detail::callable_traits<func_const_int_ref_t>::arity;
    BOOST_CHECK_EQUAL(arity_const_int_ref, 1);
    constexpr auto arity_volatile_const_int_ref = mwheel::detail::callable_traits<func_volatile_const_int_ref_t>::arity;
    BOOST_CHECK_EQUAL(arity_volatile_const_int_ref, 1);
    // argument_t
    constexpr auto arg_0_is_int = std::is_same<mwheel::argument_t<func_int_t, 0>, int>::value;
    BOOST_CHECK_EQUAL(arg_0_is_int, true);
}

BOOST_AUTO_TEST_CASE(TraitsFunctor) {
    auto l_int = [](int){};
    auto l_const_int = [](const int){};
    auto l_const_int_ref = [](const int&){};
    auto l_volatile_const_int_ref = [](volatile const int&){};
    using l_int_t = decltype(l_int);
    using l_const_int_t = decltype(l_const_int);
    using l_const_int_ref_t = decltype(l_const_int_ref);
    using l_volatile_const_int_ref_t = decltype(l_volatile_const_int_ref);
    // callable_t
    constexpr auto type_is_same_int = std::is_same<
        mwheel::detail::callable_traits<l_int_t>::type,
        mwheel::callable_t<l_int_t>>::value;
    BOOST_CHECK_EQUAL(type_is_same_int, true);
    constexpr auto type_is_same_const_int = std::is_same<
        mwheel::detail::callable_traits<l_const_int_t>::type,
        mwheel::callable_t<l_const_int_t>>::value;
    BOOST_CHECK_EQUAL(type_is_same_const_int, true);
    constexpr auto type_is_same_const_int_ref = std::is_same<
        mwheel::detail::callable_traits<l_const_int_ref_t>::type,
        mwheel::callable_t<l_const_int_ref_t>>::value;
    BOOST_CHECK_EQUAL(type_is_same_const_int_ref, true);
    constexpr auto type_is_same_volatile_const_int_ref = std::is_same<
        mwheel::detail::callable_traits<l_volatile_const_int_ref_t>::type,
        mwheel::callable_t<l_volatile_const_int_ref_t>>::value;
    BOOST_CHECK_EQUAL(type_is_same_volatile_const_int_ref, true);
    // arity
    constexpr auto arity_int = mwheel::detail::callable_traits<l_int_t>::arity;
    BOOST_CHECK_EQUAL(arity_int, 1);
    constexpr auto arity_const_int = mwheel::detail::callable_traits<l_const_int_t>::arity;
    BOOST_CHECK_EQUAL(arity_const_int, 1);
    constexpr auto arity_const_int_ref = mwheel::detail::callable_traits<l_const_int_ref_t>::arity;
    BOOST_CHECK_EQUAL(arity_const_int_ref, 1);
    constexpr auto arity_volatile_const_int_ref = mwheel::detail::callable_traits<l_volatile_const_int_ref_t>::arity;
    BOOST_CHECK_EQUAL(arity_volatile_const_int_ref, 1);
    // argument_t
    constexpr auto arg_0_is_int = std::is_same<mwheel::argument_t<l_int_t, 0>, int>::value;
    BOOST_CHECK_EQUAL(arg_0_is_int, true);
}

BOOST_AUTO_TEST_CASE(TraitsArityFunctor) {
    auto f0 = []{};
    auto f1 = [](int){};
    auto f2 = [](int, int){};
    auto f3 = [](int, int, int){};
    auto f4 = [](int, int, int, int){};
    auto f5 = [](int, int, int, int, int){};
    using f0_t = decltype(f0);
    using f1_t = decltype(f1);
    using f2_t = decltype(f2);
    using f3_t = decltype(f3);
    using f4_t = decltype(f4);
    using f5_t = decltype(f5);
    constexpr auto arity_f0 = mwheel::detail::callable_traits<f0_t>::arity;
    constexpr auto arity_f1 = mwheel::detail::callable_traits<f1_t>::arity;
    constexpr auto arity_f2 = mwheel::detail::callable_traits<f2_t>::arity;
    constexpr auto arity_f3 = mwheel::detail::callable_traits<f3_t>::arity;
    constexpr auto arity_f4 = mwheel::detail::callable_traits<f4_t>::arity;
    constexpr auto arity_f5 = mwheel::detail::callable_traits<f5_t>::arity;
    BOOST_CHECK_EQUAL(arity_f0, 0);
    BOOST_CHECK_EQUAL(arity_f1, 1);
    BOOST_CHECK_EQUAL(arity_f2, 2);
    BOOST_CHECK_EQUAL(arity_f3, 3);
    BOOST_CHECK_EQUAL(arity_f4, 4);
    BOOST_CHECK_EQUAL(arity_f5, 5);
}

BOOST_AUTO_TEST_CASE(DecoratorFreeFunctionTemplate) {
    auto f_before_call_count = 0;
    auto f_after_call_count = 0;
    auto f_before = [&f_before_call_count]{++f_before_call_count;};
    auto f_after = [&f_after_call_count]{++f_after_call_count;};

    auto f_wrapped = mwheel::make_decorator(inc_free_function_template<int>, f_before, f_after);

    auto r0 = f_wrapped(0);
    BOOST_CHECK_EQUAL(r0, 1);
    BOOST_CHECK_EQUAL(f_before_call_count, 1);
    BOOST_CHECK_EQUAL(f_after_call_count, 1);
    auto r1 = f_wrapped(1);
    BOOST_CHECK_EQUAL(r1, 2);
    BOOST_CHECK_EQUAL(f_before_call_count, 2);
    BOOST_CHECK_EQUAL(f_after_call_count, 2);
    auto r2 = f_wrapped(2);
    BOOST_CHECK_EQUAL(r2, 3);
    BOOST_CHECK_EQUAL(f_before_call_count, 3);
    BOOST_CHECK_EQUAL(f_after_call_count, 3);

    auto ff_wrapped = mwheel::make_decorator(f_wrapped);

    auto rr1 = ff_wrapped(0);
    BOOST_CHECK_EQUAL(rr1, 1);
    BOOST_CHECK_EQUAL(f_before_call_count, 4);
    BOOST_CHECK_EQUAL(f_after_call_count, 4);
    auto rr2 = ff_wrapped(1);
    BOOST_CHECK_EQUAL(rr2, 2);
    BOOST_CHECK_EQUAL(f_before_call_count, 5);
    BOOST_CHECK_EQUAL(f_after_call_count, 5);
    auto rr3 = ff_wrapped(2);
    BOOST_CHECK_EQUAL(rr3, 3);
    BOOST_CHECK_EQUAL(f_before_call_count, 6);
    BOOST_CHECK_EQUAL(f_after_call_count, 6);

    auto fff_wrapped = mwheel::make_decorator(ff_wrapped, f_before, f_after);

    auto rrr1 = fff_wrapped(0);
    BOOST_CHECK_EQUAL(rrr1, 1);
    BOOST_CHECK_EQUAL(f_before_call_count, 8);
    BOOST_CHECK_EQUAL(f_after_call_count, 8);
    auto rrr2 = fff_wrapped(1);
    BOOST_CHECK_EQUAL(rrr2, 2);
    BOOST_CHECK_EQUAL(f_before_call_count, 10);
    BOOST_CHECK_EQUAL(f_after_call_count, 10);
    auto rrr3 = fff_wrapped(2);
    BOOST_CHECK_EQUAL(rrr3, 3);
    BOOST_CHECK_EQUAL(f_before_call_count, 12);
    BOOST_CHECK_EQUAL(f_after_call_count, 12);
}

BOOST_AUTO_TEST_CASE(DecoratorFunctor) {
    auto f_call_count = 0;
    auto f_before_call_count = 0;
    auto f_after_call_count = 0;
    auto f = [&f_call_count](int){++f_call_count;};
    auto f_before = [&f_before_call_count]{++f_before_call_count;};
    auto f_after = [&f_after_call_count]{++f_after_call_count;};

    auto f_wrapped = mwheel::make_decorator(f, f_before, f_after);

    f_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 1);
    BOOST_CHECK_EQUAL(f_before_call_count, 1);
    BOOST_CHECK_EQUAL(f_after_call_count, 1);
    f_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 2);
    BOOST_CHECK_EQUAL(f_before_call_count, 2);
    BOOST_CHECK_EQUAL(f_after_call_count, 2);
    f_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 3);
    BOOST_CHECK_EQUAL(f_before_call_count, 3);
    BOOST_CHECK_EQUAL(f_after_call_count, 3);

    auto ff_wrapped = mwheel::make_decorator(f_wrapped);

    ff_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 4);
    BOOST_CHECK_EQUAL(f_before_call_count, 4);
    BOOST_CHECK_EQUAL(f_after_call_count, 4);
    ff_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 5);
    BOOST_CHECK_EQUAL(f_before_call_count, 5);
    BOOST_CHECK_EQUAL(f_after_call_count, 5);
    ff_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 6);
    BOOST_CHECK_EQUAL(f_before_call_count, 6);
    BOOST_CHECK_EQUAL(f_after_call_count, 6);

    auto fff_wrapped = mwheel::make_decorator(ff_wrapped, f_before, f_after);

    fff_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 7);
    BOOST_CHECK_EQUAL(f_before_call_count, 8);
    BOOST_CHECK_EQUAL(f_after_call_count, 8);
    fff_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 8);
    BOOST_CHECK_EQUAL(f_before_call_count, 10);
    BOOST_CHECK_EQUAL(f_after_call_count, 10);
    fff_wrapped(0);
    BOOST_CHECK_EQUAL(f_call_count, 9);
    BOOST_CHECK_EQUAL(f_before_call_count, 12);
    BOOST_CHECK_EQUAL(f_after_call_count, 12);
}

BOOST_AUTO_TEST_CASE(MakeDecoratorFreeFunctionOverload) {
    void (&selected_overload)(float)  = free_function_overloaded;
    auto f_wrapped = mwheel::make_decorator(selected_overload);
    f_wrapped(0);
}

BOOST_AUTO_TEST_CASE(DecoratorMemberFunctionOverload) {
    Counter c{};
    // manually resolve overload
    auto poke_ref = [] (Counter& x) {return x.poke(0);};
    auto f_wrapped = mwheel::make_decorator(poke_ref);
    f_wrapped(c);
    BOOST_CHECK_EQUAL(c.call_count, 1);
    f_wrapped(c);
    BOOST_CHECK_EQUAL(c.call_count, 2);
    f_wrapped(c);
    BOOST_CHECK_EQUAL(c.call_count, 3);
}

BOOST_AUTO_TEST_CASE(DecoratorBind) {
    auto nullary_ref = std::bind(free_function_template<int>, 100);
    // manually resolve overload of std::bind::operator() via explicit template parameter
    auto f_wrapped = mwheel::detail::decorator<int()>(nullary_ref);
    auto res = f_wrapped();
    BOOST_CHECK_EQUAL(res, 200);
}

BOOST_AUTO_TEST_CASE(DecoratorOptionalFences) {
    int sentry = 0;
    int before_sentry = 0;
    int after_sentry = 0;
    auto pass = [&sentry](int value){sentry += value;};
    auto pass_before = [&before_sentry]{before_sentry += 2;};
    auto pass_after = [&after_sentry]{after_sentry += 3;};
    auto pass_wrapped = mwheel::make_decorator(pass, pass_before, pass_after);
    pass_wrapped(1);
    BOOST_CHECK_EQUAL(sentry, 1);
    BOOST_CHECK_EQUAL(before_sentry, 2);
    BOOST_CHECK_EQUAL(after_sentry, 3);
    pass_wrapped(1);
    BOOST_CHECK_EQUAL(sentry, 2);
    BOOST_CHECK_EQUAL(before_sentry, 4);
    BOOST_CHECK_EQUAL(after_sentry, 6);
    pass_wrapped = mwheel::make_decorator(pass, nullptr, pass_after);
    pass_wrapped(1);
    BOOST_CHECK_EQUAL(sentry, 3);
    BOOST_CHECK_EQUAL(before_sentry, 4);
    BOOST_CHECK_EQUAL(after_sentry, 9);
    pass_wrapped = mwheel::make_decorator(pass, pass_before);
    pass_wrapped(1);
    BOOST_CHECK_EQUAL(sentry, 4);
    BOOST_CHECK_EQUAL(before_sentry, 6);
    BOOST_CHECK_EQUAL(after_sentry, 9);
    pass_wrapped = mwheel::make_decorator(pass);
    pass_wrapped(1);
    BOOST_CHECK_EQUAL(sentry, 5);
    BOOST_CHECK_EQUAL(before_sentry, 6);
    BOOST_CHECK_EQUAL(after_sentry, 9);
}

BOOST_AUTO_TEST_SUITE_END()
