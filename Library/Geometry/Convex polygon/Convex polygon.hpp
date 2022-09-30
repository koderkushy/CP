namespace geom {
    template<class T, bool STRICT = true>
    struct ConvexPolygon {
        std::vector<point<T>> hull;

        ConvexPolygon (std::vector<point<T>> s, const bool is_sorted = false)
        {
            if (!is_sorted)
                std::sort(s.begin(), s.end()),
                s.erase(std::unique(s.begin(), s.end()), s.end());

            Graham_scan(s);
        }

        void Graham_scan (const vector<point<T>>& s) {
            std::vector<point<T>> over;
            hull.reserve(s.size()), over.reserve(s.size());

            for (auto p: s) {
                if constexpr (STRICT) {
                    while (hull.size() > 1 and area_twice_signed(*(++hull.rbegin()), p, hull.back()) >= 0) hull.pop_back();
                    while (over.size() > 1 and area_twice_signed(*(++over.rbegin()), p, over.back()) <= 0) over.pop_back();
                } else {
                    while (hull.size() > 1 and area_twice_signed(*(++hull.rbegin()), p, hull.back()) > 0) hull.pop_back();
                    while (over.size() > 1 and area_twice_signed(*(++over.rbegin()), p, over.back()) < 0) over.pop_back();
                }
                hull.push_back(p), over.push_back(p);
            }

            if (over.size() > 2)
                hull.insert(hull.end(), ++over.rbegin(), --over.rend());
        }

        size_t size () const { return hull.size(); }
        bool empty () const { return hull.empty(); }
        const point<T>& operator[] (const int i) const { return hull[i]; }
        const point<T>& front () const { return hull.front(); }
        const point<T>& back () const { return hull.back(); }

        std::vector<point<T>> linearize () const {
            if (hull.empty())
                return {};

            auto a = hull;
            const auto p = std::max_element(a.begin(), a.end());
            std::reverse(p, a.end());
            std::inplace_merge(a.begin(), p, a.end());
            return a;
        }

        std::vector<point<T>> boundary () const {
            if (size() < 1)
                return {};

            auto a = hull;
            a.push_back(a[0]);
            for (int i = size(); i; --i)
                a[i] -= a[i - 1];
            return a;
        }

        auto separate () {
            const auto b = hull.begin(), e = hull.end();
            auto j = max_element(b, e);

            std::vector<point<T>> dn(b, j + 1), up(j, e);

            if (up.back().x != b -> x) up.push_back(*b);
            if (j > b and (j - 1) -> x == j -> x) dn.pop_back();

            reverse(up.begin(), up.end());
            return std::pair(up, dn);
        }

        // Set union
        ConvexPolygon& operator|= (const ConvexPolygon& c) {
            auto&& s = linearize();
            auto&& s_c = c.linearize();
            s.insert(s.end(), s_c.begin(), s_c.end());
            std::inplace_merge(s.begin(), s.begin() + size(), s.end());
            hull.clear();
            Graham_scan(s);
            return *this;
        }

        // Minkowski sum
        ConvexPolygon& operator+= (const ConvexPolygon& c) {
            const int n = size(), m = c.size();

            if (m == 0) return *this;
            if (m == 1) return *this += c[0];
            if (n == 0) return *this = c;
            if (n == 1) {
                auto p = hull[0];
                return (*this = c) += p;
            }

            auto bx = boundary(), by = c.boundary();

            hull.clear(), hull.reserve(n + m);
            hull.push_back(bx[0] + by[0]);

            auto i = bx.begin() + 1, j = by.begin() + 1;
            while (i != bx.end() and j != by.end()) {
                auto cross = *i ^ *j;
                if (cross == 0) hull.push_back(*i++ + *j++);
                else if (cross > 0) hull.push_back(*i++);
                else hull.push_back(*j++);
            }
            hull.insert(hull.end(), i, bx.end());
            hull.insert(hull.end(), j, by.end());

            for (int i = 1; i < hull.size(); i++)
                hull[i] += hull[i - 1];

            hull.pop_back();
            return *this;
        }

        ConvexPolygon& operator+= (const point<T>& p) {
            for (auto& x: hull) x += p;
            return *this;
        }
        ConvexPolygon& operator-= (const point<T>& p) { return *this += -p; }
        ConvexPolygon operator| (const ConvexPolygon& c) { return ConvexPolygon(*this) |= c; }
        ConvexPolygon operator+ (const point<T>& p) { return ConvexPolygon(*this) += p; }
        ConvexPolygon operator+ (const ConvexPolygon& c) { return ConvexPolygon(*this) += c; }
        ConvexPolygon operator- (const point<T>& p) { return ConvexPolygon(*this) += -p; }
        friend ConvexPolygon operator+ (const point<T>& p, const ConvexPolygon& c) { return c + p; }
        friend ConvexPolygon operator- (const point<T>& p, const ConvexPolygon& c) { return c - p; }
    };
}
