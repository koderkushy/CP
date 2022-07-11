template <class Node,
          class Calc,
          bool Normalized = false,
          bool SafeMode = true>
struct Segtree {
  public:
    explicit Segtree (const int n, const Node id, const Calc& F) : id(id), F(F), sz(n), N(size(n)), a(N << 1, id) {}
    explicit Segtree (const std::vector<Node>& x, const Node id, const Calc& F) : id(id), F(F), sz(x.size()), N(size(x.size())) {
        a.resize(N << 1, id);
        std::copy(x.begin(), x.end(), a.begin() + N);
        for (int i = N; --i; )
            update(i);
    }

    inline Node& get (const int i) const {
        if constexpr (SafeMode) assert(0 <= i and i < sz);
        return a[i + N];
    }

    void set (int i, const Node x) {
        if constexpr (SafeMode) assert(0 <= i and i < sz);
        for (a[i += N] = x; i >>= 1; )
            update(i);
    }

    inline Node ask_all () const {
        if constexpr (SafeMode) assert(Normalized);
        return a[1];
    }

    Node ask (int l, int r) {
        if constexpr (SafeMode) assert(0 <= l and l <= r and r <= sz);
        if constexpr (Normalized)
            if (l == 0 and r == sz)
                return ask_all();

        Node x = id, y = id;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l & 1) x = F(x, a[l++]);
            if (r & 1) y = F(a[--r], y);
        }
        return F(x, y);
    }

    // First j in [l, N] such that pred(F[l, j)) is FALSE, if pred is monotonic
    template<class Predicate>
    int max_right(int l, const Predicate& pred){
        if constexpr (SafeMode) assert(0 <= l and l <= N and pred(id)); 
        if (l == N) return l;

        Node prev = id, t = id;
        l += N;
        do{
            l >>= __builtin_ctz(l);
            if(!pred(F(prev, a[l]))){
                while(l < N)
                    if(pred(t = F(prev, a[l <<= 1])))
                        prev = t, l++;
                return l - N;
            }
            prev = F(prev, a[l++]);
        } while((l & -l) != l);
        return N;
    }

    // First j in [0, r] such that pred(F[j, r)) is TRUE, if pred is monotonic
    template<class Predicate>
    int min_left(int r, const Predicate& pred){
        if constexpr (SafeMode) assert(r > -1 and r <= N and pred(id));
        if(r == 0) return r;

        Node last = id, t = id;
        r += N;
        do{
            r--, r >>= __builtin_ctz(~r);
            if(r == 0) r = 1;
            if(!pred(F(a[r], last))){
                while(r < N)
                    if(pred(t = F(a[(r <<= 1) += 1], last)))
                        last = t, r--;
                return r + 1 - N;
            }
            last = F(a[r], last);
          } while((r & -r) != r);
        return 0;
    }

  private:
    const int N, sz;
    std::vector<Node> a;
    const Node id;
    const Calc F;

    inline size_t size (int n) {
        if constexpr (Normalized) return (1 << 32 - __builtin_clz(std::max(1, n-1)));
        else return n;
    }
    
    inline void update (int i) {
        a[i] = F(a[i << 1], a[i << 1 | 1]);
    }
};
