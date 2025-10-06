#include "header.hpp"

/*
 * This program was taken from HLSRecurse. 
 * D. B. Thomas, "Synthesisable recursion for C++ HLS tools," 
 * doi: 10.1109/ASAP.2016.7760777. 
 */

int sub(int v,int h, int stride, int n) {
    return v*stride*n/2+h*n/2; 
}

void tiled_mmm(int n, int stride, float *dst, float *a, float *b)
{
    if( n<=16 ){
        for(int r=0; r<n; r++){
            for(int c=0; c<n; c++){
                float acc=dst[r*stride+c];
                for(int i=0; i<n; i++){
                    acc += a[r*stride+i] * b[i*stride+c];
                }
                dst[r*stride+c]=acc;
            }
        }
    }else{

        tiled_mmm(n/2, stride, dst+sub(0,0, stride, n), a+sub(0,0, stride, n), b+sub(0,0, stride, n));
        tiled_mmm(n/2, stride, dst+sub(0,1, stride, n), a+sub(0,0, stride, n), b+sub(0,1, stride, n));
        tiled_mmm(n/2, stride, dst+sub(1,0, stride, n), a+sub(1,0, stride, n), b+sub(0,0, stride, n));
        tiled_mmm(n/2, stride, dst+sub(1,1, stride, n), a+sub(1,0, stride, n), b+sub(0,1, stride, n));

        tiled_mmm(n/2, stride, dst+sub(0,0, stride, n), a+sub(0,1, stride, n), b+sub(1,0, stride, n));
        tiled_mmm(n/2, stride, dst+sub(0,1, stride, n), a+sub(0,1, stride, n), b+sub(1,1, stride, n));
        tiled_mmm(n/2, stride, dst+sub(1,0, stride, n), a+sub(1,1, stride, n), b+sub(1,0, stride, n));
        tiled_mmm(n/2, stride, dst+sub(1,1, stride, n), a+sub(1,1, stride, n), b+sub(1,1, stride, n));
    }
}

int process_top(int n, int stride, float *dst, float *a, float *b) {
    #pragma HLS INTERFACE ap_memory port = dst depth=16
    #pragma HLS INTERFACE ap_memory port = a depth=16
    #pragma HLS INTERFACE ap_memory port = b depth=16
    tiled_mmm(n, stride, dst, a, b);
    return 0;
}