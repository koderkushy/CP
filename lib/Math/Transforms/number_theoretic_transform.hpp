#ifndef CP_NTT_HPP
#define CP_NTT_HPP

#include "lib/Math/modular_integer.hpp"

namespace conv_ntt {

constexpr std::array primitive_roots {
    std::pair(998244353, 3),    // 23
    std::pair(1004535809, 3),   // 21
    std::pair(1012924417, 5),   // 21
    std::pair(1300234241, 3),   // 23
    std::pair(1484783617, 5)    // 23
};

template<class mint>
class Ntt {
public:

    static constexpr int kNttMod = mint::getmod();
    static constexpr int depth = __builtin_ctz(kNttMod - 1);
    static constexpr int root = []() { for (auto [mod, root]: primitive_roots) if (mod == kNttMod) return root; }();
    static constexpr int w = mint(root).pow(kNttMod - 1 >> depth).get();
    static constexpr int iw = mint(w).inv().get();

    static constexpr auto gen = [](int value) {        
        std::array<int, depth> x{};
        x[0] = value;
        for (int i = 1; i < depth; i++)
            x[i] = x[i-1] * static_cast<int64_t>(x[i-1]) % kNttMod;
        return x;
    };

    static constexpr auto rx = gen(w);
    static constexpr auto ix = gen(iw);
};

template<class mint>
auto butterfly (std::vector<mint> &a, const bool invert = false) {
    static constexpr auto Mod = mint::getmod();
    static constexpr int buf_size = 1 << 20;
    using Ntt = Ntt<mint>;

    static mint rbuf[buf_size], ibuf[buf_size];
    static bool once = [&]() {
        for (int len = 1, pow = Ntt::depth - 1; len < (1 << 20); len <<= 1, pow--) {
            auto rb = rbuf + len; rb[0] = 1;
            auto ib = ibuf + len; ib[0] = 1;
            for (int i = 1; i < len; i++)
                rb[i] = rb[i - 1] * Ntt::rx[pow],
                ib[i] = ib[i - 1] * Ntt::ix[pow];
        }
        return true;
    }();

    const int& N = a.size();
    assert(__builtin_popcount(N) == 1 and __builtin_ctz(N) <= Ntt::depth);

    for(int i = 1, j = 0; i < N; i++){
        int b = N >> 1;
        while(b & j)
            j ^= b, b >>= 1;
        j ^= b;
        if(j > i)
            std::swap(a[i], a[j]);
    }

    const auto &buf = invert? ibuf: rbuf;

    for (int len = 1, pow = Ntt::depth - 1; len < N; len <<= 1, pow--)
        for (int s = 0; s < N; s += (len << 1))
            for (int i = 0; i < len; i++) {
                const auto x = a[i + s], y = a[i + s + len] * buf[i + len];
                a[i + s] = x + y, a[i + s + len] = x - y;
            }

    if (invert) for (auto &v: a) v *= mint(N).inv();
}

template<class mint>
auto conv_naive (const std::vector<mint>& a, const std::vector<mint>& b, size_t MAX = 0) {
    std::vector<mint> c(std::max(static_cast<size_t>(0), a.size() + b.size() - 1));
    
    if (MAX == 0) MAX = c.size();
    
    for (int i = 0; i < std::min(a.size(), MAX); i++)
        for (int j = 0; j < std::min(b.size(), MAX); j++)
            c[i+j] += a[i] * b[j];

    return std::move(c);
}

template<class mint>
auto conv (std::vector<mint>& a, std::vector<mint> b, const size_t MAX = 0u){

    static constexpr int kNttMod = mint::getmod();

    if (std::min(a.size(), b.size()) < 40)
        return void(a = conv_naive(a, b, MAX));
    
    if (MAX and MAX < a.size()) a.resize(MAX);
    if (MAX and MAX < b.size()) b.resize(MAX);

    const int n = (a.size() + b.size()); 
    int m = 1 << 32 - __builtin_clz(n - 1);

    a.resize(m), butterfly(a);
    b.resize(m), butterfly(b);

    for (int i = 0; i < m; i++)
        a[i] *= b[i];

    butterfly(a, true), a.resize(n-1);
    if (MAX) a.resize(MAX);
}

template<class mint>
auto conv (std::vector<std::vector<mint>>& a, const size_t MAX = 0u){
    static auto cmp = [&](const int i, const int j){
        return a[i].size() > a[j].size();
    };

    std::priority_queue<int, std::vector<int>, decltype(cmp)> p(cmp);
    for (int i = 0; i < a.size(); i++)
        p.push(i);

    while (p.size() > 1){
        int x = p.top(); p.pop();
        int y = p.top(); p.pop();
        conv(a[x], a[y], MAX),
        p.push(x), a[y].clear();
    }

    a = {a[p.top()]};
}

template<class mint>
auto conv_arbitrary_mod (const std::vector<mint>& a, const std::vector<mint>& b, const size_t MAX = 0u) {
    
    if (std::min(a.size(), b.size()) < 40)
        return conv_naive(a, b, MAX);

    static constexpr int mod0 = 998244353, mod1 = 1300234241, mod2 = 1484783617;
    using mint0 = Mint<mod0>;
    using mint1 = Mint<mod1>;
    using mint2 = Mint<mod2>;

    const int n = (MAX? std::min(a.size(), MAX): a.size()),
              m = (MAX? std::min(b.size(), MAX): b.size());

    std::vector<mint0> a0(n), b0(m);
    std::vector<mint1> a1(n), b1(m);
    std::vector<mint2> a2(n), b2(m);

    for (int i = 0; i < n; i++) a0[i] = a[i].get(), a1[i] = a[i].get(), a2[i] = a[i].get();
    for (int i = 0; i < m; i++) b0[i] = b[i].get(), b1[i] = b[i].get(), b2[i] = b[i].get();

    conv(a0, b0, MAX), conv(a1, b1, MAX), conv(a2, b2, MAX);

    std::vector<mint> res(a0.size());

    static constexpr mint1 r01 = Mint<mod1>(mod0).inv().get();
    static constexpr mint2 r12 = Mint<mod2>(mod1).inv().get();
    static constexpr mint2 r02r12 = r12 / mod0;
    static constexpr mint m0 = mod0, m0m1 = m0 * mod1;

    for (int i = 0; i < res.size(); i++) {
        const int x0 = a0[i].get();
        const int x1 = ((a1[i].get() - x0) * r01).get();
        const int x2 = ((a2[i].get() - x0) * r02r12 - x1 * r12).get();
        
        res[i] = x0 + x1 * m0 + x2 * m0m1;
    }

    return std::move(res);
}

} // namespace conv_ntt

#endif // CP_NTT_HPP