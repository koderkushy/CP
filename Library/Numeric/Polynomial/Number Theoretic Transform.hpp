#pragma once

// need mint

struct NTT_MODS { int MOD, rt, it, pw; } 
ntt_mods[5] = {
    { 7340033, 5, 4404020, 1 << 20 },
    { 415236097, 73362476, 247718523, 1 << 22 },
    { 463470593, 428228038, 182429, 1 << 21},
    { 998244353, 15311432, 469870224, 1 << 23 },
    { 918552577, 86995699, 324602258, 1 << 22 }
};

template<typename mint, int root = 15311432, int root_depth = 23>
struct NTT {
    static vector<mint> rx, ix;

    static void setup () {
        rx = vector(root_depth + 1, mint(root));
        ix = vector(root_depth + 1, 1/mint(root));
        for(int i = 0; i < root_depth; i++) 
            rx[i+1] = rx[i] * rx[i], ix[i+1] = ix[i] * ix[i];
    }

    static void fft (vector<mint> &a, bool invert = false) {
        const int& N = a.size();
        assert(__builtin_popcount(N) == 1);

        for(int i = 1, j = 0; i < N; i++){
            int b = N >> 1;
            while(b & j)
                j ^= b, b >>= 1;
            j ^= b;
            if(j > i)
                swap(a[i], a[j]);
        }

        mint X = 1, iN = mint(N).inv();
        const auto &x = invert? ix: rx;
 
        for(int l = 1, p = 22; l < N; l<<=1, p--)
            for(int s = 0; s < N; s += l+l, X = 1)
                for(int i = s; i < s+l; i++, X *= x[p])
                    a[i+l] = a[i] - X * a[i+l],
                    a[i] = a[i] * 2 - a[i+l];
 
        if(invert) for(auto &v: a) v *= iN;
    }
    
    static vector<mint> convolute_naive (const vector<mint>& a, const vector<mint>& b, size_t MAX = 0) {
        vector c(max((size_t)0, a.size()+b.size()-1), mint());
        if(MAX == 0) MAX = c.size();
        for(int i = 0; i < min(a.size(), MAX); i++)
            for(int j = 0; j < min(b.size(), MAX); j++)
                c[i+j] += a[i] * b[j];
        return c;
    }

    // In-place multiply modulo x^(MAX)
    static void convolute (vector<mint>& a, vector<mint> b, const size_t MAX = 0){
        if(min(a.size(), b.size()) < 55)
            return void(a = convolute_naive(a, b, MAX));
        
        if(MAX) a.resize(min(MAX, a.size())), b.resize(min(MAX, b.size()));
 
        const int n = (a.size() + b.size()); 
        int m = 1; while(m < n) m <<= 1;
 
        a.resize(m), fft(a);
        b.resize(m), fft(b);

        for(int i = 0; i < m; i++) a[i] *= b[i];

        fft(a, true), a.resize(n-1);
        if(MAX) a.resize(MAX);
    }
 
    // divide and conquer
    static void convolute (vector<vector<mint>>& a, const size_t MAX = 0){
        auto cmp = [&](int i, int j){
            return a[i].size() > a[j].size();
        };
        priority_queue<int, vector<int>, decltype(cmp)> p(cmp);

        for(int i = 0; i < a.size(); i++)
            p.push(i);
        while(p.size() > 1){
            int x = p.top(); p.pop();
            int y = p.top(); p.pop();
            convolute(a[x], a[y], MAX),
            p.push(x), a[y].clear();
        }
        a = {a[p.top()]};
    }
};
template<typename mint, int root, int root_depth> vector<mint> NTT<mint, root, root_depth>::rx;
template<typename mint, int root, int root_depth> vector<mint> NTT<mint, root, root_depth>::ix;
using ntt = NTT<mint, 15311432, 23>;

