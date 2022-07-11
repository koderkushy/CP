// Usage: https://judge.yosupo.jp/submission/95319
template<class int_t, bool FLIP = false>
struct FastRMQ {
    static constexpr int THIRTYTWO = 32;
    static constexpr int THIRTYONE = 31;
    static constexpr int FIVE = 5;
    using uint = uint32_t;

    static constexpr uint32_t trim32[] = {
        (1U<<0)-1, (1U<<1)-1, (1U<<2)-1, (1U<<3)-1, (1U<<4)-1, (1U<<5)-1, (1U<<6)-1, (1U<<7)-1, 
        (1U<<8)-1, (1U<<9)-1, (1U<<10)-1, (1U<<11)-1, (1U<<12)-1, (1U<<13)-1, (1U<<14)-1, (1U<<15)-1, 
        (1U<<16)-1, (1U<<17)-1, (1U<<18)-1, (1U<<19)-1, (1U<<20)-1, (1U<<21)-1, (1U<<22)-1, (1U<<23)-1, 
        (1U<<24)-1, (1U<<25)-1, (1U<<26)-1, (1U<<27)-1, (1U<<28)-1, (1U<<29)-1, (1U<<30)-1, (1U<<31)-1, -1U
    };

    static constexpr int pow_sz[] = {
        1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 
        1<<8, 1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15, 
        1<<16, 1<<17, 1<<18, 1<<19, 1<<20, 1<<21, 1<<22, 1<<23, 
        1<<24, 1<<25, 1<<26, 1<<27, 1<<28, 1<<29, 1<<30, 1<<31
    };

    std::vector<int_t> a;
    std::vector<uint> mask;
    std::vector<std::vector<int_t>> v;
    static constexpr int_t INF = std::numeric_limits<int_t>::max();
    const int m, n;

    template<class Iter>
    FastRMQ (const Iter l, const Iter r)
    : FastRMQ(std::vector(l, r)) {}

    FastRMQ (std::vector<int_t> arr) : m(arr.size() + THIRTYONE >> FIVE), n(m << FIVE) {
        if constexpr (FLIP) std::for_each(arr.begin(), arr.end(), [](int_t& x) { x = -x; });

        a.assign(arr.begin(), arr.end()), a.resize(n, INF);
        v.assign(THIRTYTWO - __builtin_clz(m), std::vector(m, INF));
        mask.assign(n, 1);

        for (int i = 0; i < n; i += THIRTYTWO)
            v[0][i >> FIVE] = *std::min_element(a.begin() + i, a.begin() + i + THIRTYTWO);
        for (int l = 1, j = 1; (l << 1) <= m; l <<= 1, j++) {
            for (int i = 0; i + (l << 1) <= m; i++)
                v[j][i] = std::min(v[j-1][i], v[j-1][i + l]);
        }
        std::vector pv(n, -THIRTYTWO);
        for (int i = 0, si = 0, s[n]; i < n; i++) {
            while(si and !(a[s[si-1]] < a[i])) si--;
            if(si) pv[i] = s[si-1];
            s[si++] = i;
        }
        for (int i = 0; i < n; i++) if(i - pv[i] < THIRTYTWO)
            mask[i] |= (mask[pv[i]] << (i - pv[i]));
    }

    inline int_t qu_tiny (const int r, const int d) {
        if(d <= 0) return INF;
        return a[r - (THIRTYONE ^ __builtin_clz(mask[r] & trim32[d]))];
    }
    inline int_t qu_long (const int l, const int r) {
        if(r <= l) return INF;
        const int& ctz = THIRTYONE ^ __builtin_clz(r - l);
        return std::min(v[ctz][l], v[ctz][r - pow_sz[ctz]]);
    }
    int_t qu (int l, int r) {
        const int &lv = (l >> FIVE) + static_cast<bool>(l & THIRTYONE), &rv = r >> FIVE;
        const int &lv_times_sz = lv << FIVE;
        if constexpr (FLIP) return - ((lv_times_sz >= r) ? qu_tiny(r-1, r - l) : std::min({ qu_tiny(lv_times_sz-1, lv_times_sz - l), qu_long(lv, rv), qu_tiny(r-1, r - (rv << FIVE)) }));
        else return (lv_times_sz >= r) ? qu_tiny(r-1, r - l) : std::min({ qu_tiny(lv_times_sz-1, lv_times_sz - l), qu_long(lv, rv), qu_tiny(r-1, r - (rv << FIVE)) });
    }
};
