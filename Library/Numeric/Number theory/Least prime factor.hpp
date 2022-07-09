#pragma once

vector<int> lpf, mul, primes;

void lpf_linear(const int N = 1<<18){
    lpf = mul = vector(N, 0);
    for (int i = 2; i < N; i++) {
        if (lpf[i] == 0)
            lpf[i] = i, mul[i] = 1, primes.pb(i);
        for (int p: primes) {
            if(p > lpf[i] or p * i > N) break;
            lpf[p * i] = p, mul[p * i] = 1 + (p == lpf[i]? mul[i]: 0);
        }
    }
}
