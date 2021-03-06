#pragma once

vector<int> P;

void iprime(){
    bool ok[N]{0};
    ok[0] = ok[1] = 1;
    for(lol i = 2; i*i < N; i++){          // upto 1e5 -   9,592 primes
        if(ok[i]) continue;                // upto 1e6 -  78,498 primes
        for(lol j = i*i; j < N; j += i)    // upto 1e7 - 664,579 primes
            ok[j] = 1;
    }
    for(int i = 0; i < N; i++) 
        if(!ok[i]) P.push_back(i);
}

