#include "header.hpp"

int gcd(int a, int b)
{
  if (b == 0)
    return a;
  return gcd(b, a % b);
}


int process_top(int a , int b)
{
  return gcd(a, b);
}