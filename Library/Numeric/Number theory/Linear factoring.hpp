#pragma once

vector lpf(N, 0), mul(N, 0);

void ilpf(){
    lpf[1] = 1;
    for(lol i = 2; i < N; ++i){
        if(lpf[i]) continue;
        lpf[i] = i, mul[i] = 1;
        for(lol j = i*i; j < N; j += i) 
            if(!lpf[j]) 
                lpf[j] = i, mul[j] = 1+mul[j/i];
    }
}

map<int, vector<int>> memo;
vector<int> ftrz(int n){

    if(auto v = memo.find(n); v != memo.end())
        return v -> ss;

    vector<int> d[2];
    d[0].pb(n);

    for(int j = n, c = 0, l = lpf[j]; j > 1; c = !c, l = lpf[j]){
        for(int u: d[c]) do d[!c].pb(u); 
            while(u % l == 0 and (u /= l));
        
        while(j % l == 0) j /= l;
        d[c].clear();
    }

    if(d[0].size()) return memo[n] = d[0];
    else return memo[n] = d[1];
}
