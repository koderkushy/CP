template <class Node, class Calc>
struct Segtree {
    const int N, sz; std::vector<Node> a;
    const Node id; const Calc F;

    size_t RESIZE (size_t x, bool mode) const { if (__builtin_popcount(x) == 1) return x; return mode ? (1 << 32 - __builtin_clz(x-1)) : x; }
    explicit Segtree (const std::vector<Node>& x, Node id, const Calc& F) : Segtree(x.begin(), x.end(), id, F) {}
    explicit Segtree (const int n, Node id, const Calc& F) : Segtree(std::vector(n, id), id, F) {}
    template<typename RandomIt>
    explicit Segtree (RandomIt L, RandomIt R, Node id, const Calc& F) : sz(R - L), N(RESIZE(R-L, 1)), id(id), F(F) {
        a.resize(N << 1, id), std::copy(L, R, a.begin() + N);
        for (int i = N; --i; ) a[i] = F(a[i << 1], a[i << 1 | 1]);
    }
    
    void up (int i, const Node x) {
        // assert(i > -1 and i < sz);
        for(a[i += N] = x; i >>= 1; ) a[i] = F(a[i << 1], a[i << 1 | 1]);
    }

    Node qu (int l, int r) {
        // assert(l <= r and l > -1 and r <= sz);
        if(l == 0 and r == sz) return a[1];     // this needs resize to 2^K
        Node zl = id, zr = id;
        for(l += N, r += N; l < r; l >>= 1, r >>= 1)
            zl = (l & 1) ? F(zl, a[l++]): zl, zr = (r & 1) ? F(a[--r], zr): zr;
        return F(zl, zr);
    }
};