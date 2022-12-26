#ifndef CP_TRIE_HPP
#define CP_TRIE_HPP

// untested
template<int M = 26>
class Trie {
public:

    // Hard code the alphabet
    static constexpr std::array<char, M> alphabet = []() {
        std::array<char, M> a{};
        for (int i = 0; i < M; i++) a[i] = 'a' + i;
        return a;
    }();

    static constexpr std::array<int, 256> ord = []() {
        std::array<int, 256> a{}; int o = 0;
        for (int i = 0; i < M; i++) a[alphabet[i]] = o++;
        return a;
    }();

    using node = std::array<int, M>;

    std::vector<node> a{{}};
    std::vector<int> c{0};

    constexpr int push () {
        a.emplace_back({}); return a.size() - 1;
    }

    Trie () {}
    Trie (const std::vector<string>& a) {
        for (auto s: a) insert(s);
    }

    void insert (const string& s) {
        int u = 0;
        for (char& x: s) {
            u = (a[u][ord[x]] ? a[u][ord[x]] : a[u][ord[x]] = push());
            c[u]++;
        }
    }

    bool exists (const string& s) {
        return count(s);
    }

    int count (const string& s) {
        int u = 0;
        for (char& x: s) {
            u = a[u][ord[x]];
            if (!u or !c[u])
                return 0;
        }
        return c[u];
    }
};

#endif // CP_TRIE_HPP