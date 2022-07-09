// Needs Segtree
template <class Node, class Lazy, class Ncom, class Lcom, class Appl>
struct LazyTree : public Segtree<Node, Ncom> {

    using ST = Segtree<Node, Ncom>;

    const int& N = ST::N;
    const Node& node_id = ST::identity;
    vector<Node>& a = ST::a;
    const Ncom& node_com = ST::com;

    const int logN;
    vector<Lazy> b;
    const Lcom lazy_com;
    const Appl apply_lazy;
    const Lazy lazy_id;
 
    template <typename... T>
    explicit LazyTree (const int n, const Node& nid, T... args)
        : LazyTree(vector(n, nid), args...) {}
    explicit LazyTree (const vector<Node>& x,
                       const Node& nid,
                       const Lazy& lid,
                       const Ncom& ncom,
                       const Lcom& lcom,
                       const Appl& appl)
        : lazy_id(lid), lazy_com(lcom), apply_lazy(appl),
          logN(32 - __builtin_clz(x.size() - 1)), ST (x, nid, ncom) {
        b = vector(N, lazy_id);
    }

    void combine (const int i) { a[i] = node_com(a[i<<1], a[i<<1|1]); }
    void r_apply (const int i, const Lazy u) {
        a[i] = apply_lazy(a[i], u); if(i < N) b[i] = lazy_com(b[i], u);
    }
    void push (const int i) {
        r_apply(i<<1, b[i]), r_apply(i<<1|1, b[i]); b[i] = lazy_id;
    }

    void up (int l, int r, const Lazy x) {
        assert(0 <= l and l <= r and r <= N);
        l += N, r += N; if(l == r) return;
        const int lctz = __builtin_ctz(l), rctz = __builtin_ctz(r);
        for(int j = logN; j > lctz; --j) push(l >> j);
        for(int j = logN; j > rctz; --j) push((r-1) >> j);
        for(int p = l, q = r; p < q; p >>= 1, q >>= 1){
            if(p&1) r_apply(p++, x); if(q&1) r_apply(--q, x);
        }
        for (int i = lctz + 1; i <= logN; ++i) combine(l >> i);
        for (int i = rctz + 1; i <= logN; ++i) combine((r - 1) >> i);
    }

    Node qu (int l, int r) {
        assert(0 <= l and l <= r and r <= N), l += N, r += N; 
        if(l == r) return node_id; if(r-l == N) return a[1];
        const int lctz = __builtin_ctz(l), rctz = __builtin_ctz(r);
        for(int j = logN; j > lctz; --j) push(l >> j);
        for(int j = logN; j > rctz; --j) push((r-1) >> j);
        Node zl = node_id, zr = node_id; for(; l < r; l >>= 1, r >>= 1){
            if(l&1) zl = node_com(zl, a[l++]); if(r&1) zr = node_com(a[--r], zr);
        } return node_com(zl, zr);
    }
};

