// Needs Normalized Segtree
// Usage: https://codeforces.com/edu/course/2/lesson/5/4/practice/contest/280801/submission/163684450
template <class Node, class Lazy, class MergeNode, class MergeLazy, class ApplyLazy>
struct LazyTree : public Segtree<Node, MergeNode, true> {
    using Segtree = Segtree<Node, MergeNode, true>; using Segtree::N; using Segtree::id; using Segtree::a; using Segtree::F; using Segtree::update;
    template<typename U> using Vec = std::vector<U>;

    const int       logN;
    Vec<Lazy>       b;
    const MergeLazy L;
    const ApplyLazy A;
    const Lazy      lazy_id;
    
    template<typename... T>
    explicit LazyTree (const int n, const Node& nid, T... args)
        : LazyTree(std::vector(n, nid), nid, args...) {}

    explicit LazyTree (const Vec<Node>& x,
                       const Node& nid,
                       const Lazy& lid,
                       const MergeNode& ncom,
                       const MergeLazy& lcom,
                       const ApplyLazy& appl)
        : lazy_id(lid), L(lcom), A(appl),
          logN(32 - __builtin_clz(x.size() - 1)), Segtree (x, nid, ncom) {
        b.assign(N, lazy_id);
    }

    inline void apply_all (const int i, const Lazy u) {
        a[i] = A(a[i], u); if(i < N) b[i] = L(b[i], u);
    }

    inline void pull (int x) {
        static int C[30], i = 0;
        while (x >>= 1) C[++i] = x;
        while (i) apply_all(C[i]<<1, b[C[i]]), apply_all(C[i]<<1|1, b[C[i]]), b[C[i]] = lazy_id, i--;
    }

    void up (int l, int r, const Lazy x) {
        assert(0 <= l and l <= r and r <= N);
        l += N, r += N; if(l == r) return;
        const int lctz = __builtin_ctz(l), rctz = __builtin_ctz(r);
        const int opt = std::min(lctz, rctz);
        pull(l >>= opt), pull((r >>= opt) - 1);
        for(int p = l, q = r; p < q; p >>= 1, q >>= 1){
            if(p&1) apply_all(p++, x); if(q&1) apply_all(--q, x);
        }
        l >>= lctz - opt; --r >>= rctz - opt; 
        while (l >>= 1) update(l);
        while (r >>= 1) update(r);
    }

    Node qu (int l, int r) {
        assert(0 <= l and l <= r and r <= N), l += N, r += N; 
        if(l == r) return id; if(r-l == N) return a[1];
        const int lctz = __builtin_ctz(l), rctz = __builtin_ctz(r);
        const int opt = std::min(lctz, rctz);
        pull(l >>= opt), pull((r >>= opt) - 1);
        Node zl = id, zr = id;
        for(; l < r; l >>= 1, r >>= 1){
            if(l&1) zl = F(zl, a[l++]);
            if(r&1) zr = F(a[--r], zr);
        }
        return F(zl, zr);
    }
};