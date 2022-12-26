#ifndef CP_HOPCROFT_KARP_MATCHING_HPP
#define CP_HOPCROFT_KARP_MATCHING_HPP

class HopcroftKarp {
public:

    using edge = std::pair<int, int>;

    HopcroftKarp (const int N, const int M, const std::vector<edge>& e)
    : N(N), M(M), adj(N), L_match(N, -1), R_match(M, -1), reach(N)
    {
        for (auto [l, r]: e)
            adj[l].push_back(r);

        max_matching();
    }

    int matched_count () const {
        return size;
    }

    std::vector<edge> &matched_edges () const {
        static auto edges = [&]() {
            std::vector<edge> e;
            for (int i = 0; i < N; i++) if (L_match[i] != -1)
                e.emplace_back(i, L_match[i]);

            return e;
        }();

        return edges;
    }

    std::vector<edge> &min_edge_cover () const {
        static auto cover = [&]() {
            auto cover = matched_edges();

            std::vector<std::vector<int>> Radj(M);

            for (int i = 0; i < N; i++) {
                for (auto v: adj[i])
                    Radj[v].push_back(i);
                if (L_match[i] == -1 and adj[i].size())
                    cover.emplace_back(i, adj[i][0]);
            }

            for (int i = 0; i < M; i++)
                if (R_match[i] == -1 and Radj[i].size())
                    cover.emplace_back(Radj[i][0], i);

            return cover;
        }();

        return cover;
    }
    
    std::array<std::vector<int>, 2> &min_vertex_cover () const {
        static auto cover = [&]() {
            std::array<std::vector<int>, 2> set;

            for (int i = 0; i < N; i++) {
                if (~L_match[i]) {
                    if (~reach[i]) set[1].push_back(L_match[i]);
                    else set[0].push_back(i);
                }
            }

            return set;
        }();

        return cover;
    }

    std::array<std::vector<int>, 2> &max_independent_set () const {
        static auto set = [&]() {
            const auto& vertex_cover = min_vertex_cover();
            std::vector<bool> l(N, true), r(M, true);

            for (int i: vertex_cover[0]) l[i] = false;
            for (int i: vertex_cover[1]) r[i] = false;

            std::array<std::vector<int>, 2> set;
            for (int i = 0; i < N; i++) if (l[i]) set[0].push_back(i);
            for (int i = 0; i < M; i++) if (r[i]) set[1].push_back(i);

            return set;
        }();

        return set;
    }

private:

    const int N, M;
    int size = 0;
    std::vector<std::vector<int>> adj;
    std::vector<int> L_match, R_match, reach;

    void max_matching () {                              // Maximum cardinality matching in O(|E|sqrt(|V|))
        for (bool updated = true; updated; ) {
            updated = false;

            static std::vector<int> root(N), queue(N);
            static int I, J;

            I = J = 0;
            std::fill(root.begin(), root.end(), -1),
            std::fill(reach.begin(), reach.end(), -1);

            for (int i = 0; i < N; i++)
                if (L_match[i] == -1)
                    queue[J++] = i, root[i] = i, reach[i] = i;

            while (I < J) {
                int u = queue[I++];
                if (L_match[root[u]] != -1) continue;

                for (int v: adj[u]) {
                    if (R_match[v] == -1) {
                        while (v != -1)
                            R_match[v] = u, std::swap(L_match[u], v), u = reach[u];
                        
                        updated = true;
                        size++;
                        break;
                    }

                    if (reach[R_match[v]] == -1)
                        v = R_match[v], reach[v] = u, root[v] = root[u], queue[J++] = v;
                }
            }
        }
    }
};

#endif // CP_HOPCROFT_KARP_MATCHING_HPP