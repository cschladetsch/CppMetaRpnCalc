#include <iostream>
#include <type_traits>

enum class Op : int { Add = 1000, Sub = 1001, Mul = 1002, Div = 1003, Mod = 1004 };

// The Stack
template <auto... Vs> struct Stack {};

// The Engine
template <typename CurrentStack, auto... Tokens>
struct RPN;

// --- 1. Base Case: No more tokens ---
template <auto Result, auto... Rest>
struct RPN<Stack<Result, Rest...>> {
    static constexpr auto value = Result;
};

// --- 2. Push Value: Head of tokens is not an Op ---
template <auto... Vs, auto Head, auto... Tail>
requires (!std::is_same_v<decltype(Head), Op>)
struct RPN<Stack<Vs...>, Head, Tail...>
    : RPN<Stack<Head, Vs...>, Tail...> {};

// --- 3. Operator Specializations ---
// These match when the *next* token is specifically an Op

// Addition
template <auto B, auto A, auto... Vs, auto... Tail>
struct RPN<Stack<B, A, Vs...>, Op::Add, Tail...> 
    : RPN<Stack<A + B, Vs...>, Tail...> {};

// Subtraction
template <auto B, auto A, auto... Vs, auto... Tail>
struct RPN<Stack<B, A, Vs...>, Op::Sub, Tail...> 
    : RPN<Stack<A - B, Vs...>, Tail...> {};

// Multiplication
template <auto B, auto A, auto... Vs, auto... Tail>
struct RPN<Stack<B, A, Vs...>, Op::Mul, Tail...> 
    : RPN<Stack<A * B, Vs...>, Tail...> {};

// Division
template <auto B, auto A, auto... Vs, auto... Tail>
struct RPN<Stack<B, A, Vs...>, Op::Div, Tail...> 
    : RPN<Stack<A / B, Vs...>, Tail...> {};

// Modulo
template <auto B, auto A, auto... Vs, auto... Tail>
struct RPN<Stack<B, A, Vs...>, Op::Mod, Tail...> 
    : RPN<Stack<A % B, Vs...>, Tail...> {};

int main() {
    // (5 + 10) * 3
    using Calc = RPN<Stack<>, 5, 10, Op::Add, 3, Op::Mul>;
    static_assert(Calc::value == 45);

    // (20 / 4) + (7 % 4)
    using Calc2 = RPN<Stack<>, 20, 4, Op::Div, 7, 4, Op::Mod, Op::Add>;
    static_assert(Calc2::value == 8);
    
    std::cout << Calc::value << std::endl;
    std::cout << Calc2::value << std::endl;
    return 0;
}
