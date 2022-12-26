#ifndef CP_FWHT_HPP
#define CP_FWHT_HPP

namespace conv_fwht {

template<class mint, char type = '^'>
auto fwht_inplace (std::vector<mint>& a, const bool invert = false) {
    const size_t n = a.size();
    assert(__builtin_popcount(n) == 1);

    for (int len = 1; len < n; len <<= 1) {
        for (int s = 0; s < n; s += (len << 1)) {
            for (int i = s; i < s + len; i++) {
                const auto u = a[i], v = a[i + len];
                if constexpr (type == '^') a[i] = u + v, a[i + len] = u - v;
                if constexpr (type == '|') a[i + len] += invert ? -u: u;
                if constexpr (type == '&') a[i] += invert ? -v: v;
            }
        }
    }

    if (type == '^' and invert) {
        const auto n_inv = 1 / mint(n);
        for (auto& x: a)
            x *= n_inv;
    }
}

template<class mint, char type = '^'>
auto conv (std::vector<mint>& a, std::vector<mint> b) {
    const size_t n = a.size();
    assert(n == b.size() and __builtin_popcount(n) == 1);

    fwht_inplace<mint, type>(a), fwht_inplace<mint, type>(b);

    for (int i = 0; i < n; i++)
        a[i] *= b[i];

    fwht_inplace<mint, type>(a, true);
}

} // namespace conv_fwht

#endif // CP_FWHT_HPP