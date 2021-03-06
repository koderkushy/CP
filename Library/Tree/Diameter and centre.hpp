#pragma once

auto centre(vector<vector<int>>& g, const int st){
    int mx = 0, node = st;
    function<void(int,int,int)> dfs = [&](int u, int p, int d){
        if(d > mx) mx = d, node = u;
        for(int v: g[u]) if(p != v) dfs(v, u, d+1);
    };
    dfs(node, 0, 0); int e1 = node; mx = 0;
    dfs(node, 0, 0); int e2 = node;
    vector<int> path; set<int> cen;
    function<bool(int,int)> get = [&](int u, int p){
        path.push_back(u);
        if(e2 == u){
            cen.insert(path[mx/2]), cen.insert(path[mx - mx/2]);
            return 1;
        }
        for(int v: g[u]) if(p != v) if(get(v, u)) return 1;
        path.pop_back(); return 0;
    };
    get(e1, 0); return pair(mx, cen);
}
