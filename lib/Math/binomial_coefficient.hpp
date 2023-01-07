#ifndef CP_BINOMIAL_COEFFICIENTS_HPP
#define CP_BINOMIAL_COEFFICIENTS_HPP

// need mint

namespace comb {
    constexpr int N = 1 << 20;

    template<class mint>
    class Frl {
    public:
        static const std::vector<mint> v, inv;
    };

    template<class mint>
    const std::vector<mint> Frl<mint>::v = []() {
        std::vector<mint> a(N);
        a[0] = 1;

        for (int i = 1; i < N; ++i)
            a[i] = a[i - 1] * i;

        return a;
    }();

    template<class mint>
    const std::vector<mint> Frl<mint>::inv = []() {
        std::vector<mint> a(N);
        a[N - 1] = Frl<mint>::v[N - 1].inv();

        for (int i = N - 1; i; --i)
            a[i - 1] = a[i] * i;

        return a;
    }();

    template<class mint>
    auto Ncr (const int n, const int r) {
        return assert(n < N), (r >= 0 and r <= n ? Frl<mint>::v[n] * Frl<mint>::inv[r] * Frl<mint>::inv[n - r]: 0);
    }

    template<class mint>
    auto Npr (const int n, const int r) {
        return assert(n < N), (r >= 0 and r <= n ? Frl<mint>::v[n] * Frl<mint>::inv[n - r]: 0);
    }

} // namespace comb

using namespace comb;

const auto& frl = Frl<mint>::v;
const auto& irl = Frl<mint>::inv;
const auto& ncr = Ncr<mint>;
const auto& npr = Npr<mint>;


#endif // CP_BINOMIAL_COEFFICIENTS_HPP