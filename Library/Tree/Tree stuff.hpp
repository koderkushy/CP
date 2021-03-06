#pragma once

struct tree {
    vector<vector<int>> g;
    int n, root = 0;

    void neat (int u, int p) {
        auto &x = g[u];
        if(auto j = find(all(x), p); j != x.end()) x.erase(j);
        for(int& v: x) neat(v, u);
    }
    
    vector<int> dep;
    void depths () {
        dep.resize(n, 0);
        auto dfs = [&](auto&& dfs, int u) -> void {
            for(int v: g[u]) dep[v] = dep[u]+1, dfs(dfs, v);
        }; dfs(dfs, root);
    }
    
    vector<int> par;
    void parents () {
        par.resize(n, 0);
        auto dfs = [&](auto&& dfs, int u) -> void {
            for(auto v: g[u]) par[v] = u, dfs(dfs, v);
        }; dfs(dfs, root);
    }

    // LCA - up[i][x] is the (1<<i)th order ancestor of x
    vector<vector<int>> up;

    void lca_initialize () {
        if(par.empty()) parents();
        if(dep.empty()) depths();

        up = vector(int(ceil(log(n)/log(2))), par);

        for (int x = 1; x < up.size(); x++)
            for (int i = 0; i < n; i++)
                up[x][i] = up[x-1][up[x-1][i]];
    }
    
    int lcaof (int x, int y) {
        if(dep[x] < dep[y]) swap(x, y);
        for(int d = dep[x] - dep[y], i = 0; d; d >>= 1, i++)
            if(d & 1) x = up[i][x];
        for(int i = up.size() - 1; i >= 0; i--){
            if(up[i][x] != up[i][y])
                x = up[i][x], y = up[i][y];
        }
        return (x != y)? up[0][x]: x;
    }

    // Euler tour and HLD
    // [in[u], out[u]) - subtree, [in[nx[u]], in[u]] - ascending heavy path. adamant orz.
    vector<int> sz, in, out, nx;
    
    void tour_initialize () {
        in = out = nx = vector(n, 0), sz = vector(n, 1);

        auto sizes = [&](auto&& sizes, int u) -> int {
            for(auto& v: g[u]){
                sz[u] += sizes(sizes, v);
                if(sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
            }
            return sz[u];
        };
        sizes(sizes, root);

        int t = 0;
        auto tour = [&](auto&& tour, int u) -> void {
            in[u] = t++;
            for(int v: g[u])
                nx[v] = (v == g[u][0]? nx[u]: v),
                tour(tour, v);
            out[u] = t;
        };
        tour(tour, root);
    }

    vector<pii> chains (int u, int pu) {
        vector<pii> c;
        while(dep[u] >= dep[pu]) {
            int top = (dep[nx[u]] < dep[pu] ? pu: nx[u]);
            c.pb({top, u});
            if(u == root) break;
            u = par[top];
        }
        return c;
    }

    tree () { cin >> n, tree(n); }
    tree (const int n): n(n), g(n) {

        for(int i = 1; i < n; i++) {
            int u, v; cin >> u >> v;
            u--, v--;
            g[u].pb(v), g[v].pb(u);
        }

        neat(0, -1);
        lca_initialize();
        tour_initialize();

    }
};

