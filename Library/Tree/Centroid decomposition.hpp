struct CentroidDecomposition {
    CentroidDecomposition (const std::vector<std::vector<int>>& g, const int root = 0)
    : adj(g), n(g.size()), root(root), sub(g.size()), vis(g.size())
    {
        decompose(root);

    }

    void decompose (const int root) {
        auto [centroid, size] = get_centroid (root);
        // solve here

        


        vis[centroid] = true;
        for (auto v: adj[centroid])
            if (!vis[v])
                decompose(v);
    }

  private:
    std::pair<int, int> get_centroid (const int root) {
        auto dfs = [&](auto&& dfs, const int u, const int p = -1) -> int {
            sub[u] = 1;
            for (auto v: adj[u])
                if (!vis[v] and v != p)
                    sub[u] += dfs(dfs, v, u);
            return sub[u];
        };

        const int& size = dfs(dfs, root);
        int c = root, p = -1;

        loop:;
        for (auto v: adj[c])
            if (!vis[v] and v != p and sub[v] > size / 2) {
                p = c, c = v;
                goto loop;
            }

        return {c, size};
    }

    std::vector<std::vector<int>> adj;
    std::vector<int> sub;
    std::vector<bool> vis;
    const int n, root;
};
