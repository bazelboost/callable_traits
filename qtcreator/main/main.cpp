/*<-
Copyright Barrett Adair 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http ://boost.org/LICENSE_1_0.txt)
->*/

//[ intro
//` This short program showcases some, but not all, of the features available in [libname].

#include <type_traits>
#include <functional>
#include <tuple>
#include <callable_traits/callable_traits.hpp>

namespace ct = callable_traits;

// foo is an example of a function object
struct foo {
    void operator()(int, int&&, const int&, void* = nullptr) const {}
};

int main() {

    // indexed argument types
    using second_arg = ct::arg_at<1, foo>;
    static_assert(std::is_same<second_arg, int&&>::value, "");

    // arg types are packaged into std::tuple, which serves as the default
    // type list in ``[libname]`` (runtime capabilities are not used).
    using args = ct::args<foo>;
    using expected_args = std::tuple<int, int&&, const int&, void*>;
    static_assert(std::is_same<args, expected_args>::value, "");

    // ``[libns]``function_type decays a callable type to
    // a plain function type, which is structured in terms of INVOKE
    using function_type = ct::function_type<foo>;
    using expected_function_type = void(int, int&&, const int&, void*);
    static_assert(std::is_same<function_type, expected_function_type>::value, "");

    // By design, the ``libname`` interface uses constexpr
    // std::integral_constant functions (whenever sensible).
    // By also defining the appropriate overloads, this gives
    // programmers the option of using either type arguments
    // or a value arguments:
    static_assert(ct::arity<foo>() == 4, "");
    static_assert(ct::arity(foo{}) == 4, "");

    // Attentive readers might notice that the type of the
    // foo{} expression above is foo&&, rather than foo. Indeed,
    // ``libname`` is designed to also allow both ref-qualified
    // and cv-qualified arguments across the board:

    static_assert(ct::arity<foo&&>() == 4, "");

    // Now, if foo had an operator() overload with a && qualifier,
    // taking a different number of arguments, the above static assert
    // would fail.

    // For consistency, we'll avoid the value-style overloads for
    // the remainder of this example whenever possible.

    static_assert(ct::max_arity<foo>() == 4, "");
    static_assert(ct::min_arity<foo>() == 3, "");

    // a quick way to check for a void return type
    static_assert(ct::has_void_return<foo>(), "");

    // C-style variadics detection (e.g. an ellipses in a signature)
    static_assert(!ct::has_varargs<foo>(), "");

    int i = 0;

    // ``[libns]``can_invoke allows us to preview whether std::invoke
    // would compile with the given arguments.
    static_assert(ct::can_invoke(foo{}, 0, 0, i), "");
    // no error:     std::invoke(foo{}, 0, 0, i);

    static_assert(!ct::can_invoke(foo{}, nullptr), "");
    // error:         std::invoke(foo{}, nullptr);

    // For function objects, the following checks are determined by the
    // value. qualifiers on operator(), rather than the qualifiers on
    // of the type passed. This is done for consistency with member
    // function pointers, where the checks below would look at the
    // function qualifiers (rather than qualifiers on the pointer itself)
    static_assert(ct::is_const_qualified<foo>(), "");
    static_assert(!ct::is_volatile_qualified<foo>(), "");
    static_assert(!ct::is_reference_qualified<foo>(), "");
    static_assert(!ct::is_lvalue_qualified<foo>(), "");
    static_assert(!ct::is_rvalue_qualified<foo>(), "");

    // is_constexpr would return std::true_type if foo's operator() were constexpr.
    static_assert(!ct::is_constexpr<foo>(), "");

    // The same check can be performed using std::integral_constant
    // in conjunction with function addresses:
    using pmf = decltype(&foo::operator());
    using pmf_constant = std::integral_constant<pmf, &foo::operator()>;
    static_assert(!ct::is_constexpr<pmf_constant>(), "");

    // So that you don't have to scroll to the top to check,
    // here's the type of pmf for reference.
    using with_const = void (foo::*)(int, int&&, const int&, void*) const;
    static_assert(std::is_same<pmf, with_const>::value, "");

    // If you find yourself in the unfortunate-and-probably-avoidable
    // situation of needing to transform member function pointer
    // types, ``[libname]`` has all the tools you need to prolong
    // your sanity.

    // ``[libname]`` lets you manipulate qualifiers on PMF types.
    // To remove const:
    using mutable_pmf = ct::remove_function_const<pmf>;
    using without_const = void (foo::*)(int, int&&, const int&, void*) /*no const!*/;
    static_assert(std::is_same<mutable_pmf, without_const>::value, "");

    // To add an rvalue qualifier:
    using rvalue_pmf = ct::add_function_rvalue<pmf>;
    using with_rvalue = void (foo::*)(int, int&&, const int&, void*) const &&;
    static_assert(std::is_same<rvalue_pmf, with_rvalue>::value, "");

    // Just like std::add_rvalue_reference, ``[libns]``add_function_rvalue
    // follows C++11 reference collapsing rules. While remove_function_const
    // and add_function_rvalue are somewhat clumsy names, they are the best
    // the best the author could provide while still allowing both terseness
    // and grep-ability against std::remove_const, etc. in <type_traits>.
    // Naturally, ``[libname]`` provides similar tools for the other C++
    // function qualifiers. Head to the reference section of this documentation
    // for more examples.

    // To remove a member pointer:
    using fn = ct::remove_member_pointer<pmf>;
    using expected_fn = void (int, int&&, const int&, void*) const;
    static_assert(std::is_same<fn, expected_fn>::value, "");

    // We just created an abominable function type - notice the const
    // qualifier! ``libns``remove_function_const accepts abominable
    // types too:

    using not_abominable = ct::remove_function_const<fn>;
    using expected_fn2 = void (int, int&&, const int&, void*);
    static_assert(std::is_same<not_abominable, expected_fn2>::value, "");
}
//]
