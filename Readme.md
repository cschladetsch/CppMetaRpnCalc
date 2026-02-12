# RPN Calc in C++ Template Metaprogramming

Tiny RPN calculator implemented entirely in template metaprogramming. No runtime logic, no `std::stack`, just variadic template recursion and partial specialization.

## How it works (briefly)

- The stack is a variadic non-type pack stored in a `Stack` struct.
- The engine pattern-matches on tokens via template specialization, pushing values and collapsing the stack when it sees an operator.
- The result is computed at compile time and checked with a `static_assert`.

## Supported operators

`+` (`Op::Add`), `-` (`Op::Sub`), `*` (`Op::Mul`), `/` (`Op::Div`), `%` (`Op::Mod`)

## Examples

In `main.cpp`:

```cpp
// (5 + 10) * 3
using Calc = RPN<Stack<>, 5, 10, Op::Add, 3, Op::Mul>;
static_assert(Calc::value == 45);
```

With division and modulo:

```cpp
// (20 / 4) + (7 % 4)
using Calc2 = RPN<Stack<>, 20, 4, Op::Div, 7, 4, Op::Mod, Op::Add>;
static_assert(Calc2::value == 8);
```

## Build and run

```sh
g++ -std=c++20 -O2 -Wall -Wextra -pedantic main.cpp -o rpn
./rpn
```
