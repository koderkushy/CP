#ifndef CP_HEAVY_LIGHT_DECOMPOSITION_HPP
#define CP_HEAVY_LIGHT_DECOMPOSITION_HPP

template<class Node, class Calc>
class HLD {
public:

    explicit HLD (const std::vector<std::vector<int>>& initial_adj,
                  const int root,
                  const std::vector<Node>& initial_a,
                  const Node& id,
                  const Calc& F):
    N(initial_adj.size()),
    root(root),
    id(id),
    F(F),
    adj(initial_adj),
    par(N, std::vector(1, root)),
    in(N),
    head(N, root),
    sub(N, 1),
    depth(N),
    log_max_depth((dfs_size(root), dfs_hld(root), 32 - __builtin_clz(max(*std::max_element(depth.begin(), depth.end()) - 1, 0)))),
    fwd_st(N << 1, id),
    bwd_st(N << 1, id)
    {
        for (auto& v: par) v.resize(log_max_depth + 1);
        for (int j = 0; j < log_max_depth; ++j)
            for (int i = 0; i < N; ++i)
                par[i][j + 1] = par[par[i][j]][j];

        for (int i = 0; i < N; ++i)
            fwd_st[in[i] + N] = bwd_st[in[i] + N] = initial_a[i];
        for (int i = N; --i; )
            fwd_st[i] = F(fwd_st[i << 1], fwd_st[i << 1 | 1]),
            bwd_st[i] = F(bwd_st[i << 1 | 1], bwd_st[i << 1]);
    }

    inline bool is_ancestor (const int u, const int v) const {
        return in[u] <= in[v] and in[u] + sub[u] > in[v];
    }

    int lca (int u, int v) const {
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;

        for (int j = log_max_depth; j > -1; --j) {
            if (!is_ancestor(par[v][j], u))
                v = par[v][j];
        }

        return par[v][0];
    }

    void set (int i, const Node& x) {
        for (i = in[i] + N, fwd_st[i] = bwd_st[i] = x; i >>= 1; )
            fwd_st[i] = F(fwd_st[i << 1], fwd_st[i << 1 | 1]),
            bwd_st[i] = F(bwd_st[i << 1 | 1], bwd_st[i << 1]);
    }

    Node qu_path (int u, int v) const {
        assert(0 <= std::min(u, v) and std::max(u, v) < N);
        
        const int l = lca(u, v);

        Node zl = id, zr = id;

        while (depth[head[u]] > depth[l])
            zl = F(zl, bwd_st_qu(in[head[u]], in[u] + 1)),
            u = par[head[u]][0];

        if (u != l) zl = F(zl, bwd_st_qu(in[l] + 1, in[u] + 1));

        while (depth[head[v]] > depth[l])
            zr = F(fwd_st_qu(in[head[v]], in[v] + 1), zr),
            v = par[head[v]][0];

        if (depth[v] >= depth[l])
            zr = F(fwd_st_qu(in[l], in[v] + 1), zr);

        return F(zl, zr);
    }

    Node qu_subtree (const int u) const {
        return fwd_st_qu(in[u], in[u] + sub[u]);
    }

private:

    const int N, root;
    const Node id;
    const Calc F;
    std::vector<std::vector<int>> adj, par;
    std::vector<int> in, head, sub, depth;
    const int log_max_depth;
    std::vector<Node> fwd_st, bwd_st;

    void dfs_size (const int u, const int p = -1) {
        if (auto j = std::find(adj[u].begin(), adj[u].end(), p); j != adj[u].end())
            adj[u].erase(j);

        for (auto& v: adj[u]) {
            depth[v] = depth[u] + 1, dfs_size(v, u), sub[u] += sub[v];
            if (sub[v] > sub[adj[u][0]])
                std::swap(v, adj[u][0]);
        }
    }

    void dfs_hld (const int u) {
        static int time = 0;
        in[u] = time++;
        
        for (auto v: adj[u])
            head[v] = (v == adj[u][0] ? head[u]: v),
            par[v][0] = u, dfs_hld(v);
    }

    Node fwd_st_qu (int l, int r) const {
        Node zl = id, zr = id;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l & 1) zl = F(zl, fwd_st[l++]);
            if (r & 1) zr = F(fwd_st[--r], zr);
        }
        return F(zl, zr);
    }

    Node bwd_st_qu (int l, int r) const {
        Node zl = id, zr = id;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l & 1) zl = F(bwd_st[l++], zl);
            if (r & 1) zr = F(zr, bwd_st[--r]);
        }
        return F(zr, zl);
    }
};

#endif // CP_HEAVY_LIGHT_DECOMPOSITION_HPP