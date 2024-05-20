#ifndef CP_LINE_HPP
#define CP_LINE_HPP

// needs point_2d

namespace geom {

template<typename T>
class line {
public:
    static constexpr T zero = 1e-8;
    point<T> p{}, d{};
    // x = p + d.t

    line () {}
    template<class U> line (const line<U> x): p(x.p), d(x.d) {}
    template<class U> line (const point<U> a, const point<U> b, const bool two_point = true): p(a), d(two_point ? b - a: b) {}
    line (const point<T> x): p(), d(x) {}
    line (const T x, const T y, const bool slope_intercept = true): p(0, y), d(slope_intercept ? point<T>(1, x) : point<T>(x, -y)) {}

    point<T> q() const { return p + d; }
    operator bool() const { return d.x > zero or d.y > zero; }
    bool operator|| (const line<T> l) const { return d || l.d; }
    
    line<double> translate_perp (T x) const { return {point<double>(p) + d.perp().unit() * x, d, false}; }
    line<long double> translate_perpl (T x) const { return {point<long double>(p) + d.perp().unitl() * x, d, false}; }
};

template<class T, class istream> istream &operator>> (istream &in, line<T> &l) { in >> l.p >> l.d, l.d -= l.p; return in; } // two point
template<class T, class ostream> ostream &operator<< (ostream &out, const line<T> &l) { return out << "{" << l.p << " -> " << l.q() << "}"; }

template<class T> bool on_line (const point<T> &p, const line<T> &L){ return L ? p - L.p || L.d : p == L.p; }
template<class T> bool on_ray (const point<T> &p, const line<T> &L){ return L ? (L.p - p || L.q() - p) && (L.p - p) * L.d <= 0 : p == L.p; }
template<class T> bool on_segment (const point<T> &p, const line<T> &L){ return L ? (L.p - p || L.q() - p) && (L.p - p) * (L.q() - p) <= 0 : p == L.p; }
template<class T> bool on_open_segment (const point<T> &p, const line<T> &L){ return L ? (L.p - p || L.q() - p) && (L.p - p) * (L.q() - p) < 0 : p == L.p; }
template<class T> double distance_to_line (const point<T> &p, const line<T> &L){ return L ? abs(p - L.p ^ L.d) / L.d.norm() : distance(p, L.p); }
template<class T> long double distance_to_linel (const point<T> &p, const line<T> &L){ return L ? abs(p - L.p ^ L.d) / L.d.norml() : distancel(p, L.p); }
template<class T> double distance_to_ray (const point<T> &p, const line<T> &L){ return (p - L.p) * L.d <= 0 ? distance(p, L.p) : distance_to_line(p, L); }
template<class T> long double distance_to_rayl (const point<T> &p, const line<T> &L){ return (p - L.p) * L.d <= 0 ? distancel(p, L.p) : distance_to_linel(p, L); }
template<class T> double distance_to_segment (const point<T> &p, const line<T> &L){ return (p - L.p) * L.d <= 0 ? distance(p, L.p) : (p - L.q()) * L.d >= 0 ? distance(p, L.q()) : distance_to_line(p, L); }
template<class T> long double distance_to_segmentl (const point<T> &p, const line<T> &L){ return (p - L.p) * L.d <= 0 ? distancel(p, L.p) : (p - L.q()) * L.d >= 0 ? distancel(p, L.q()) : distance_to_linel(p, L); }
template<class T> point<double> projection (const point<T> &p, const line<T> &L) { return point<double>(L.p) + (L ? (p - L.p) * L.d / L.d.squared_norm() * point<double>(L.d) : point<double>()); }
template<class T> point<long double> projectionl (const point<T> &p, const line<T> &L) { return point<long double>(L.p) + (L ? (p - L.p) * L.d / L.d.squared_norml() * point<long double>(L.d) : point<long double>()); }
template<class T> point<double> reflection (const point<T> &p, const line<T> &L) { return 2.0 * projection(p, L) - point<double>(p); }
template<class T> point<long double> reflectionl (const point<T> &p, const line<T> &L) { return 2.0l * projectionl(p, L) - point<long double>(p); }
template<class T> point<double> closest_point_on_segment (const point<T> &p, const line<T> &L) { return (p - L.p) * L.d <= 0 ? point<double>(L.p) : ((p - L.q()) * L.d >= 0 ? point<double>(L.q()) : projection(p, L)); }
template<class T> point<double> closest_point_on_segmentl (const point<T> &p, const line<T> &L) { return (p - L.p) * L.d <= 0 ? point<long double>(L.p) : ((p - L.q()) * L.d >= 0 ? point<long double>(L.q()) : projectionl(p, L)); }

} // namespace geom

#endif // CP_LINE_HPP