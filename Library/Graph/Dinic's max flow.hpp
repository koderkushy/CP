#pragma once

struct MaxFlow {
    int64_t n, s, t, flow = 0;
    vector<vector<int>> G;
    vector<vector<int64_t>> C;
    vector<int> nx, level;
 
    MaxFlow (int _n, int _s, int _t): n(_n), s(_s), t(_t){
        C = vector(n+1, vector(n+1, int64_t())), 
        G = vector(n+1, vector(0, 0));
    }
 
    void add (int u, int v, int f) {
        G[u].pb(v), G[v].pb(u), C[u][v] += f;
    }
 
    void Dinic () {
        for (auto &g: G)
            sort(all(g)), g.resize(unique(all(g)) - g.begin());
 
        auto bfs = [&]() {
            level = vector(n+1, 0);
            queue<int> q;
            q.push(s), level[s] = 1;

            while (q.size()) {
                int u = q.front(); q.pop();
                for (int v: G[u]) if (C[u][v] > 0 and level[v] == 0)
                    level[v] = 1 + level[u], q.push(v);
            }

            return level[t];
        };
    
        auto dfs = [&](auto&& dfs, const int u, const int64_t block) -> int64_t {
            if (u == t)
                return block;

            while (nx[u] < G[u].size()) {
                const int& v = G[u][nx[u]];
                int64_t f;

                nx[u]++;

                if (C[u][v] > 0 and level[v] == level[u] + 1 and (f = dfs(dfs, v, min (block, C[u][v]))) > 0)
                    return C[u][v] -= f, C[v][u] += f, f;
            }
            
            return 0;
        };

        while (bfs()) {
            int64_t D; nx = vector(n+1, 0);
            do  flow += (D = dfs (dfs, s, 1ll << 62));
                while (D);
        }
    }
};

