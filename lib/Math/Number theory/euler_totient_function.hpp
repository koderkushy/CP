#ifndef CP_EULER_TOTIENT_FUNCTION_HPP
#define CP_EULER_TOTIENT_FUNCTION_HPP

std::vector<int> phi;

auto phi_init (const int N = 1 << 20) {
    std::vector<int> prime;
    std::vector<bool> ok(N);
    phi.resize(N);

    phi[1] = 1;
    
    for (int i = 2; i < N; i++){
        if (!ok[i])
            prime.push_back(i),
            phi[i] = i-1;
        for (int p: prime) {
            if (i * p >= N) break;
            else if (i % p == 0)
                phi[i * p] = phi[i] * p;
            else 
                phi[i * p] = phi[i] * phi[p];
            ok[i * p] = 1;
        }
    }
}

#endif // CP_EULER_TOTIENT_FUNCTION_HPP