

#ifndef HEADER
#define HEADER


#include <cmath>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <iostream>

struct float_pair_t {
    float first;
    float second;
    float_pair_t() = default;
    float_pair_t(float _first, float _second)
        : first(_first), second(_second) {}
};

#ifndef __SYNTHESIS__
inline std::ostream& operator<<(std::ostream& dst, const float_pair_t& p) {
    dst << "Pair<" << p.first << "," << p.second << ">";
    return dst;
}
#endif

class XorShift128 {
    uint32_t x, y, z, w;
public:
    XorShift128()
        : x(123456789), y(362436069), z(521288629), w(88675123) {}

    uint32_t NextUInt() {
        uint32_t t = x ^ (x << 11);
        x = y; y = z; z = w;
        return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
    }

    inline float operator()() {
        // avoid unsigned->float conversion
        int tmp = NextUInt() & 0x7FFFFFFFul;
        return tmp * 0.0000000004656612873077392578125f;
    }

    inline void ranpt(float pt[], float regn[], int dim) {
        for (int j = 0; j < dim; ++j) {
            float low  = regn[j];
            float high = regn[dim + j];
            pt[j] = low + (high - low) * (*this)();
        }
    }

    template <unsigned Dim>
    inline void ranpt_d(float pt[Dim], float regn[Dim * 2]) {
        for (unsigned j = 0; j < Dim; ++j) {
            pt[j] = regn[j] + (regn[Dim + j] - regn[j]) * (*this)();
        }
    }
};

template <class T>
class RegionAllocator {
    T* m_free;
    T* m_max;
public:
    RegionAllocator(T* base, unsigned n)
        : m_free(base), m_max(base + n) {}

    T* alloc(unsigned n) {
        T* res = m_free;
        m_free += n;
        assert(m_free <= m_max);
        return res;
    }

    RegionAllocator NewRegion() const {
        // Subregion shares remaining memory
        unsigned remaining = m_max - m_free;
        return RegionAllocator(m_free, remaining);
    }

#ifndef __SYNTHESIS__
    void dump(std::ostream& o) const {
        o << "RegionAllocator<" << (void*)m_free << "," << (void*)m_max << ">";
    }
#endif
};

#ifndef __SYNTHESIS__
inline std::ostream& operator<<(std::ostream& o, const RegionAllocator<float>& r) {
    r.dump(o);
    return o;
}
#endif

class miser_test_config {
public:
    static const unsigned N = 4;
    typedef XorShift128 rng_t;
    inline static float f(const float p[N]) {
        float acc = 0;
        for (unsigned i = 0; i < N; ++i) acc += p[i] * p[i];
        return std::exp(-acc / 2);
    }
};

float_pair_t process_top(float regn[2 * miser_test_config::N], unsigned long npts,
                         float dith, typename miser_test_config::rng_t& rng,
                         RegionAllocator<float> region);


#endif
