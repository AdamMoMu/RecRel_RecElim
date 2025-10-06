#include "header.hpp"

#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;
int fib(int n) 
{
    int result1, result2;
    return run_function_old<int>(
        IfElse([&](){ return n <= 1; },
            Return([&](){ return 1; }),
            Sequence(
                RecurseWithResult(result1, [&](){ return make_hls_state_tuple(n - 1); }),
                RecurseWithResult(result2, [&](){ return make_hls_state_tuple(n - 2); }),
                Return([&](){ return result1 + result2; })
            )
        ),
        n, result1, result2
    );
}


int process_top(int n) {
    return fib(n);
}
