// need mint
struct NTT_MODS { int MOD, rt, it, pw; } 
ntt_mods[5] = {
    { 7340033, 5, 4404020, 1 << 20 },
    { 415236097, 73362476, 247718523, 1 << 22 },
    { 463470593, 428228038, 182429, 1 << 21},
    { 998244353, 15311432, 469870224, 1 << 23 },
    { 918552577, 86995699, 324602258, 1 << 22 }
};

template<int MOD = 998244353, int root = 15311432, int rinv = 469870224, int depth = 23>
struct NTT {
    using array = std::array<int, depth>;
    using mint = Mint<MOD>;

    template<int value>
    constexpr static array setup () {        
        array x{};
        x[0] = value;
        for (int i = 1; i < depth; i++)
            x[i] = x[i-1] * static_cast<int64_t>(x[i-1]) % MOD;
        return x;
    }

    static constexpr array rx = setup<root>();
    static constexpr array ix = setup<rinv>();

    template<bool invert = false>
    static void transform (std::vector<mint> &a) {
        const int& N = a.size();
        assert(__builtin_popcount(N) == 1 and __builtin_ctz(N) <= depth);

        for(int i = 1, j = 0; i < N; i++){
            int b = N >> 1;
            while(b & j)
                j ^= b, b >>= 1;
            j ^= b;
            if(j > i)
                std::swap(a[i], a[j]);
        }

        mint X = 1;
        const auto &x = invert? ix: rx;

        for (int len = 1, pow = depth - 1; len < N; len <<= 1, pow--)
            for (int s = 0; s < N; s += (len << 1), X = 1)
                for (int i = s; i < s + len; i++, X *= x[pow])
                    a[i] = a[i] + X * std::exchange(a[i + len], a[i] - X * a[i + len]);
 
        if constexpr (invert) for(auto &v: a) v *= mint(N).inv();
    }
    
    static std::vector<mint> convolute_naive (const std::vector<mint>& a, const std::vector<mint>& b, size_t MAX = 0) {
        std::vector<mint> c(std::max(static_cast<size_t>(0), a.size() + b.size() - 1));
        if(MAX == 0) MAX = c.size();
        for(int i = 0; i < std::min(a.size(), MAX); i++)
            for(int j = 0; j < std::min(b.size(), MAX); j++)
                c[i+j] += a[i] * b[j];
        return c;
    }

    static void convolute (std::vector<mint>& a, std::vector<mint> b, const size_t MAX = 0){
        if (std::min(a.size(), b.size()) < 55)
            return void(a = convolute_naive(a, b, MAX));
        
        if (MAX and MAX < a.size()) a.resize(MAX);
        if (MAX and MAX < b.size()) b.resize(MAX);

        const int n = (a.size() + b.size()); 
        int m = 1 << 32 - __builtin_clz(n - 1);
 
        a.resize(m), transform(a);
        b.resize(m), transform(b);
        for(int i = 0; i < m; i++)
            a[i] *= b[i];

        transform<true>(a), a.resize(n-1);
        if(MAX) a.resize(MAX);
    }
 
    static void convolute (std::vector<std::vector<mint>>& a, const size_t MAX = 0){
        static auto cmp = [&](int i, int j){
            return a[i].size() > a[j].size();
        };
        std::priority_queue<int, std::vector<int>, decltype(cmp)> p(cmp);
        for(int i = 0; i < a.size(); i++) p.push(i);

        while(p.size() > 1){
            int x = p.top(); p.pop();
            int y = p.top(); p.pop();
            convolute(a[x], a[y], MAX),
            p.push(x), a[y].clear();
        }
        a = {a[p.top()]};
    }
};
