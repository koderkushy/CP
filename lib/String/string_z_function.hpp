#ifndef CP_Z_FUNCTION_HPP
#define CP_Z_FUNCTION_HPP

// z[i] = longest common prefix of a[0:n], a[i:n]

template<class vec_type>
auto z_function (const vec_type& a) {
    const int& n = a.size();
    std::vector z(n, 0);

    for (int i = 1, l = 0, r = 1; i < n; ++i){
        if (i < r) 
            z[i] = std::min(r - i, z[i - l]);
        while (i + z[i] < n and a[z[i]] == a[i + z[i]])
            z[i]++;
        if (i + z[i] > r)
            l = i, r = i + z[i];
    }
    
    return std::move(z);
}

#endif // CP_Z_FUNCTION_HPP