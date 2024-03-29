#ifndef CP_POINT_2D_HPP
#define CP_POINT_2D_HPP

namespace geom {

template<typename int_t>
class point {
public:
    static constexpr int_t zero = 1e-8;
    int_t x{}, y{};

    point () {}
    template<class T> point (const point<T> &a): x(a.x), y(a.y) {}
    template<class T, class U> point (const T X, const U Y): x(X), y(Y) {}
    template<class T> point (const std::array<T, 2> a): x(a[0]), y(a[1]) {}
    template<class T> operator std::array<T, 2>() const { return {x, y}; }

    auto operator* (const point &otr) const {
        if constexpr (std::is_integral<int_t>::value)
            return static_cast<int64_t>(x) * otr.x + static_cast<int64_t>(y) * otr.y;
        else
            return x * otr.x + y * otr.y;
    }

    auto operator^ (const point &otr) const {
        if constexpr (std::is_integral<int_t>::value)
            return static_cast<int64_t>(x) * otr.y - static_cast<int64_t>(y) * otr.x;
        else
            return x * otr.y - y * otr.x;
    }

    point operator+ (const point &otr) const { return {x + otr.x, y + otr.y}; }
    point operator- (const point &otr) const { return {x - otr.x, y - otr.y}; }
    point &operator+= (const point &otr) { return *this = *this + otr; }
    point &operator-= (const point &otr) { return *this = *this - otr; }
    point operator- () const { return {-x, -y}; }
    friend point operator+ (const int_t &c, const point &p) { return {c + p.x, c + p.y}; }
    friend point operator- (const int_t &c, const point &p) { return {c - p.x, c - p.y}; }
    friend point operator* (const int_t &c, const point &p) { return {c * p.x, c * p.y}; }
    friend point operator/ (const int_t &c, const point &p) { return {c / p.x, c / p.y}; }
    point operator+ (const int_t &c) const { return {x + c, y + c}; }
    point operator- (const int_t &c) const { return {x - c, y - c}; }
    auto operator* (const int_t &c) const {
        if constexpr (std::is_integral<int_t>::value)
            return point<int64_t>(x * static_cast<int64_t>(c), y * static_cast<int64_t>(c));
        else
            return point<int_t>(x * c, y * c);
    }

    point operator/ (const int_t &c) const { return {x / c, y / c}; }
    point &operator+= (const int_t &c) { return *this = *this + c; }
    point &operator-= (const int_t &c) { return *this = *this - c; }
    point &operator*= (const int_t &c) { return *this = *this * c; }
    point &operator/= (const int_t &c) { return *this = *this / c; }
    bool operator< (const point &otr) const { return x != otr.x ? x < otr.x : y < otr.y; }
    bool operator> (const point &otr) const { return x != otr.x ? x > otr.x : y > otr.y; }
    bool operator<= (const point &otr) const { return ! (*this > otr); }
    bool operator>= (const point &otr) const { return ! (*this < otr); }
    bool operator== (const point &otr) const {
        if constexpr (std::is_integral<int_t>::value)
            return x == otr.x and y == otr.y;
        else
            return abs(x - otr.x) < zero and abs(y - otr.y) < zero;
    }
    
    bool operator!= (const point &otr) const { return ! (*this == otr); }

    double norm () const { return hypot(x, y); }
    long double norml () const { return hypotl(x, y); }
    auto squared_norm () const { return (*this) * (*this); }

    double arg () const { return atan2(y, x); }
    long double argl () const { return atan2l(y, x); }

    point<double> unit () const { return point<double>(x, y) / norm(); }
    point<long double> unitl () const { return point<long double>(x, y) / norml(); }

    point perp () const { return {-y, x}; }
    point<double> rotate (const double theta) const { return {x * cos(theta) - y * sin(theta), x * sin(theta) + y * cos(theta)}; }
    point<long double> rotatel (const double theta) const { return {x * cosl(theta) - y * sinl(theta), x * sinl(theta) + y * cosl(theta)}; }

    point reflect_x () const { return {x, -y}; }
    point reflect_y () const { return {-x, y}; }
    point reflect (const point &o = {}) const { return {2 * o.x - x, 2 * o.y - y}; }

