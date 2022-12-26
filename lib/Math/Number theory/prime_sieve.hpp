#ifndef CP_PRIME_SIEVE_HPP
#define CP_PRIME_SIEVE_HPP

std::vector<int> primes;

auto primes_init (const int N = 1 << 20) {
    // there are 9'592 primes upto 1e5
    //          78'498 primes upto 1e6
    //         664'579 primes upto 1e7

    std::vector ok(N, false);
    ok[0] = ok[1] = 1;

    for (uint64_t i = 2; i * i < N; i++)
        if (!ok[i])
            for (uint64_t j = i * i; j < N; j += i)
                ok[j] = 1;

    for (int i = 0; i < N; i++)
        if (!ok[i])
            primes.push_back(i);
}

#endif // CP_PRIME_SIEVE_HPP