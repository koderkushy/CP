// Needs Segtree (N = 2^K)
template <class Node, class Comb>
struct SegtreeWalk : public Segtree<Node, Comb> {

    using ST = Segtree<Node, Comb>;

    template<typename... T>
    explicit SegtreeWalk (const int& n, const Node& id, T... args)
        : ST (n, id, args...) {}
    explicit SegtreeWalk (const vector<Node>& x, const Node& id, const Comb& com)
        : ST (x, id, com) {}

    const int& N = ST::N;
    const Node& identity = ST::identity;
    vector<Node>& a = ST::a;
    const Comb& com = ST::com;

    // First j in [l, N] such that f(com[l, j)) is FALSE
    int max_right(int l, const function<bool(Node)> f){
        assert(l > -1 and l <= N and f(identity)); 
        if(l == N) return l;
        Node prev = identity, t = identity;
        l += N;
        do{
            l >>= __builtin_ctz(l);
            if(!f(com(prev, a[l]))){
                while(l < N) if(f(t = com(prev, a[l <<= 1])))
                    prev = t, l++;
                return l - N;
            }
            prev = com(prev, a[l++]);
        } while((l & -l) != l);
        return N;
    }

    // First j in [0, r] such that f(com[j, r)) is TRUE
    int min_left(int r, const function<bool(Node)> f){
        assert(r > -1 and r <= N and f(identity));
        if(r == 0) return r;
        Node last = identity, t = identity;
        r += N;
        do{ 
            r--, r >>= __builtin_ctz(~r);
            if(r == 0) r = 1;
            if(!f(com(a[r], last))){
                while(r < N) if(f(t = com(a[(r <<= 1) += 1], last)))
                    last = t, r--;
                return r + 1 - N;
            }
            last = com(a[r], last);
          } while((r & -r) != r);
        return 0;
    }
};
