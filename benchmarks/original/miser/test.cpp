
#include "header.hpp"
#include <iostream>

int main()
{
    const unsigned N = miser_test_config::N;
    float p[1024];

    // Initial N*2 region
    p[0]=0;
    p[1]=0;
    p[2]=0;
    p[3]=0;
    p[4]=1;
    p[5]=2;
    p[6]=3;
    p[7]=4;

    XorShift128 rng;
    RegionAllocator<float> region(p+8, sizeof(p)/sizeof(p[0])-8);

    unsigned freeStart= N*2;

    auto res= process_top(p, 100000, 0.05f, rng, region);
    
    std::cout << *(int*)&res.first << std::endl;
    std::cout << *(int*)&res.second << std::endl;

    return 0;
}