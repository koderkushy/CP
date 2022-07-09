#pragma once

vector<int> topsort(const vector<vector<int>> &g, int st){
    const int n = g.size();
    vector c(n, 0), top(0, 0);
    function<bool(int)> dfs = [&](int u){
        if(c[u] == 1) return 0;
        if(c[u] == 2) return 1;
        c[u] = 1;
        for(int v: g[u]){
            if(!dfs(v)) return 0;
        }
        c[u] = 2, top.pb(u);
        return 1;
    };
    for(int i = st; i < n; i++){
        if(c[i] == 0 and !dfs(i))
            return vector(0, 0);
    }
    reverse(all(top));
    return top;
}
