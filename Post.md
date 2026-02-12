RPN Calc in C++ Template Metaprogramming

If you aren't using your compiler as a functional programming engine, are you even using C++?

I spent some time implementing an RPN calculator entirely in Template Metaprogramming. No runtime logic, no `std::stack`, just pure variadic template recursion and partial specialization.

The implementation (briefly):

- The stack is a variadic non-type pack stored in a `Stack` struct.
- The engine pattern-matches on tokens via template specialization, pushing values and collapsing the stack when it sees an operator.
- The result is computed at compile time and checked with a `static_assert`.

Why do this? Beyond the "because I can" factor, it is about understanding the limits of computation inside teh compiler. Moving logic to compile time is not just a performance trick; it can make invalid states unrepresentable. If the math is wrong, the build fails.

Question for the TMP fans: at what point does the compile-time vs. runtime trade-off break for you? Is it when build times hit five minutes, or when you have to explain the error messages to your junior devs?

Anyway, fun little rabbit hole. I'm sure there are 20 ways to make the error spew cleaner, but I kinda liked how gnarly it got.

#cpp #metaprogramming #templates
