#ifndef CP_INTERVAL_SET_HPP
#define CP_INTERVAL_SET_HPP

template<class T, bool kDisjoint = false>
class IntervalSet {
    static_assert(std::is_integral_v<T>);
    using interval = std::pair<T, T>;

public:

    template<typename... Args>
    IntervalSet (Args&&... args)
    : a(args...) {}

    auto size () const {
        return a.size();
    }

    auto insert (const interval x) {
        a.insert(x);
    }

    auto insert (const T l, const T r) {
        assert(l < r), a.insert(interval(l, r));
    }

    auto find (const interval x) {
        return a.find(x);
    }

    auto find (const T i) {
        auto j = a.upper_bound(interval(i, std::numeric_limits<T>::max()));
        if (j == a.begin())
            return a.end();
        j--;
        if (j -> first > i or j -> second <= i)
            return a.end();
        return j;
    }

    auto erase (const typename std::set<interval>::iterator j) {
        a.erase(j);
    }

    auto split (const T i) {
        auto j = find(i);
        if (j == a.end())
            return false;

        auto [l, r] = *j;
        a.erase(j);

        if (i > l) a.insert(interval(l, i));
        if (i < r) a.insert(interval(i, r));

        return true;
    }

    auto split_excluding (const T i) {
        auto j = find(i);
        if (j == a.end())
            return false;

        auto [l, r] = *j;
        a.erase(j);

        if (i > l) a.insert(interval(l, i));
        if (i + 1 < r) a.insert(interval(i + 1, r));

        return true;
    }

    auto unite () {
        std::vector<interval> u;

        for (auto f = a.begin(); f != a.end(); ) {
            auto [l, r] = *f;
            for (; f != a.end() and f -> first <= r; )
                r = std::max(r, f -> second), f++;
            u.push_back(interval(l, r));
        }

        a = std::set(u.begin(), u.end());
    }

    auto unite_overlapping () {
        std::vector<interval> u;

        for (auto f = a.begin(); f != a.end(); ) {
            auto [l, r] = *f;
            for (; f != a.end() and f -> first < r; )
                r = max(r, f -> second), f++;
            u.insert(interval(l, r));
        }

        a = set(u.begin(), u.end());
    }

    auto unite (const typename std::set<interval>::iterator j1, const typename std::set<interval>::iterator j2) {
        auto& [l1, r1] = *j1;
        auto& [l2, r2] = *j2;

        if (l1 > l2)
            std::swap(l1, l2), std::swap(r1, r2);

        if (r1 < l2)
            return false;
        else
            a.erase(j1),
            a.erase(j2),
            a.insert(interval(l1, std::max(r1, r2)));
    }

    std::set<interval> a;
};


#endif // CP_INTERVAL_SET_HPP