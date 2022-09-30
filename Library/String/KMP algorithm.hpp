template<class vec_type>
std::vector<int> kmp_function (const vec_type& a) {
    const int& n = a.size();
    std::vector p(n, 0);                    // p[i] = longest common prefix of a[0:n] and suffix of a[0:i]

    for (int i = 1; i < n; i++) {
        int j = p[i - 1];
        while (j > 0 and a[j] != a[i]) 
            j = p[j - 1];
        if(a[j] == a[i])
            j++;
        p[i] = j;
    }

    return p;
}
