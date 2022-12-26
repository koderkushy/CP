#ifndef CP_FACTORING_HPP
#define CP_FACTORING_HPP

// needs lpf

std::map<int, std::vector<int>> memo;

auto ftrz (const int n) {

    if (auto v = memo.find(n); v != memo.end())
        return v -> ss;

    std::vector<int> d[2];
    d[0].push_back(n);

    for (int j = n, c = 0, l = lpf[j]; j > 1; c = !c, l = lpf[j]) {
        for (int u: d[c])
            do
                d[!c].push_back(u);
            while (u % l == 0 and u /= l);
        
        while (j % l == 0)
            j /= l;

        d[c].clear();
    }

    if (d[0].size())
        return memo[n] = d[0];
    else
        return memo[n] = d[1];
}

#endif CP_FACTORING_HPP