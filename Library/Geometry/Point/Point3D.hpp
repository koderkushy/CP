namespace geom {
    template<class int_t>
    struct point3{
        static constexpr int_t zero = 1e-8;
        int_t x{}, y{}, z{};

        point3(){ }
        template<class U> point3(const point3<U> &otr): x(otr.x), y(otr.y), z(otr.z){ }
        template<class U, class V, class W> point3(U x, V y, W z): x(x), y(y), z(z){ }
        template<class U> point3(const std::array<U, 3> &p): x(p[0]), y(p[1]), z(p[2]){ }
        template<class U> operator std::array<U, 3>() const { return {x, y, z}; }
        int_t operator* (const point3 &otr) const { return x * otr.x + y * otr.y + z * otr.z; }
        point3 operator^ (const point3 &otr) const { return {y * otr.z - z * otr.y, z * otr.x - x * otr.z, x * otr.y - y * otr.x}; }
        point3 operator+ (const point3 &otr) const { return {x + otr.x, y + otr.y, z + otr.z}; }
        point3 operator- (const point3 &otr) const { return {x - otr.x, y - otr.y, z - otr.z}; }
        point3 &operator+= (const point3 &otr) { return *this = *this + otr; }
        point3 &operator-= (const point3 &otr) { return *this = *this - otr; }
        point3 operator- () const { return {-x, -y, -z}; }
        friend point3 operator+ (const int_t &c, const point3 &p) { return {c + p.x, c + p.y, c + p.z}; }
        friend point3 operator- (const int_t &c, const point3 &p) { return {c - p.x, c - p.y, c - p.z}; }
        friend point3 operator* (const int_t &c, const point3 &p) { return {c * p.x, c * p.y, c * p.z}; }
        friend point3 operator/ (const int_t &c, const point3 &p) { return {c / p.x, c / p.y, c / p.z}; }
        point3 operator+ (const int_t &c) const { return {x + c, y + c, z + c}; }
        point3 operator- (const int_t &c) const { return {x - c, y - c, z - c}; }
        point3 operator* (const int_t &c) const { return {x * c, y * c, z * c}; }
        point3 operator/ (const int_t &c) const { return {x / c, y / c, z / c}; }
        point3 &operator+= (const int_t &c) { return *this = *this +=  c; }
        point3 &operator-= (const int_t &c) { return *this = *this -=  c; }
        point3 &operator*= (const int_t &c) { return *this = *this *=  c; }
        point3 &operator/= (const int_t &c) { return *this = *this /=  c; }
        bool operator> (const point3 &otr) const { return std::tuple(x, y, z) > std::tuple(otr.x, otr.y, otr.z); }
        bool operator< (const point3 &otr) const { return std::tuple(x, y, z) < std::tuple(otr.x, otr.y, otr.z); }
        bool operator>= (const point3 &otr) const { return std::tuple(x, y, z) >= std::tuple(otr.x, otr.y, otr.z); }
        bool operator<= (const point3 &otr) const { return std::tuple(x, y, z) <= std::tuple(otr.x, otr.y, otr.z); }
        bool operator== (const point3 &otr) const { return std::tuple(x, y, z) == std::tuple(otr.x, otr.y, otr.z); }
        bool operator!= (const point3 &otr) const { return std::tuple(x, y, z) != std::tuple(otr.x, otr.y, otr.z); }

        double norm () const { return sqrt(x * x + y * y + z * z); }
        double norml () const { return sqrtl(x * x + y * y + z * z); }
        int_t sq_norm () const { return x * x + y * y + z * z; }

        point3<double> unit () const { return point3<double>(x, y, z) / norm(); }
        point3<long double> unitl () const { return point3<long double>(x, y, z) / norml(); }

        point3 reflect_x () const { return {x, -y, -z}; }
        point3 reflect_y () const { return {-x, y, -z}; }
        point3 reflect_z () const { return {-x, -y, z}; }
        point3 reflect_xy () const { return {x, y, -z}; }
        point3 reflect_yz () const { return {-x, y, z}; }
        point3 reflect_zx () const { return {x, -y, z}; }
        point3 reflect (const point3 o = {}) const { return {2 * o.x - x, 2 * o.y - y, 2 * o.z - z}; }

        bool operator|| (const point3 &otr) const { auto d = *this ^ otr; return abs(d.x) <= zero && abs(d.y) <= zero && abs(d.z) <= zero; }
    };

    using point3l = point3<int>;
    using point3ll = point3<int64_t>;
    using point3d = point3<double>;
    using point3ld = point3<long double>;

    template<class T, class istream> istream &operator>> (istream &in, point3<T> p) { return in >> p.x >> p.y >> p.z; }
    template<class T, class ostream> ostream &operator<< (ostream &out, const point3<T> p) { return out << "{" << p.x << ", " << p.y << ", " << p.z << "}"; }
    template<class T> double angle (const point3<T> p, const point3<T> q) { return atan2((p ^ q).norm(), p * q); }
    template<class T> long double anglel (const point3<T> p, const point3<T> q) { return atan2l((p ^ q).norml(), p * q); }

    template<class T> double area (const point3<T> a, const point3<T> b, const point3<T> c) { return ((b - a) ^ (c - b)).norm() / 2; }
}
