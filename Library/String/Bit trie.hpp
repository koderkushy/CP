template<int bits = 31>
struct BitTrie{
    static_assert(bits < 64);
    using uint = typename std::conditional<(bits > 31), uint64_t, uint32_t>::type;
    
    template<typename U>
    BitTrie (const std::vector<U>& a) {
        for (auto x: a) insert(uint(x));
    }
    BitTrie () {}
    
    void insert (const uint x) {
        for(int bit = bits - 1, u = 0; ~bit; -- bit)
            if (x >> bit & 1)
                u = ~r[u] ? r[u] : (r[u] = push()), c[u]++;
            else
                u = ~l[u] ? l[u] : (l[u] = push()), c[u]++;
    }

    void erase (const uint x) {
        for(int bit = bits - 1, u = 0; ~bit; -- bit)
            c[u = (x >> bit & 1) ? r[u]: l[u]] -= 1;
    }

    uint min_xor_with (const uint x) {
        uint z = 0;
        for(int bit = bits - 1, u = 0; ~bit; -- bit)
            if (x >> bit & 1)
                u = !~r[u] or !c[r[u]] ? (z |= ONE << bit, l[u]) : r[u];
            else
                u = !~l[u] or !c[l[u]] ? (z |= ONE << bit, r[u]) : l[u];
        return z;
    }

    uint max_xor_with (const uint x) {
        return (ALL ^ x) ^ x ^ min_xor_with(ALL ^ x);
    }

  private:
    static constexpr uint ONE = 1, ALL = (ONE << bits) - 1;
    std::vector<int> l{-1}, r{-1}, c{0};

    constexpr int push () {
        l.push_back(-1), r.push_back(-1), c.push_back(0);
        return l.size() - 1;
    }
};
