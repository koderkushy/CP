#ifndef CP_IMPLICIT_SEGMENT_TREE_HPP
#define CP_IMPLICIT_SEGMENT_TREE_HPP

template <
    class Node,
    class Calc,
    class int64
>
class ImplicitSegtree {
    static_assert(std::is_integral_v<int64>);
public:

    class CustomHash {
    public:
        size_t operator() (int64 x) const {
            static const int64 FIXED_RNDM = std::chrono::steady_clock::now().time_since_epoch().count();
            return x ^ FIXED_RNDM;
        }
    };

    explicit ImplicitSegtree (const int64 L, const int64 R, const Node& id, const Calc& F)
    : L(L), R(R), id(id), F(F), N(R - L) {}

    inline const Node& get (const int64 i) const {
        assert(L <= i and i < R);
        return read(i - L + N);
    }

    void set (int64 i, const Node x) {
        assert(L <= i and i < R);
        for (write(i += N - L, x); i >>= 1; )
            update(i);
    }

    Node qu (int l, int r) const {
        assert(L <= l and l <= r and r <= R);

        Node x = id, y = id;
        for (l += N - L, r += N - L; l < r; l >>= 1, r >>= 1) {
            if (l & 1) x = F(x, read(l++));
            if (r & 1) y = F(read(--r), y);
        }

        return F(x, y);
    }

private:
    const int64 N, L, R;
    const Node id;
    const Calc F;

    std::vector<Node> v;
    int sz = 0;

    std::unordered_map<int64, int, CustomHash> a;

    const Node& read (const int64 i) const {
        auto I = a.find(i);
        return I == a.end()? id: v[I -> second];
    }

    void write (int64 i, Node x) {
        auto I = a.find(i);
        I == a.end()? a[i] = sz++, v.push_back(x): (v[I -> second] = x, (void)0);
    }

    inline void update (const int64 i) {
        write(i, F(read(i << 1), read(i << 1 | 1)));
    }
};

#endif // CP_IMPLICIT_SEGMENT_TREE_HPP