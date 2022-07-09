template<class Node, class Compare>
struct FastRMQ {
    static constexpr int sz = 32;
    static constexpr int sz_minus_one = sz - 1;
    static constexpr int sz_ctz = __builtin_ctz(sz);
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

    Node *a, **v;
    uint *mask;
    const Node identity;
    const Compare cmp;
    const int m, n;

    void init () {
        v = new Node* [sz - __builtin_clz(m)];
        v[0] = new Node [m];
        for (int i = 0; i < n; i += sz)
            v[0][i >> sz_ctz] = *std::min_element(a + i, a + i + sz);
        for (int l = 2, j = 1; l <= m; l <<= 1, j++) {
            v[j] = new Node [m - l + 1];
            for (int i = 0; i + l <= m; i++)
                v[j][i] = std::min(v[j-1][i], v[j-1][i + (l >> 1)]);
        }
        int pv[n]; std::fill(pv, pv + n, -sz);
        for (int i = 0, si = 0, s[n]; i < n; i++) {
            while(si and !(a[s[si-1]] < a[i])) si--;
            if(si) pv[i] = s[si-1];
            s[si++] = i;
        }
        mask = new uint [n];
        std::fill(mask, mask + n, 1);
        for (int i = 0; i < n; i++) if(i - pv[i] < sz)
            mask[i] |= (mask[pv[i]] << (i - pv[i]));
    }

    template<class Iter>
    explicit FastRMQ (const Iter l, const Iter r, const Node& id, const Compare& cmp)
    : m(r - l + sz_minus_one >> sz_ctz), n(m << sz_ctz), identity(id), cmp(cmp) {
        a = new Node [n]; std::copy(l, r, a), std::fill(a + int(r - l), a + n, identity);
        init();
    }

    Node qu (int l, int r) {
        const int &lv = (l >> sz_ctz) + static_cast<bool>(l & sz_minus_one), &rv = r >> sz_ctz;
        const int &lv_times_sz = lv << sz_ctz;
        auto qu_tiny = [&](int r, int d) {
            if(d <= 0) return identity;
            return a[r - (sz_minus_one ^ __builtin_clz(mask[r] & trim32[d]))];
        };
        auto qu_long = [&](int l, int r) {
            if(r <= l) return identity;
            const int& ctz = sz_minus_one ^ __builtin_clz(r - l);
            return std::min(v[ctz][l], v[ctz][r - pow_sz[ctz]]);
        };
        if(lv_times_sz >= r) return qu_tiny(r-1, r - l);
        return std::min({
            qu_tiny(lv_times_sz-1, lv_times_sz - l),
            qu_long(lv, rv),
            qu_tiny(r-1, r - (rv << sz_ctz))
        });
    }
};
