// Usage: https://judge.yosupo.jp/submission/95113
template<typename int64_t, typename uint, int64_t INF, bool FLIP = false>
struct LiChaoTreeOffline {
    struct Line {
        int64_t m, c;
        constexpr Line (const int64_t m, const int64_t c): m(m), c(c) {}
        constexpr Line (): Line(0, INF) {}
        inline int64_t operator() (uint i) const { return m * i + c; }
    };
    struct Node {
        Line z; uint l, r;
        inline void operator= (uint i) { l = r = i; }
    };
    
    int sz, N;
    std::vector<Node> a;

    template<class Iter>
    LiChaoTreeOffline (const Iter L, const Iter R)
    : LiChaoTreeOffline (std::vector(L, R)) {}

    LiChaoTreeOffline (std::vector<uint> leafs) {
        if (leafs.empty()) leafs.emplace_back(0);
        std::sort(leafs.begin(), leafs.end());
        leafs.erase(std::unique(leafs.begin(), leafs.end()), leafs.end());
        sz = leafs.size();
        N = sz <= 1? 1: (1 << 32 - __builtin_clz(sz - 1));

        a.resize(N << 1);
        std::copy(leafs.begin(), leafs.end(), a.begin() + N);
        std::fill(a.begin() + N + sz, a.end(), leafs.back());

        for (int q = N; --q; )
            a[q].l = a[q << 1].l, a[q].r = a[q << 1 | 1].r;
    }
    
    void up(const int64_t k, const int64_t b, const uint ql, const uint qr) {
        auto call = [&](auto&& call, int j) -> void {
            if (a[j].l >= qr || a[j].r < ql) return;
            if (a[j].l >= ql && a[j].r < qr) return up(k, b, j);
            if (ql < a[j<<1|1].l) call(call, j << 1);
            if (qr > a[j<<1|1].l) call(call, j<<1|1);
        }; 
        call(call, 1);
    }

    void up (int64_t m, int64_t c, int j) {
        Line x(m, c);
        if constexpr (FLIP) x = Line(-m, -c);
        int64_t c_l = x(a[j].l), c_r = x(a[j].r);
        while (true) {
            Line& z = a[j].z;
            const int64_t z_l = z(a[j].l), z_r = z(a[j].r);
            if (c_l <= z_l and c_r <= z_r) return void(z = x);
            if (c_l >= z_l and c_r >= z_r) return;
            j <<= 1;
            const int64_t c_m = x(a[j].r), z_m = z(a[j].r);
            if (c_m < z_m) {
                if (c_l > z_l) c_l = z_l, c_r = z_m;
                else c_l = z(a[++j].l), c_r = z_r;
                std::swap(x, z);
            } else {
                if (c_l > z_l) c_l = x(a[++j].l);
                else c_r = c_m;
            }
        }
    }

    int64_t get_min(uint x) {
        int64_t mi = a[1].z(x);
        for (int j = 1; j < N; ) {
            j <<= 1, j += x > a[j].r;
            mi = std::min(mi, a[j].z(x));
        }
        if constexpr (FLIP) mi = -mi;
        return mi;
    }
};