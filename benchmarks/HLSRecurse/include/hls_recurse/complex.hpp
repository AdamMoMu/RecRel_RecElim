#ifndef complex_hpp
#define complex_hpp

#include <stdint.h>



struct complex_t
{
   float re;
    float im;

    complex_t()
    {}

    complex_t(float _re, float _im)
        : re(_re)
        , im(_im)
    {}

  static complex_t from_int(int _re, int _im=0)
  {
    return complex_t(_re, _im);
  }

  static complex_t from_float(float _re, float _im=0.0f)
  {
    return complex_t(_re, _im);
  }

  complex_t &operator=(const complex_t &o)
  {
    re=o.re;
    im=o.im;
    return *this;
  }

  float re_float()
  { return re ; }

  float im_float()
  { return im ; }

  int re_int()
  { return (int)re; }

  int im_int()
  { return (int)im; }
};

complex_t operator+(const complex_t &a, const complex_t &b)
{
    return complex_t(a.re+b.re, a.im+b.im);
}

complex_t operator-(const complex_t &a, const complex_t &b)
{
    return complex_t(a.re-b.re, a.im-b.im);
}

complex_t operator*(const complex_t &aa, const complex_t &bb)
{
    float a=aa.re, b=aa.im, c=bb.re, d=bb.im;
    return complex_t(
		     (a*c-b*d),
		     (b*c+a*d)
    );
}

#endif
