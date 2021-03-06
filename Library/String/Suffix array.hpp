#pragma once

vector<int> suff(const string &s){

    const lol n = s.size();
    vector c(n, 0), xi = c, a = c;
    vector b(n, (pair<lol, int>){0ll, 0});

    for(int i = 0; i < n; ++i)
        a[i] = i;
    sort(all(a), [&](int x, int y){
        if(s[x] != s[y]) return s[x] < s[y];
        else return x < y;
    });

    for(int i = 1; i < n; ++i)
        c[a[i]] = (s[a[i-1]] < s[a[i]])? i: c[a[i-1]];

    for(int k = 1, v, u, i; k < n; k<<=1){
        for(i = 0, v = a[i]; i < n; v = a[++i])
            u = (n+v-k)%n,
            b[c[u]+xi[c[u]]++] = {c[u]*n + c[v], u};
        a[0] = b[0].ss, xi[0] = 0;
        for(i = 1; i < n; ++i)
            a[i] = b[i].ss, xi[i] = 0,
            c[a[i]] = (b[i].ff == b[i-1].ff)? c[a[i-1]]: i;
    }

    return a;
}
