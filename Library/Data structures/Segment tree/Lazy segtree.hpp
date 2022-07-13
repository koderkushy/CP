template<class Node, class Lazy, class Calc, class Comp, class Map, bool SafeMode = false>
struct LazyTree {
  public:
    template<typename... T>
    explicit LazyTree (const int n, const Node id, T... args) : LazyTree(std::vector(n, id), args ...) {}

    explicit LazyTree (const std::vector<Node>& x,
                       const Node id,
                       const Lazy null,
                       const Calc& F,
                       const Comp& L,
                       const Map& A)
    : sz(x.size()), N(size(x.size())), id(id), null(null), F(F), L(L), A(A) {
        a.resize(N << 1, id), b.resize(N, null);
        std::copy(x.begin(), x.end(), a.begin() + N);
        for (int i = N; --i; )
            update(i);
    }

    void set (int i, const Node x) {
        if constexpr (SafeMode) assert(0 <= i and i < sz);
        pull(i += N);
        a[i] = x;
        while (i >>= 1) update(i);
    }

    inline Node get (const int i) const {
        if constexpr (SafeMode) assert(0 <= i and i < sz);
        pull(i + N);
        return a[i + N];
    }

    inline Node ask_all () const {
        return a[1];
    }

    Node ask (int l, int r) {
        if constexpr (SafeMode) assert(0 <= l and l <= r and r <= sz);
        if (r - l == sz) return ask_all();
        if (l == r) return id;

        l |= N, r |= N;
        const int lctz = __builtin_ctz(l), rctz = __builtin_ctz(r), opt = std::min(lctz, rctz);
        pull_pair(l >>= opt, (r >>= opt) - 1);
        
        Node x = id, y = id;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) x = F(x, a[l++]);
            if (r & 1) y = F(a[--r], y);
        }
        return F(x, y);
    }

    void apply (int i, const Lazy u) {
        if constexpr (SafeMode) assert(0 <= i and i < sz);
        pull(i += N);
        a[i] = A(a[i], u);
        while (i >>= 1) update(i);
    }

    void apply (int l, int r, const Lazy u) {
        if constexpr (SafeMode) assert(0 <= l and l <= r and r <= sz);
        if (l == r) return;

        l |= N, r |= N;
        const int lctz = __builtin_ctz(l), rctz = __builtin_ctz(r), opt = std::min(lctz, rctz);
        pull_pair(l >>= opt, (r >>= opt) - 1);

        for (int L = l, R = r; L < R; L >>= 1, R >>= 1) {
            if (L & 1) apply_all(L++, u);
            if (R & 1) apply_all(--R, u);
        }

        l >>= lctz - opt, --r >>= rctz - opt;
        while (l >>= 1) update(l); while (r >>= 1) update(r);
    }
  
  private:
    std::vector<Node> a;
    std::vector<Lazy> b;
    const int N, sz;
    const Node id;
    const Lazy null;
    const Calc F;
    const Comp L;
    const Map A;

    inline size_t size (int x) { return 1 << 32 - __builtin_clz(std::max(1, x - 1)); }

    inline void update (int i) {
        a[i] = F(a[i << 1], a[i << 1 | 1]);
    }
    
    inline void apply_all (const int i, const Lazy u) {
        a[i] = A(a[i], u); if(i < N) b[i] = L(b[i], u);
    }

    inline void push (const int i) {
        apply_all(i<<1, b[i]), apply_all(i<<1|1, b[i]), b[i] = null;
    }

    void pull (int x) {
        static int C[30], i = 0;
        for (; x >>= 1; C[++i] = x);
        for (; i; i--) push(C[i]);
    }

    void pull_pair (int l, int r) {
        static int L[30], R[30], i = 0;
        while((l >>= 1) < (r >>= 1))
            ++i, L[i] = l, R[i] = r;
        pull(l << 1);
        for (; i; i--) push(L[i]), push(R[i]);
    }
};