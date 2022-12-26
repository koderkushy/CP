#ifndef CP_ZETA_TRANSFORM_AKA_SOS_HPP
#define CP_ZETA_TRANSFORM_AKA_SOS_HPP

template<class mint>
auto zeta_transform (std::vector<mint>& a, const bool invert = false) {
    const size_t n = a.size();
    assert(__builtin_popcount(n) == 1);

    for (int i = 1; i < n; i <<= 1)
        for (int mask = 0; mask < n; ++mask)
            if ((mask & i) == 0)
                a[mask | i] += (invert? -1: 1) * a[mask];
}

#endif // CP_ZETA_TRANSFORM_AKA_SOS_HPP
