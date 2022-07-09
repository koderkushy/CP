#pragma once

struct hstring{
    const lol hmd, b;
    vector<lol> bp, h;
    const int n;

    hstring(const string &a): n(a.size()), hmd(1e9 + rng()%47), b(256 + rng()%(hmd - 256)){
        bp = h = vector<lol>(n+1); 
        bp[0] = 1;

        foin{
            h[i+1] = h[i] + bp[i]*(int)a[i];
            bp[i+1] = b*bp[i], 
            bp[i+1] %= hmd, h[i+1] %= hmd;
        }
    }

    lol hv(int l, int r){
        lol v = (h[r] - h[l])*bp[n-l];
        v %= hmd;
        if(v < 0) v += hmd;
        return v;
    }
};
