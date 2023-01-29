#ifndef CP_CENTROID_DECOMPOSITION_HPP
#define CP_CENTROID_DECOMPOSITION_HPP

class CentroidTree {
public:

    CentroidTree (const std::vector<std::vector<int>>& initial_adj, const int root = 0):
    N(initial_adj.size()),
    adj(initial_adj),
    sub(N),
    vis(N),
    par(N)
    {
        decompose(root);
    }

    void decompose (const int root, const int p = -1) {
        auto [centroid, size] = get_centroid (root);
        
        // solve here. Don't visit vis[u] = 1 vertices

        


        vis[centroid] = true, par[centroid] = p;

        for (auto v: adj[centroid])
            if (!vis[v])
                decompose(v, centroid);
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

        const int size = dfs(dfs, root);
        int p = -1, c = root;

        loop:;
        for (auto v: adj[c])
            if (!vis[v] and v != p and sub[v] > size / 2) {
                p = c, c = v;
                goto loop;
            }

        return {c, size};
    }

    const int N;
    std::vector<std::vector<int>> adj;
    std::vector<int> sub, par;
    std::vector<bool> vis;
};

#endif // CP_CENTROID_DECOMPOSITION_HPP