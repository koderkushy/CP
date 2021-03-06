#pragma once

struct SuffixAutomaton {

    vector<int> len{0}, link{-1}, first{-1};
    vector<bool> isClone{0};
    vector<map<char, int>> next{{}};
    int last = 0;

    size_t size () const {
        return len.size();
    }

    int push(int l, int sl, int fp, bool c, const map<char, int> &adj){
        len.push_back(l);
        link.push_back(sl);
        first.push_back(fp);
        isClone.push_back(c);
        next.push_back(adj);
        return (int)len.size() - 1;
    }

    void extend (const string& s) {
        for(auto c: s) extend(c);
    }

    void extend (char c) {
        int cur = push(len[last] + 1, -1, len[last], false, {}), p = last;
        last = cur;
        while(~p and !next[p].count(c))
            next[p][c] = cur, p = link[p];

        if(p == -1) return void(link[cur] = 0);

        int q = next[p][c];
        if(len[q] == len[p] + 1) return void(link[cur] = q);

        int clone = push(len[p] + 1, link[q], first[q], true, next[q]);
        while(~p and next[p][c] == q)
            next[p][c] = clone, p = link[p];

        link[q] = link[cur] = clone;
    }

    SuffixAutomaton () {}
    SuffixAutomaton (const string& s) {
        extend(s);
    }

    int find (const string& s) {
        int u = 0;
        for(auto c: s)
            if(auto j = next[u].find(c); j == next[u].end()) return -1;
            else u = j -> ss;
        return u;
    }

    vector<int> counts () {
        int n = size();
        vector count(n, 0);
        vector inv(n, vector(0, 0));
        for(int i = 1; i < n; i++)
            inv[link[i]].push_back(i);

        auto dfs = [&](auto dfs, int u) -> void {
            count[u] = !isClone[u];
            for(auto v: inv[u])
                dfs(dfs, v), count[u] += count[v];
        };
        dfs(dfs, 0);
        return count;
    }
};
