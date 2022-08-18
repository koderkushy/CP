template<class uint>
struct DinicsMaxflow {

    DinicsMaxflow (int n, int s, int t)
    : n(n), source(s), sink(t), C(n, std::vector(n, zero)), adj(n), level(n), next(n) {
        assert(s < n and s > -1 and t < n and t > -1 and s != t);
    }

    void add (int u, int v, uint f) {
        assert(std::min(u, v) > -1 and std::max(u, v) < n);
        C[u][v] += f;
        adj[u].emplace_back(v), adj[v].emplace_back(u);
    }

    void reset () {
        for (auto& v: adj) v.clear();
        for (auto& v: C) v.assign(n, zero);
        state = 0;
    }

    uint max_flow () {
        static uint flow;

        if (state) return flow;
        else flow = 0;
        
        for (auto& v: adj)
            std::sort(v.begin(), v.end()),
            v.erase(std::unique(v.begin(), v.end()), v.end());

        while (relabel()) {
            std::fill(next.begin(), next.end(), 0);
            for (uint df; (df = augment(source, INF)) > zero; )
                flow += df;
        }

        state = 1;
        return flow;
    }

    std::vector<bool> min_cut () {
        max_flow();

        std::vector<bool> cut(n);
        for (int i = 0; i < n; i++)
            if (level[i] == -1)
                cut[i] = true;

        return cut;
    }

  private:

    static constexpr uint zero = static_cast<uint>(1e-9), INF = std::numeric_limits<uint>::max();

    std::vector<std::vector<uint>> C;
    std::vector<std::vector<int>> adj;
    std::vector<int> level, next;

    bool state = 0;
    const int n, source, sink;

    bool relabel () {
        std::fill(level.begin(), level.end(), -1);
        level[source] = 0;

        static std::vector<int> q(n);
        static int l, r;

        q[0] = source, l = 0, r = 1;

        while (l < r) {
            int u = q[l++];
            for (int v: adj[u]) if (C[u][v] > zero and level[v] == -1) {
                level[v] = level[u] + 1, q[r++] = v;
                if (v == sink)
                    return true;
            }
        }

        return false;
    }

    uint augment (const int u, const uint& block) {
        if (u == sink)
            return block;

        while (next[u] < adj[u].size()) {
            const int& v = adj[u][next[u]++];

            if (C[u][v] <= zero or level[u] + 1 != level[v])
                continue;

            if (auto flow = augment(v, std::min(block, C[u][v])); flow > zero) {
                C[u][v] -= flow, C[v][u] += flow;
                return flow;
            }
        }

        return zero;
    }
    
};
