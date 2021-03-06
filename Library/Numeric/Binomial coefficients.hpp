#pragma once

// need mint
struct comb {
    static vector<mint> frl, irl;
    static void setup (const int N = 1 << 18) {
        vector<mint> frl(N, 1), irl(N, 1);
        for(int i = 1; i < N; i++)
            frl[i]=frl[i-1]*i, irl[i]=irl[i-1]/i;
    }
    static mint ncr (int n, int r) { return (r > n or r < 0)? 0: frl[n]*irl[r]*irl[n-r]; }
    static mint npr (int n, int r) { return (r > n or r < 0)? 0: frl[n]*irl[n-r]; }
};
vector<mint> comb::frl, comb::irl;
constexpr auto &ncr = comb::ncr, &npr = comb::npr;
constexpr auto &frl = comb::frl, &irl = comb::irl;
