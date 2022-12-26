#ifndef CP_SEGMENT_TREE_HPP
#define CP_SEGMENT_TREE_HPP

template <
    class Node,
    class Calc,
    bool kNearestPowOf2 = false
>
class Segtree {
public:

    explicit Segtree (const int n, const Node id, const Calc& F)
    : sz(n), N(kNearestPowOf2 ? 1 << 32 - __builtin_clz(std::max(1, sz - 1)) : sz), a(N << 1, id), id(id), F(F)
    {

    }

    explicit Segtree (const std::vector<Node>& x, const Node id, const Calc& F)
    : sz(x.size()), N(kNearestPowOf2 ? 1 << 32 - __builtin_clz(std::max(1, sz - 1)) : sz), id(id), F(F)
    {
        a.resize(N << 1, id);
        std::copy(x.begin(), x.end(), a.begin() + N);

        for (int i = N; --i; )
            a[i] = F(a[i << 1], a[i << 1 | 1]);
    }

    void set (int i, const Node x) {
        // assert(0 <= i and i < sz);
        for (a[i += N] = x; i >>= 1; )
            a[i] = F(a[i << 1], a[i << 1 | 1]);
    }

    Node qu (int l, int r) const {
        // assert(0 <= l and l <= r and r <= sz);

        Node x = id, y = id;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l & 1) x = F(x, a[l++]);
            if (r & 1) y = F(a[--r], y);
        }

        return F(x, y);
    }

    // First j in [l, N] such that pred(F[l, j)) is FALSE, if pred is monotonic
    template<class Predicate>
    int max_right (int l, const Predicate& pred) const {
        // assert(0 <= l and l <= N and pred(id)); 
        if (l == N) return l;

        Node prev = id, t = id;
        l += N;

        do {
            l >>= __builtin_ctz(l);
            if (!pred(F(prev, a[l]))) {
                while (l < N)
                    if (pred(t = F(prev, a[l <<= 1])))
                        prev = t, l++;
                return l - N;
            }

            prev = F(prev, a[l++]);
        } while ((l & -l) != l);

        return N;
    }

    // First j in [0, r] such that pred(F[j, r)) is TRUE, if pred is monotonic
    template<class Predicate>
    int min_left (int r, const Predicate& pred) const {
        // assert(r > -1 and r <= N and pred(id));
        if(r == 0) return r;

        Node last = id, t = id;
        r += N;
        
        do {
            r--, r >>= __builtin_ctz(~r);
            if (r == 0) r = 1;
            if (!pred(F(a[r], last))){
                while (r < N)
                    if (pred(t = F(a[(r <<= 1) += 1], last)))
                        last = t, r--;
                return r + 1 - N;
            }

            last = F(a[r], last);
        } while((r & -r) != r);

        return 0;
    }

private:

    const int sz;
    const int N;
    std::vector<Node> a;
    const Node id;
    const Calc F;
};

#endif // CP_SEGMENT_TREE_HPP