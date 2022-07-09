#pragma once

// z[i] = length of largest common prefix of a[0:n], a[i:n]
vector<int> zfun (const string &a) {
    const int n = a.size();
    vector z(n, 0);
    for(int i = 1, l = 0, r = 1; i < n; ++i){
        if(i < r) 
            z[i] = min(r-i, z[i-l]);
        while(i + z[i] < n and a[z[i]] == a[i + z[i]])
            z[i]++;
        if(i + z[i] > r)
            l = i, r = i + z[i];
    }
    return z;
}
