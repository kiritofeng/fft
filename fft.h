#ifndef FFT_H

#define FFT_H

#include <algorithm>
#include <cmath>
#include <complex>
#include <vector>

const long double PI = acos(-1);

void FFT(std::vector<std::complex<long double>> &P, int inv = 1) {
    static std::vector<std::complex<long double>> roots;
    for(int i = 1, j = 0; i < P.size(); ++ i) {
        int b = P.size() >> 1;
        while(j >= b) {
            j -= b;
            b >>= 1;
        }
        if(i < j) std::swap(P[i], P[j]);
    }

    long double theta = inv * 2 * PI / P.size();

    if(roots.size() != P.size() / 2) {
        roots.clear();
        roots.reserve(P.size());
        for(int i=0; i < P.size() / 2; ++ i)
            roots[i] = std::complex<long double> (cos(theta * i), sin(theta * i));
    }

    for(int i = 2; i <= P.size(); i <<= 1) {
        int layer = P.size() / i;
        for(int j = 0; j < P.size(); j += i) {
            for(int k = 0; k < i / 2; ++ k) {
                auto u = P[j + k];
                auto v = P[j + k + i / 2] * roots[layer * k];
                P[j + k] = u + v;
                P[j + k + i / 2] = u - v;
            }
        }
    }

    if(inv == -1) {
        for(auto & a: P) {
            a /= P.size();
        }
    }
}

#endif
