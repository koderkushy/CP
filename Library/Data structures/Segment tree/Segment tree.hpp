// Usage: https://codeforces.com/edu/course/2/lesson/4/1/practice/contest/273169/submission/163676743
template <class Node, class Calc, bool Normalized = false>
struct Segtree {
    using Vec = std::vector<Node>;
    const int N, sz; Vec a;
    const Node id; const Calc F;
    const int size (int n) { if constexpr (Normalized) return (1 << 32 - __builtin_clz(std::max(1, n-1))); else return n; }

    explicit Segtree (const Vec& x, const Node id, const Calc& F): Segtree(x.begin(), x.end(), id, F) {}
    explicit Segtree (const int n, const Node id, const Calc& F): N(size(n)), id(id), F(F), a(N << 1, id) {}

    template<typename Iter>
    explicit Segtree (const Iter L, const Iter R, const Node id, const Calc& F)
    : sz(R - L), N(size(R-L)), id(id), F(F), a(N << 1, id) {
        std::copy(L, R, a.begin() + N);
        for (int i = N; --i; ) update(i);
    }
    
    inline void update (const int i) { a[i] = F(a[i << 1], a[i << 1 | 1]); }

    void up (int i, const Node x) {                                                     // assert(i > -1 and i < sz);
        for(a[i += N] = x; i >>= 1; ) update(i);
    }

    Node qu (int l, int r) {                                                            // assert(l <= r and l > -1 and r <= sz);
        if constexpr (Normalized) if (r - l == sz) return a[1];
        Node zl = id, zr = id;
        for(l += N, r += N; l < r; l >>= 1, r >>= 1){
            if (l & 1) zl = F(zl, a[l++]); if (r & 1) zr = F(a[--r], zr);
        }
        return F(zl, zr);
    }
};