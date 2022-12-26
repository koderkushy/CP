#ifndef CP_MANACHERS_HPP
#define CP_MANACHERS_HPP

auto manachers (const std::string& a) {
    const int& n = a.size();
    std::vector m(n, 1);                                                         // m[i] = max j such that a[i:j] = a[i:i-j:-1]

    for (int i = 0, l = 0, r = 1; i < n; i++) {
        if (i < r)
            m[i] = std::min(r - i, m[l + r - i - 1]);
        while (i + m[i] < n and i >= m[i] and a[i + m[i]] == a[i - m[i]])
            m[i]++;
        if (r < i + m[i])
            r = i + m[i], l = i - m[i] + 1;
    }

    return std::move(m);
}

#endif // CP_MANACHERS_HPP