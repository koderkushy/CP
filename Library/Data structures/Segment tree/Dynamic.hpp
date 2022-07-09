template <class Node, class Comb>
struct DynamicSegtree{
    const int64_t N, L, R;
    const Node identity;
    const Comb com;

    vector<Node> v;
    int sz = 0;

    struct chash {
        size_t operator()(uint64_t x) const {
            static const uint64_t FIXED_RNDM = chrono::steady_clock::now().time_since_epoch().count();
            return x ^ FIXED_RNDM;
        }
    };

    unordered_map<int64_t, int, chash> a;
    
    Node read(int64_t i){ auto I = a.find(i); return I == a.end()? identity: v[I -> ss]; }
    void write(int64_t i, Node x){ auto I = a.find(i); I == a.end()? a[i] = sz++, v.pb(x): (v[I -> ss] = x, (void)0); }

    explicit DynamicSegtree (const int64_t L, const int64_t R, const Node& id, const Comb& com)
        : L(L), R(R), identity(id), com(com), N(R - L) {}

    void up (int64_t i, const Node x) {
        i -= L; assert(0 <= i and i < N);
        for (write(i += N, x); i >>= 1; )
            write(i, com(read(i << 1), read(i << 1 | 1)));
    }
    
    Node qu (int64_t l, int64_t r){
        l -= L, r -= L; assert(l <= r and l > -1 and r <= N);
        Node zl = identity, zr = identity;
        for(l += N, r += N; l < r; l >>= 1, r >>= 1)
            zl = (l & 1) ? com(zl, read(l++)): zl, zr = (r & 1) ? com(read(--r), zr): zr;
        return com(zl, zr);
    }
};
