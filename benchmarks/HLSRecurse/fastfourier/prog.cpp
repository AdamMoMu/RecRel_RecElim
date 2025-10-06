#include "header.hpp"

using namespace hls_recurse;

void fft(int log2n, const complex_t *pIn, complex_t *pOut)
{
    const complex_t wn_table[16]={
      complex_t::from_float(1,     -2.44929359829471e-16),
      complex_t::from_float(-1,     1.22464679914735e-16),
      complex_t::from_float(6.12323399573677e-17, 1),
      complex_t::from_float(0.707106781186548,         0.707106781186547),
      complex_t::from_float(0.923879532511287,          0.38268343236509),
      complex_t::from_float(0.98078528040323 ,        0.195090322016128),
      complex_t::from_float(0.995184726672197 ,       0.0980171403295606),
      complex_t::from_float(0.998795456205172,         0.049067674327418),
      complex_t::from_float(0.999698818696204,        0.0245412285229123),
      complex_t::from_float(0.999924701839145,        0.0122715382857199),
      complex_t::from_float(0.999981175282601,       0.00613588464915448),
      complex_t::from_float(0.999995293809576,       0.00306795676296598),
      complex_t::from_float(0.999998823451702,       0.00153398018628477),
      complex_t::from_float(0.999999705862882,      0.000766990318742704),
      complex_t::from_float(0.999999926465718,      0.000383495187571396),
      complex_t::from_float(0.999999981616429,      0.000191747597310703)
    };

    int n=1<<log2n;
    complex_t wn=wn_table[log2n];
    int sIn=1;
    int sOut=1;

    run_function_old<void>(
        IfElse([&](){ return n<=2; },
            [&](){
                if (n == 1){
                    pOut[0] = pIn[0];
                }else if (n == 2){
                    pOut[0] = pIn[0]+pIn[sIn];
                    pOut[sOut] = pIn[0]-pIn[sIn];
                }
            },
            Sequence(
                [&](){
                    n=n/2;
                },
                Recurse([&](){ return make_hls_state_tuple(n,wn*wn,pIn,2*sIn,pOut,sOut); }),
                Recurse([&](){ return make_hls_state_tuple(n,wn*wn,pIn+sIn,2*sIn,pOut+sOut*n,sOut); }),
                [&](){
                    complex_t w=complex_t::from_int(1);

                    for (int j=0;j<n;j++){
                        complex_t t1 = w*pOut[n+j];
                        complex_t t2 = pOut[j]-t1;
                        pOut[j] = pOut[j]+t1;
                        pOut[j+n] = t2;
                        w = w*wn; // This introduces quite a lot of numerical error
                    }
                }
            )
        ),
        n, wn, pIn, sIn, pOut, sOut
    );
}

// Top-level function for HLS
int process_top(int log2n, const complex_t* pIn, complex_t* pOut) {
    #pragma HLS INTERFACE port=pIn  ap_memory depth=8
    #pragma HLS INTERFACE port=pOut ap_memory depth=8
    fft(log2n, pIn, pOut);
    return 0;
}