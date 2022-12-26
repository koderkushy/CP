#ifndef CP_PREFIX_FUNCTION_HPP
#define CP_PREFIX_FUNCTION_HPP

// p[i] = longest common prefix of a[0:n] and suffix of a[0:i]

template<class vec_type>
auto kmp_function (const vec_type& a) {
    const int& n = a.size();
    std::vector p(n, 0);

    for (int i = 1; i < n; i++) {
        int j = p[i - 1];
        while (j > 0 and a[j] != a[i]) 
            j = p[j - 1];
        if (a[j] == a[i])
            j++;
        p[i] = j;
    }

    return std::move(p);
}

#endif // CP_PREFIX_FUNCTION_HPP