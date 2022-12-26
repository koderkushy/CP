#ifndef CP_LEAST_PRIME_FACTOR_HPP
#define CP_LEAST_PRIME_FACTOR_HPP

std::vector<int> lpf, mul, primes;

auto lpf_init (const int N = 1 << 18) {
    lpf = mul = std::vector(N, 0);
    primes.reserve(N / std::max(1, static_cast<int>(log(N))));

    for (int i = 2; i < N; i++) {
        if (lpf[i] == 0)
            lpf[i] = i, mul[i] = 1,
            primes.push_back(i);

        for (int p: primes) {
            if (p > lpf[i] or p * i > N)
                break;
            lpf[p * i] = p;
            mul[p * i] = 1 + (p == lpf[i]? mul[i]: 0);
        }
    }
}

#endif // CP_LEAST_PRIME_FACTOR_HPP