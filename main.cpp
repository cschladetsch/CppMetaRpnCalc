#include <iostream>
#include <type_traits>

enum class Op : int { Add = 1000, Sub = 1001, Mul = 1002, Div = 1003, Mod = 1004 };

// The Stack
template <auto... Vs> struct Stack {};

// --- Compile-time coroutine-like generator ---
template <auto Value, typename NextState>
struct Yield {
    static constexpr auto value = Value;
    using next = NextState;
};

template <auto Value>
struct Done {
    static constexpr auto value = Value;
};

template <template <typename> typename Step, typename State, auto... Ys>
struct Run;

template <typename StepResult, template <typename> typename Step, auto... Ys>
struct RunNext;

template <auto Final, auto... Ys>
struct RunResult {
    using yields = Stack<Ys...>;
    static constexpr auto final = Final;
};

template <template <typename> typename Step, typename State, auto... Ys>
struct Run {
    using step = typename Step<State>::type;
    using type = typename RunNext<step, Step, Ys...>::type;
};

template <auto Y, typename Next, template <typename> typename Step, auto... Ys>
struct RunNext<Yield<Y, Next>, Step, Ys...> {
    using type = typename Run<Step, Next, Ys..., Y>::type;
};

template <auto Final, template <typename> typename Step, auto... Ys>
struct RunNext<Done<Final>, Step, Ys...> {
    using type = RunResult<Final, Ys...>;
};

template <int I>
struct GenState {};

template <typename State>
struct GenStep;

template <int I>
struct GenStep<GenState<I>> {
    using type = std::conditional_t<
        (I < 3),
        Yield<I + 1, GenState<I + 1>>,
        Done<I>
    >;
};

template <auto... Vs>
void print_stack(Stack<Vs...>) {
    ((std::cout << Vs << ' '), ...);
    std::cout << std::endl;
}

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
    using Gen = Run<GenStep, GenState<0>>::type;
    static_assert(std::is_same_v<typename Gen::yields, Stack<1, 2, 3>>);
    static_assert(Gen::final == 3);

    std::cout << "coro yields: ";
    print_stack(typename Gen::yields{});
    std::cout << "coro final: " << Gen::final << std::endl;

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
