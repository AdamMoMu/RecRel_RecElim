#include "header.hpp"

#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;


int binarySearch(int arr[16], int low, int high, int x)
{
    int mid;

    return run_function_old<int>(
        IfElse([&](){ return high >= low; },
            Sequence(
                [&](){ mid = low + (high - low) / 2; },
                IfElse([&](){ return arr[mid] == x; },
                    Return([&](){ return mid; }),
                    IfElse([&](){ return arr[mid] > x; },
                        RecurseAndReturn([&](){ return make_hls_state_tuple(arr, low, mid - 1, x); }),
                        RecurseAndReturn([&](){ return make_hls_state_tuple(arr, mid + 1, high, x); })
                    )
                )
            ),
            Return([&](){ return -1; })
        ),
        arr, low, high, x
    );
}

int process_top(int n, int x, int arr[1024])
{
#pragma HLS INTERFACE port=arr ap_memory depth = 1024
  int result = binarySearch(arr,0,n - 1,x);
  return result;
}