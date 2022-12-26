#ifndef CP_MODULAR_INTEGER_HPP
#define CP_MODULAR_INTEGER_HPP

template<uint32_t kMod>
class Mint {
public:

    using internal_value_type = int;
    static_assert(int(kMod) >= 1, "MOD is less than 1");
    static Mint raw (uint32_t v) { return *reinterpret_cast<Mint*>(&v); }
    static constexpr int getmod () { return kMod; }
    constexpr Mint (int64_t v) : val((v %= kMod) < 0 ? v + kMod : v) {}
    constexpr Mint () : val(0) {}
    template<class T> constexpr explicit operator T() const { return val; }
    constexpr int get () const { return val; };
    constexpr Mint& operator++ () { return val = ++val == kMod ? 0 : val, *this; }
    constexpr Mint& operator-- () { return --(val ? val : val = kMod), *this; }
    constexpr Mint operator+ () const { return *this; }
    constexpr Mint operator- () const { return raw(val ? kMod - val : 0); }
    constexpr Mint& operator+= (const Mint o) { return val = int(val += o.val - kMod) < 0 ? val + kMod : val, *this; }
    constexpr Mint& operator-= (const Mint o) { return val = int(val -= o.val) < 0 ? val + kMod : val, *this; }    
    constexpr Mint& operator*= (const Mint &o) { return val = (static_cast<uint64_t>(val) * o.val) % kMod, *this; }
    constexpr Mint& operator/= (const Mint &o) { return *this *= o.inv(); }
    constexpr friend Mint operator+ (Mint a, Mint b) { return a += b; }
    constexpr friend Mint operator- (Mint a, Mint b) { return a -= b; }
    constexpr friend Mint operator* (Mint a, Mint b) { return a *= b; }
    constexpr friend Mint operator/ (Mint a, Mint b) { return a /= b; }
    constexpr friend Mint operator++ (Mint& a, int) { return std::exchange(a, ++Mint(a)); }
    constexpr friend Mint operator-- (Mint& a, int) { return std::exchange(a, --Mint(a)); }
    constexpr friend bool operator== (Mint a, Mint b) { return a.val == b.val; }
    constexpr friend bool operator!= (Mint a, Mint b) { return a.val != b.val; }
    constexpr Mint inv () const { return extgcd(val, kMod); }
    constexpr Mint pow (uint64_t p) const {
        assert(p >= 0);
        Mint a = *this, res = 1;
        while (p > 0) {
            if (p & 1) res *= a;
            a *= a, p >>= 1;
        }
        return res;
    }
    constexpr friend std::istream& operator>>(std::istream& is, Mint& x) { int64_t v; return is >> v, x = v, is; }
    constexpr friend std::ostream& operator<<(std::ostream& os, Mint x) { return os << x.val; }

private:
    int val;

    constexpr static int extgcd (int a, int b) {
        std::array x{1, 0};
        int _x0 {}, _x1{}, _a{}, _b{};
        while (b)
            _x1 = x[0] - a / b * x[1], _x0 = x[1],
            _a = b, _b = a % b,
            a = _a, b = _b, x[0] = _x0, x[1] = _x1;
        return x[0];
    }
};

using mint = Mint<998244353>;


#endif // CP_MODULAR_INTEGER_HPP