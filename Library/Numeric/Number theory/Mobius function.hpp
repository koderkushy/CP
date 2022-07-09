#pragma once

vector<int> mu(N);

void mobius(){
    vector<int> prime;
    bool ok[N]{0}; mu[1] = 1;
    for(int i = 2; i < N; i++){
        if(!ok[i])
            prime.push_back(i), mu[i] = -1;
        for(int p: prime){
            if(i*p >= N) break;
            else if(i % p == 0){
                mu[i*p] = 0; 
            }else 
                mu[i*p] = -mu[i];
            ok[i*p] = 1;
        }
    }
}
