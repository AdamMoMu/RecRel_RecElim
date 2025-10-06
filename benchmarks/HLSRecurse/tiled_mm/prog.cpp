#include "header.hpp"
#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

int sub(int v,int h, int stride, int n) {
    return v*stride*n/2+h*n/2; 
}

void tiled_mmm(int n, int stride, float *dst, const float *a, const float *b)
{

    run_function_old<void>(
        IfElse([&](){ return n<=16; },
            [&](){
                for(int r=0; r<n; r++){
                    for(int c=0; c<n; c++){
                        float acc=dst[r*stride+c];
                        for(int i=0; i<n; i++){
                            acc += a[r*stride+i] * b[i*stride+c];
                        }
                        dst[r*stride+c]=acc;
                    }
                }
            },
            Sequence(
                Recurse([&](){ return make_hls_state_tuple(n/2, stride, dst+sub(0,0, stride, n), a+sub(0,0, stride, n), b+sub(0,0, stride, n)); }),
                Recurse([&](){ return make_hls_state_tuple(n/2, stride, dst+sub(0,1, stride, n), a+sub(0,0, stride, n), b+sub(0,1, stride, n)); }),
                Recurse([&](){ return make_hls_state_tuple(n/2, stride, dst+sub(1,0, stride, n), a+sub(1,0, stride, n), b+sub(0,0, stride, n)); }),
                Recurse([&](){ return make_hls_state_tuple(n/2, stride, dst+sub(1,1, stride, n), a+sub(1,0, stride, n), b+sub(0,1, stride, n)); }),

                Recurse([&](){ return make_hls_state_tuple(n/2, stride, dst+sub(0,0, stride, n), a+sub(0,1, stride, n), b+sub(1,0, stride, n)); }),
                Recurse([&](){ return make_hls_state_tuple(n/2, stride, dst+sub(0,1, stride, n), a+sub(0,1, stride, n), b+sub(1,1, stride, n)); }),
                Recurse([&](){ return make_hls_state_tuple(n/2, stride, dst+sub(1,0, stride, n), a+sub(1,1, stride, n), b+sub(1,0, stride, n)); }),
                Recurse([&](){ return make_hls_state_tuple(n/2, stride, dst+sub(1,1, stride, n), a+sub(1,1, stride, n), b+sub(1,1, stride, n)); })
            )
        ),
        n, stride, dst, a, b
    );
}

int process_top(int n, int stride, float *dst, float *a, float *b) {
    #pragma HLS INTERFACE ap_memory port = dst depth=16
    #pragma HLS INTERFACE ap_memory port = a depth=16
    #pragma HLS INTERFACE ap_memory port = b depth=16
    tiled_mmm(n, stride, dst, a, b);
    return 0;
}