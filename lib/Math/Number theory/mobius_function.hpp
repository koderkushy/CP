#ifndef CP_MOBIUS_FUNCTION_HPP
#define CP_MOBIUS_FUNCTION_HPP

std::vector<int> mu;

auto mu_init (const int N = 1 << 20) {
    std::vector prime(0, 0);
    std::vector ok(N, false);

    mu.resize(N, 1);

    for (int i = 2; i < N; i++) {
        if (!ok[i])
            prime.push_back(i),
            mu[i] = -1;

        for (int p: prime) {
            if (i * p >= N)
                break;
            else if (i % p == 0)
                mu[i * p] = 0; 
            else 
                mu[i * p] = -mu[i];
            ok[i * p] = 1;
        }
    }
}

#endif // CP_MOBIUS_FUNCTION_HPP