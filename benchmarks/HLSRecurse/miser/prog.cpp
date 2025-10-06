#include "header.hpp"

#include <cmath>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include "../include/hls_recurse/state_machine_self_recursion.hpp"

class f2_miser_impl
{
public:
    template<unsigned TDim, class TFunc, class TRng>
    static float_pair_t eval(TFunc func, float regn[2*TDim], int npts, float dith, TRng &rng, RegionAllocator<float> region)

    //Monte Carlo samples a user-supplied ndim-dimensional function func in a rectangular volume
    //specified by regn[1..2*ndim], a vector consisting of ndim “lower-left” coordinates of the
    //region followed by ndim “upper-right” coordinates. The function is sampled a total of npts
    //times, at locations determined by the method of recursive stratified sampling. The mean value
    //of the function in the region is returned as ave; an estimate of the statistical uncertainty of ave
    //(square of standard deviation) is returned as var. The input parameter dith should normally
    //be set to zero, but can be set to (e.g.) 0.1 if func’s active region falls on the boundary of a
    //power-of-two subdivision of region.

    {
        const float PFAC=0.1f;
        const int MNPT=15;
        const int MNBS=60;
        const float TINY=1.0e-30f;
        const float BIG=1.0e30f;        //Here PFAC is the fraction of remaining function evaluations used at each stage to explore the
        //variance of func. At least MNPT function evaluations are performed in any terminal subregion;
        //a subregion is further bisected only if at least MNBS function evaluations are available. We take
        //MNBS = 4*MNPT.


        float *regn_temp;

        int long n,npre,nptl,nptr;
        int j,jb;

        float fracl,fval;
        float rgl,rgm,rgr,sigl,siglb,sigr,sigrb;
        float sum,sumb,summ,summ2;

        float pt[TDim];
        float *rmid;

        float_pair_t resl, resr;

        return run_function_old<float_pair_t>(
            IfElse( [&](){ return npts < MNBS; },  // Too few points to bisect; do straight
                Sequence(
                    [&](){
                        summ=summ2=0.0; //Monte Carlo.
                        for (n=0;n<npts;n++) {
                            rng.ranpt(pt,regn,TDim);
                            fval=func(pt);
                            summ += fval;
                            summ2 += fval * fval;
                        }
                    },
                    Return([&](){ return float_pair_t(
                        summ/npts,
                        max(TINY,(summ2-summ*summ/npts)/(npts*npts))
                    );})
                )
            ,   //Do the preliminary (uniform) sampling.
                Sequence(
                    [&](){
                        rmid=region.alloc(TDim);
                        // LEGUP-HACK : Convert from unsigned to int
                        npre=max((int)(npts*PFAC),(int)MNPT);

                        {
                            float fmaxl[TDim];
                            float fmaxr[TDim];
                            float fminl[TDim];
                            float fminr[TDim];
                            for (j=0;j<TDim;j++) { // Initialize the left and right bounds for
                                float s=rng()*dith;
                                rmid[j]=(0.5+s)*regn[j]+(0.5-s)*regn[TDim+j];
                                fminl[j]=fminr[j]=BIG;
                                fmaxl[j]=fmaxr[j] = -BIG;
                            }
                            for (n=0;n<npre;n++) { // Loop over the points in the sample.
                                rng.ranpt(pt,regn,TDim);
                                fval=func(pt);
                                for (j=0;j<TDim;j++) { // Find the left and right bounds for each
                                    if (pt[j]<=rmid[j]) { // dimension.
                                        fminl[j]=min(fminl[j],fval);
                                        fmaxl[j]=max(fmaxl[j],fval);
                                    }
                                    else {
                                        fminr[j]=min(fminr[j],fval);
                                        fmaxr[j]=max(fmaxr[j],fval);
                                    }
                                }
                            }
                            sumb=BIG; //Choose which dimension jb to bisect.
                            jb=0;
                            siglb=sigrb=1.0;
                            for (j=0;j<TDim;j++) {
                                if (fmaxl[j] > fminl[j] && fmaxr[j] > fminr[j]) {
                                    sigl=max(TINY,powf(fmaxl[j]-fminl[j],1.5f));
                                    sigr=max(TINY,powf(fmaxr[j]-fminr[j],1.5f));
                                    sum=sigl+sigr; //Equation (7.8.24), see text.
                                    if (sum<=sumb)
                                    {
                                        sumb=sum;
                                        jb=j;
                                        siglb=sigl;
                                        sigrb=sigr;
                                    }
                                }
                            }
                        }
                        if (!jb){
                            jb=rng.NextUInt()%TDim;
                        }
                        rgl=regn[jb]; // Apportion the remaining points between
                        rgm=rmid[jb]; // left and right.
                        rgr=regn[TDim+jb];
                        fracl=fabs((rgm-rgl)/(rgr-rgl));
                        // LEGUP-HACK : remove float->unsigned
                        nptl=(int)(MNPT+(npts-npre-2*MNPT)*fracl*siglb
                            /(fracl*siglb+(1.0-fracl)*sigrb)); //Equation (7.8.23).
                        nptr=npts-npre-nptl;

                        regn_temp=region.alloc(2*TDim); //Now allocate and integrate the two sub
                        for(j=0;j<TDim;j++) { // regions.
                            regn_temp[j]=regn[j];
                            regn_temp[TDim+j]=regn[TDim+j];
                        }

                        regn_temp[TDim+jb]=rmid[jb];
                    },
                    RecurseWithResult(resl, [&](){ return make_hls_state_tuple(
                        regn_temp,nptl,dith, region.NewRegion()
                    );}),
                    [&](){
                        regn_temp[jb]=rmid[jb]; //Dispatch recursive call; will return back
                        regn_temp[TDim+jb]=regn[TDim+jb]; // here eventually.
                    },
                    RecurseWithResult(resr, [&](){ return make_hls_state_tuple(
                        regn_temp,nptr,dith, region.NewRegion()
                    );}),
                    Return([&](){
                        return float_pair_t(
                            fracl*resl.first+(1-fracl)*resr.first,
                            fracl*fracl*resl.second+(1-fracl)*(1-fracl)*resr.second
                        );
                    })
                    // Combine left and right regions by equation (7.8.11) (1st line).
                )
            )
            ,
            regn, npts, dith, region,
            regn_temp, jb, resl, resr, fracl, nptl, nptr, rmid
        );
    }
} f2_miser_generic;

// Top-level HLS wrapper
float_pair_t process_top(float regn[2 * miser_test_config::N], unsigned long npts,
                         float dith, typename miser_test_config::rng_t& rng,
                         RegionAllocator<float> region) {
    #pragma HLS INTERFACE m_axi port=regn offset=slave bundle=gmem
    return f2_miser_generic.template eval<miser_test_config::N,decltype(miser_test_config::f),miser_test_config::rng_t>(miser_test_config::f, regn, npts, dith, rng, region);
}
