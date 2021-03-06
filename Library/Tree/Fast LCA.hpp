#pragma once

struct fastlca{
    vector<vector<int>> g, sp; vector<int> h, eu, ini, L; const int N, rt;

    int mi(int x, int y){ return (h[x] < h[y])? x: y; }
    void dfs(int u, int p, int d){
        if(ini[u] == -1) ini[u] = eu.size(); h[u] = d++, eu.pb(u);
        for(int v: g[u]) if(v != p) dfs(v, u, d), eu.pb(u);    
    }
    void sparse(){
        int M = eu.size(); L = vector(M+1, 0);
        for(int i = 0, v = 0; i <= M; L[i] = v, i++, v += (i == 1 << (v + 1)));
        sp = vector(L[M]+2, vector(M, 0)); sp[0] = eu;
        for(int msk = 1, i = 1; msk <= M; msk <<= 1, i++) 
            for(int j = 0; j + msk < M; j++) 
                sp[i][j] = mi(sp[i-1][j], sp[i-1][j + msk]);
    }
    fastlca(const int sz, const int root = 1): N(sz), rt(root){
        g = vector(N+1, vector(0, 0)); h = ini = vector(N+1, -1);
        for(int x, y, i = 2; i <= N; i++){ 
            cin >> x >> y; g[y].pb(x), g[x].pb(y);
        }   dfs(rt, -1, 0), sparse();
    }
    int lca(int x, int y){
        if(x == 0) return y; if(y == 0) return x;
        int l = ini[x], r = ini[y]; if(l > r) swap(l, r); r++; int o = L[r-l]; 
        return mi(sp[o][l], sp[o][r-(1<<o)]);
    }
};
