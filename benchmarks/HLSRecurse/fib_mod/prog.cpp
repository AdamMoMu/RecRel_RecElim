#include "header.hpp"

#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

int fib(int n, int m) {
    int ret1, ret2;
    return run_function_old<int>(
        Sequence(
            If([&](){ return ((n <= 1) || (m <= 1)); },
                Return([&](){ return 1; })
            ),
            Sequence(
                RecurseWithResult(ret1, [&](){ return make_hls_state_tuple(n / 2, m - 1); }),
                RecurseWithResult(ret2, [&](){ return make_hls_state_tuple(n / 4, m - 2); }),
                Return([&](){ return ret1 + ret2; })
            )
        ),
    n, m, ret1, ret2
    );
}

int process_top(int n, int m) {
    return fib(n, m);
}