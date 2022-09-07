template<
        class Node,
        class Less
        >
struct RangeMinQuery {

    explicit RangeMinQuery (const std::vector<Node>& arr, const Node& INF, Less&& less)
    : INF(INF), less(less), m(arr.size() + THIRTYONE >> FIVE), n(m << FIVE), a(arr)
    {
        a.resize(n, INF);
        v.assign(THIRTYTWO - __builtin_clz(m), std::vector(m, INF));

        for (auto i = a.begin(); i < a.end(); i += THIRTYTWO)
            v[0][i - a.begin() >> FIVE] = *std::min_element(i, i + THIRTYTWO, less);

        for (int l = 1, j = 1; (l << 1) <= m; l <<= 1, j++)
            for (int i = 0; i + (l << 1) <= m; i++)
                v[j][i] = std::min(v[j - 1][i], v[j - 1][i + l], less);

        std::vector pv(n, -THIRTYTWO), s(n, 0);
        for (int i = 0, si = 0; i < n; i++) {
            while (si and !less(a[s[si - 1]], a[i]))
                si--;
            if (si)
                pv[i] = s[si - 1];
            s[si++] = i;
        }

        mask.assign(n, 1);
        for (int i = 0; i < n; i++)
            if (i - pv[i] < THIRTYTWO)
                mask[i] |= (mask[pv[i]] << (i - pv[i]));
    }

    Node ask (int l, int r) {
        const int &lv = (l >> FIVE) + static_cast<bool>(l & THIRTYONE), &rv = r >> FIVE;
        const int &lv_times_sz = lv << FIVE;

        return 
            (lv_times_sz >= r) ?
                ask_tiny(r - 1, r - l)
            :
                std::min({
                    ask_tiny(lv_times_sz - 1, lv_times_sz - l),
                    ask_long(lv, rv),
                    ask_tiny(r - 1, r - (rv << FIVE))
                }, less);
    }

  private:
    static constexpr uint32_t THIRTYTWO = 32;
    static constexpr uint32_t THIRTYONE = 31;
    static constexpr uint32_t FIVE = 5;
    static constexpr uint32_t trim[] = {
        (1U<<0)-1, (1U<<1)-1, (1U<<2)-1, (1U<<3)-1, (1U<<4)-1, (1U<<5)-1, (1U<<6)-1, (1U<<7)-1, (1U<<8)-1, (1U<<9)-1, (1U<<10)-1, (1U<<11)-1, (1U<<12)-1, (1U<<13)-1, (1U<<14)-1, (1U<<15)-1, (1U<<16)-1, (1U<<17)-1, (1U<<18)-1, (1U<<19)-1, (1U<<20)-1, (1U<<21)-1, (1U<<22)-1, (1U<<23)-1, (1U<<24)-1, (1U<<25)-1, (1U<<26)-1, (1U<<27)-1, (1U<<28)-1, (1U<<29)-1, (1U<<30)-1, (1U<<31)-1, -1U
    };

    std::vector<Node> a;
    std::vector<uint32_t> mask;
    std::vector<std::vector<Node>> v;
    const uint32_t m, n;
    const Node INF;
    const Less less;

    inline Node ask_tiny (const int r, const int d) {
        if (d <= 0) return INF;
        return a[r - (THIRTYONE ^ __builtin_clz(mask[r] & trim[d]))];
    }

    inline Node ask_long (const int l, const int r) {
        if (r <= l) return INF;
        const int& ctz = THIRTYONE ^ __builtin_clz(r - l);
        return std::min(v[ctz][l], v[ctz][r - trim[ctz] - 1], less);
    }
};
