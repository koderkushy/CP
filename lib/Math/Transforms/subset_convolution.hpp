#ifndef CP_SUBSET_CONVOLUTION_HPP
#define CP_SUBSET_CONVOLUTION_HPP

// needs sos

template<class mint>
auto subset_conv (std::vector<mint>& a, const std::vector<mint>& b) {
    const int n = a.size();
    assert(__builtin_popcount(n) == 1 and b.size() == n);
    const int p = __builtin_ctz(n) + 1;

    std::vector a_cap(p, std::vector(n, mint()));
    std::vector b_cap(p, std::vector(n, mint()));
    std::vector c_cap(p, std::vector(n, mint()));

    for (int i = 0; i < n ; ++i)
        a_cap[__builtin_popcount(i)][i] = a[i],
        b_cap[__builtin_popcount(i)][i] = b[i];

    for (int i = 0; i < p; ++i)
        zeta_transform(a_cap[i]), zeta_transform(b_cap[i]);

    for (int i = 0; i < p; ++i) {
        for (int j = 0; j <= i; ++j)
            for (int mask = 0; mask < n; ++mask)
                c_cap[i][mask] += a_cap[j][mask] * b_cap[i - j][mask];

        zeta_transform(c_cap[i], true);
    }

    for (int i = 0; i < n ; ++i)
        a[i] = c_cap[__builtin_popcount(i)][i];
}

#endif // CP_SUBSET_CONVOLUTION_HPP