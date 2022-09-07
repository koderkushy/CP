template<int M = 20>
struct BinaryLifting {
    BinaryLifting (std::vector<std::vector<int>>& g, const int root = 0)
    : n(g.size()), root(root), adj(g), par(g.size()), in(g.size()), out(g.size())
    {
        assert((1 << M) >= n);
        par[root][0] = root, dfs(root);

        for (int j = 1; j < M; j++)
            for (int i = 0; i < n; i++)
                par[i][j] = par[par[i][j - 1]][j - 1];
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

  private:
    std::vector<std::vector<int>> adj;
    std::vector<int> in, out;
    std::vector<std::array<int, M>> par;
    const int n, root;

    void dfs (int u, int p = -1) {
        static int time = 0;
        in[u] = time++;
        for (auto v: adj[u])
            if (v != p)
                par[v][0] = u, dfs(v, u);
        out[u] = time;
    }
};
