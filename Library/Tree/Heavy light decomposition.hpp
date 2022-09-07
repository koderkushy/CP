template<int M = 20>
struct HeavyLight {
    HeavyLight (std::vector<std::vector<int>>& g, const int root = 0)
    : n(g.size()), root(root), adj(g), par(g.size()), in(g.size()), out(g.size()), head(g.size()), sub(g.size(), 1)
    {
        assert((1 << M) >= n);
        par[root][0] = root;
        dfs_sizes(root);
        dfs_hld(root);

        for (int j = 1; j < M; j++)
            for (int i = 0; i < n; i++)
                par[i][j] = par[par[i][j - 1]][j - 1];
    }

    // [in[head[u]], in[u]] -> ascending heavy chain
    std::vector<std::pair<int, int>> descend (const int p, int u) {
        assert(is_ancestor(p, u));
        std::vector<std::pair<int, int>> a;

        while (in[head[u]] > in[p]) {
            a.emplace_back(head[u], u),
            u = par[head[u]][0];
        }
        
        a.emplace_back(p, u);
        std::reverse(a.begin(), a.end());
        return a;
    }    

    int ancestor (int u, int jump = 1) {
        for (int j = 0; jump; jump >>= 1)
            if (jump & 1)
                u = par[u][j];
        return u;
    }

    inline bool is_ancestor (const int u, const int v) {
        return in[u] <= in[v] and out[u] >= out[v];
    }

    int lca (int u, int v) {
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;

        for (int j = M - 1; j > -1; --j)
            if (!is_ancestor(par[v][j], u))
                v = par[v][j];

        return par[v][0];
    } 

    const int n, root;
    std::vector<std::vector<int>> adj;
    std::vector<std::array<int, M>> par;
    std::vector<int> head, in, out, sub;

  private:
    void dfs_sizes (const int u, const int p = -1) {
        auto &a = adj[u];
        for (auto& v: a) if (v == p) {
            v = a.back(), a.pop_back(); break;
        }

        for (auto& v: a) {
            dfs_sizes(v, u),
            sub[u] += sub[v];
            if (sub[v] > sub[a[0]])
                std::swap(v, a[0]);
        }
    }

    void dfs_hld (const int u) {
        static int time = 0;
        in[u] = time++;
        for (auto v: adj[u])
            head[v] = (v == adj[u][0] ? head[u]: v),
            par[v][0] = u, dfs_hld(v);
        out[u] = time;
    }
};
