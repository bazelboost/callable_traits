/*

@copyright Barrett Adair 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef CALLABLE_TRAITS_ADD_MEMBER_RVALUE_REFERENCE_HPP
#define CALLABLE_TRAITS_ADD_MEMBER_RVALUE_REFERENCE_HPP

#include <callable_traits/detail/core.hpp>

//[add_member_rvalue_reference_hpp
//`[section:ref_add_member_rvalue_reference add_member_rvalue_reference]

namespace callable_traits {

    template<typename T>
    struct add_member_rvalue_reference {

        using type = //implementation-defined
        //<-
            detail::fail_if_invalid<
                typename detail::traits<T>::add_member_rvalue_reference,
                member_qualifiers_are_illegal_for_this_type>;
        //->
    };

    template<typename T>
    using add_member_rvalue_reference_t =
        typename add_member_rvalue_reference<T>::type;
}

/*`
[heading Constraints]
* `T` must be a function type or a member function pointer

[heading Behavior]
* Adds a member rvalue reference qualifier (`&&`) to `T`, if not already present
* If an lvalue reference qualifier is present, the lvalue reference qualifier remains (in accordance with reference collapsing rules)

[heading Input/Output Examples]
[table
    [[`T`]                              [`add_member_rvalue_reference_t<T>`]]
    [[`int()`]                          [`int() &&`]]
    [[`int(foo::*)()`]                  [`int(foo::*)() &&`]]
    [[`int(foo::*)() &`]                [`int(foo::*)() &`]]
    [[`int(foo::*)() &&`]               [`int(foo::*)() &&`]]
    [[`int(foo::*)() const`]            [`int(foo::*)() const &&`]]
    [[`int(foo::*)() transaction_safe`] [`int(foo::*)() && transaction_safe`]]
    [[`int`]                            [(substitution failure)]]
    [[`int (&)()`]                      [(substitution failure)]]
    [[`int (*)()`]                      [(substitution failure)]]
    [[`int foo::*`]                     [(substitution failure)]]
    [[`int (foo::* const)()`]           [(substitution failure)]]
]

[heading Example Program]
[import ../example/add_member_rvalue_reference.cpp]
[add_member_rvalue_reference]
[endsect][/section:ref_add_member_rvalue_reference]
*/
//]

#endif
