#include "header.hpp"

int gcd(int a,int b)
{
  while(true){
    if (b == 0) 
      return a;
    int temp_a;
    temp_a = a;
    int temp_b;
    temp_b = b;
    b = temp_a % b;
    a = temp_b;
    continue; 
  }
}

int process_top(int a,int b)
{
  return gcd(a,b);
}
