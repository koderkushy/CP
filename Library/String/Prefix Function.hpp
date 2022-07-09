#pragma once

// pi[i] = length of longest (prefix of a[0:i] == suffix of a[1:i+1])
vector<int> sprefix (const string &a) {
    const int n = a.size();
    vector pi(n, 0);
    for(int i = 1; i < n; i++){
        int j = pi[i-1];
        while(j > 0 and a[j] != a[i]) 
            j = pi[j-1];
        if(a[j] == a[i]) j++;
        pi[i] = j;
    }
    return pi;
}
