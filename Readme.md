# RPN Calc in C++ Template Metaprogramming

Tiny RPN calculator implemented entirely in template metaprogramming. No runtime logic, no `std::stack`, just variadic template recursion and partial specialization.

## How it works (briefly)

- The stack is a variadic non-type pack stored in a `Stack` struct.
- The engine pattern-matches on tokens via template specialization, pushing values and collapsing the stack when it sees an operator.
- The result is computed at compile time and checked with a `static_assert`.
- A small compile-time "coroutine-like" generator uses a state machine (`Yield`/`Done`) to emit a sequence at compile time.

## Supported operators

`+` (`Op::Add`), `-` (`Op::Sub`), `*` (`Op::Mul`), `/` (`Op::Div`), `%` (`Op::Mod`)

Note: operator enum values are offset from small integers so numeric tokens (like `4`) are never mistaken for an `Op`.

## Compile-time coroutine-like generator

This is a tiny "yield" state machine that runs at compile time:

- `Yield<V, Next>` emits a value and transitions to the next state type.
- `Done<V>` terminates and stores the final value.
- `Run<Step, State>` repeatedly applies `Step<State>` to produce a `RunResult` with `yields` (a `Stack<...>`) and `final`.

It is not a real C++20 coroutine, but it mimics the idea of resuming with state and collecting yielded values entirely at compile time.

## Examples

In `main.cpp`:

```cpp
// Compile-time coroutine-like generator
using Gen = Run<GenStep, GenState<0>>::type;
static_assert(std::is_same_v<typename Gen::yields, Stack<1, 2, 3>>);
static_assert(Gen::final == 3);

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
./b
./rpn
```