    bool operator|| (const point &otr) const { return abs(*this ^ otr) <= zero; }
};

using pointl = point<int>;
using pointll = point<int64_t>;
using pointd = point<double>;
using pointld = point<long double>;

template<class T, class istream> istream &operator>> (istream &in, point<T> &p) { return in >> p.x >> p.y; }
template<class T, class ostream> ostream &operator<< (ostream &out, const point<T> &p) { return out << "(" << p.x << ", " << p.y << ")"; }

template<class T> double distance (const point<T> &p, const point<T> &q){ return (p - q).norm(); }
template<class T> long double distancel (const point<T> &p, const point<T> &q){ return (p - q).norml(); }
template<class T> auto squared_distance (const point<T> &p, const point<T> &q){ return (p - q).squared_norm(); }

template<class T> auto area_twice_signed (const point<T> a, const point<T> b, const point<T> c) { return (b - a) ^ (c - b); }
template<class T> auto area_twice_signed (const std::vector<point<T>> &a) {
    auto area = a.size() < 2 ? 0 : a.back() ^ a.front();
    for (int i = 1; i < a.size(); i++)
        area += a[i - 1] ^ a[i];
    return area;
}
template<class T> auto area_twice (const point<T> a, const point<T> b, const point<T> c) { return abs(area_twice_signed(a, b, c)); }
template<class T> double area_signed (const point<T> a, const point<T> b, const point<T> c) { return 0.5 * area_twice_signed(a, b, c); }
template<class T> double area (const point<T> a, const point<T> b, const point<T> c) { return abs(area_signed(a, b, c)); }

template<class T> auto area_twice (const std::vector<point<T>> &a) { return abs(area_twice_signed(a)); }
template<class T> double area_signed (const std::vector<point<T>> &a) { return 0.5 * area_twice_signed(a); }
template<class T> double area (const std::vector<point<T>> &a) { return abs(area_signed(a)); }

template<class T> double angle_signed (const point<T> a, const point<T> b) { return atan2(a ^ b, a * b); }
template<class T> double angle (const point<T> a, const point<T> b) { return abs(angle_signed(a, b)); }
template<class T> long double anglel_signed (const point<T> a, const point<T> b) { return atan2l(a ^ b, a * b); }
template<class T> long double anglel (const point<T> a, const point<T> b) { return abs(anglel_signed(a, b)); }

template<class T> bool is_sorted_by_angle (const point<T> origin, const point<T> a, const point<T> b, const point<T> c) {
    T x = (a - origin) ^ (b - origin);
    T y = (b - origin) ^ (c - origin);
    if (x >= 0 and y >= 0) return true;
    if (x < 0 and y < 0) return false;
    return ((a - origin) ^ (c - origin)) < 0;
}
template<class T> bool is_sorted_by_angle (const point<T> origin, const std::vector<point<T>> &a) {
    for (int i = 0; i < a.size() - 2; i++)
        if (!is_sorted_by_angle(origin, a[i], a[i + 1], a[i + 2]))
            return false;
    return true;
}

template<class T> bool colinear (const point<T> a, const point<T> b, const point<T> c) { return area_twice_signed(a, b, c) <= point<T>::zero; }
template<class T> bool colinear (const std::vector<point<T>> &a) {
    int i = 1;
    while (i < a.size() and a[0] == a[i])
        i++;
    if (i == a.size())
        return true;
    for (int j = i + 1; j < a.size(); j++)
        if (!colinear(a[0], a[i], a[j]))
            return false;
    return true;
}

template<class T> point<T> centroid_thrice (const point<T> a, const point<T> b, const point<T> c) { return a + b + c; }
template<class T> auto __centroid (const std::vector<point<T>> &a) {
    assert(!a.empty());
    auto del = a.back() ^ a.front(), area = del;
    auto c = (a.back() + a.front()) * del;

    for (int i = 1; i < a.size(); i++)
        del = a[i - 1] ^ a[i],
        area += del,
        c += (a[i - 1] + a[i]) * del;

    return std::pair(c, area);
}

template<class T> pointd centroid (const point<T> a, const point<T> b, const point<T> c) { return pointd(centroid_thrice(a, b, c)) / 3; }
template<class T> pointld centroidl (const point<T> a, const point<T> b, const point<T> c) { return pointld(centroid_thrice(a, b, c)) / 3; }
template<class T> pointd centroid (const std::vector<point<T>> &a) { auto [c, area_twice_signed] = __centroid(a); return pointd(c) / static_cast<double>(3 * area_twice_signed); }
template<class T> pointld centroidl (const std::vector<point<T>> &a) { auto [c, area_twice_signed] = __centroid(a); return pointld(c) / static_cast<long double>(3 * area_twice_signed); }

} // namespace goem

#endif // CP_POINT_2D_HPP