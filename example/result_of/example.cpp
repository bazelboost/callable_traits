/*!
Copyright (c) 2016 Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <callable_traits/callable_traits.hpp>

namespace ct = callable_traits;

using expect = int;

struct foo;

template<typename T>
void test() {
    using result = ct::result_of<T>;
    static_assert(std::is_same<expect, result>{}, "");
}

int main() {

    test<int()>();
    test<int(*)()>();
    test<int(&)()>();
    test<int() const>();
    test<int(foo::*)() const>();

    auto x = []() -> int { return 0; };

    test<decltype(x)>();
}

