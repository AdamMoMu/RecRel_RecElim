#include "header.hpp"

int ackerman(int m, int n)
{
    if(m==0) {
        return n+1;
    } 
    else if(m>0 && n==0) {
        return ackerman(m-1, 1);
    }
    else {
        n = ackerman(m,n-1);
        return ackerman(m-1,n);
    }
}


int process_top(int m , int n)
{
  return ackerman(m,n);
}