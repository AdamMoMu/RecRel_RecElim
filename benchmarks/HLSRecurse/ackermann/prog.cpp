#include "header.hpp"
#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

int ackerman(int m, int n)
{
    return run_function_old<int>(
        Sequence(
            IfElse([&](){ return m==0; },
                Return([&](){ return n+1; }),
            IfElse([&](){ return m>0 && n==0; },
                RecurseAndReturn([&](){ return make_hls_state_tuple(m-1,1u); }),
            Sequence(
                    RecurseWithResult(n, [&](){ return make_hls_state_tuple(m,n-1); }),
                    RecurseAndReturn([&](){ return make_hls_state_tuple(m-1,n); })
            )
            )
            )
        ),
		m, n
    );
}


int process_top(int m , int n)
{
  return ackerman(m,n);
}