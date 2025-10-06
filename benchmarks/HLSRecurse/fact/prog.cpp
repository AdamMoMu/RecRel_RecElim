#include "header.hpp"
#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

int factorial(int n)
{
    int temp;
    return run_function_old<int>(
        IfElse([&](){ return n == 0 || n - 1 == 0; },
            Return([&](){ return 1; }),
            Sequence(
                RecurseWithResult(temp, [&](){ return make_hls_state_tuple(n - 1); }),
                Return([&](){ return n * temp; })
            )
        ),
        n
    );
}



int process_top(int n) {
    return factorial(n);
}