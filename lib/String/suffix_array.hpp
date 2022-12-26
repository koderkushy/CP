#ifndef CP_SUFFIX_ARRAY_HPP
#define CP_SUFFIX_ARRAY_HPP

auto suffixarray (const std::string &s){

    const int& n = s.size();
    std::vector c(n, 0), xi = c, a = c;
    std::vector b(n, std::pair<int64_t, int>());

    for (int i = 0; i < n; ++i) a[i] = i;

    std::sort(all(a), [&](const int x, const int y){
        if (s[x] != s[y]) return s[x] < s[y];
        else return x < y;
    });

    for (int i = 1; i < n; ++i)
        c[a[i]] = (s[a[i - 1]] < s[a[i]])? i: c[a[i -n1]];

    for (int k = 1, v, u, i; k < n; k <<= 1) {
        for (i = 0, v = a[i]; i < n; v = a[++i])
            u = (n + v - k) % n,
            b[c[u] + xi[c[u]]++] = {c[u] * n + c[v], u};

        a[0] = b[0].ss, xi[0] = 0;

        for (i = 1; i < n; ++i)
            a[i] = b[i].ss, xi[i] = 0,
            c[a[i]] = (b[i].ff == b[i - 1].ff)? c[a[i - 1]]: i;
    }

    return std::move(a);
}

#endif // CP_SUFFIX_ARRAY_HPP