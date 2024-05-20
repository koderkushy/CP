#ifndef CP_TRIE_HPP
#define CP_TRIE_HPP

template<class string_type>
class Trie {
public:

    using char__ =  typename std::remove_const<
                        typename std::remove_reference<
                            decltype(std::declval<string_type>().front())
                        >::type
                    >::type;

    using node = std::map<char__, int>;

    Trie () {}
    Trie (const std::vector<string_type>& a) {
        for (auto s: a) insert(s);
    }

    void insert (const string_type& s) {
        int u = 0;
        for (auto x: s) {
            if (a[u].count(x))
                u = a[u][x];
            else
                u = a[u][x] = push();
            c[u]++;
        }
    }

    bool exists (const string_type& s) {
        return count(s);
    }

    int count (const string_type& s) {
        int u = 0;
        for (auto& x: s) {
            if (a[u].count(x))
                u = a[u][x];
            else
                return 0;
        }
        return c[u];
    }

private:

    std::vector<node> a{{}};
    std::vector<int> c{0};

    int push () {
        a.emplace_back();
        c.emplace_back();
        return a.size() - 1;
    }
};

#endif // CP_TRIE_HPP