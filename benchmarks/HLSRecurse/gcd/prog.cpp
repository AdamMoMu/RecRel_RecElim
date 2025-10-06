#include "header.hpp"

#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

int gcd(int a, int b)
{
  return run_function_old<int>(
      IfElse([&](){ return b == 0; },
          Return([&](){ return a; }),
          RecurseAndReturn([&](){ return make_hls_state_tuple(b, a % b); })
      ),
      a, b
  );
}


int process_top(int a , int b)
{
  return gcd(a, b);
}