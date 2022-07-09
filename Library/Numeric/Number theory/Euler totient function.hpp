#pragma once

vector<int> phi(N);

void totient(){
    vector<int> prime;
    bool ok[N]{0}; phi[1] = 1;
    for(int i = 2; i < N; i++){
        if(!ok[i])
            prime.push_back(i), phi[i] = i-1;
        for(int p: prime){
            if(i*p >= N) break;
            else if(i % p == 0)
                phi[i*p] = phi[i]*p;
            else 
                phi[i*p] = phi[i]*phi[p];
            ok[i*p] = 1;
        }
    }
}
