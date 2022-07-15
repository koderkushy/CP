namespace LIS {
    template<typename T, bool STRICT = false, bool FLIP = false>
    int length (const std::vector<T>& a) {

        const int& n = a.size();
        std::vector<uint32_t> dp(n, -1);
        int len = 0;

        for (auto x: a) {
            if constexpr (FLIP) x = -x;

            int j = std::partition_point(dp.begin(), dp.begin() + len, [&](const int top) {
                if constexpr (STRICT) return top < x; else return top <= x;
            }) - dp.begin();

            if (j == len) dp[j] = x, len++;
            else if (dp[j] > x) dp[j] = x;
        }

        return len;
    }
}