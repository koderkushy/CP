#ifndef CP_LI_CHAO_TREE_HPP
#define CP_LI_CHAO_TREE_HPP

template<
    class int64,
    class int32,
    int32 kL,
    int32 kR,
    bool kFlip = false
>
class LiChaoTree {
    static_assert(std::is_integral_v<int64> and std::is_integral_v<int32>);
public:
    static constexpr int64 INF = std::numeric_limits<int64>::max();

    class Line {
    public:
        int64 m, c;
        constexpr Line (const int64 m, const int64 c): m(m), c(c) {}
        constexpr Line () {}
        inline int64 operator() (int32 i) const {
            return m * i + c;
        }
    };

    static inline const Line max{0, INF};

    class Node {
    public:
        Line z;
        Node *l, *r;
        constexpr Node (const Line& x): z(x), l(nullptr), r(nullptr) {}
        constexpr Node () {}
    } *root = create(max);

    // query optimum at x = i
    int64 qu (int32 i) const { 
        int32 l = kL, r = kR;
        int64 z = INF;
        for (auto a = root; a; ) {
            z = std::min(z, a -> z(i));
            const int32 m = l + (r - l) / 2;
            if (i < m) r = m, a = a->l;
            else l = m, a = a->r;
        }
        if constexpr (kFlip) z = -z;
        return z;
    }

    // add line y = mx + c
    void up (const int64 m, const int64 c) {
        const Line x(m, c);
        up(x, root, kL, kR, x(kL), x(kR-1));
    }

    // add line y = mx + c on [ql, qr)
    void up (const int64 m, const int64 c, const int32 ql, const int32 qr) {
        const Line x(m, c);
        auto call = [&](auto&& call, Node* a, int32 l, int32 r) -> void {
            if (l >= qr or r <= ql) return;
            if (l >= ql and r <= qr) return (up (x, a, l, r, x(l), x(r-1)));       
            const int32 m = l + (r - l) / 2;
            if (ql < m){ if (!a->l) a->l = create(max); call(call, a->l, l, m); }
            if (qr > m){ if (!a->r) a->r = create(max); call(call, a->r, m, r); }
        };
        call(call, root, kL, kR);
    }

private:

     Node* create (Line x) {
        static constexpr size_t FIXED_SIZE = 2 * 2000 << 5;
        static Node memo[FIXED_SIZE];
        static int nx = 0;
        memo[nx++].z = x;
        return &memo[nx-1];
    }

    void up (Line x, Node* a, int32 l, int32 r, int64 x_l, int64 x_r) {
        if constexpr (kFlip)
            x.m = -x.m, x.c = -x.c;
        while(1) {
            const int64 z_l = a->z(l), z_r = a->z(r-1);

            if (x_l <= z_l and x_r <= z_r)
                return void(a->z = x);
            if (x_l >= z_l and x_r >= z_r)
                return;

            const int32 m = l + (r - l) / 2;
            int64 z_m = a->z(m-1), x_m = x(m-1);

            if (x_l > z_l) {
                if (x_m < z_m)
                    std::swap(x, a->z), r=m, x_l=z_l, x_r=z_m;
                else
                    l=m, x_l=x_m+x.m, x_r=x_r;
            } else {                
                if (x_m+x.m < z_m+a->z.m)
                    std::swap(x, a->z), l=m, x_l=z_m+a->z.m, x_r=z_r;
                else
                    r=m, x_l=x_l, x_r=x_m;
            }

            if(r == m) { if (!(a -> l)) a -> l = create(x); a = a -> l; }
            if(l == m) { if (!(a -> r)) a -> r = create(x); a = a -> r; }
        }
    }
};

#endif // CP_LI_CHAO_TREE_HPP