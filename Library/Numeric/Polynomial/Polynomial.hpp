#pragma once

// need NTT

template<typename mint, typename ntt>
struct polynomial {
    vector<mint> p;

    polynomial () {}
    polynomial (int n): p(n) {}
    polynomial (int n, mint X): p(n, X) {}
    polynomial (const vector<mint>& p_object): p(p_object) {}
    polynomial (const initializer_list<mint>& p_object): p(p_object) {}
    template<class iter>
    polynomial (iter begin, iter end): p(begin, end) {}

    size_t size() const{ return (int)p.size(); }
    polynomial& resize(int n, mint x = 0){ return p.resize(n, x), *this; }
    int degree() const{ return p.empty() ? numeric_limits<int>::max() : size() - 1; }
    mint &operator[](int i){ return p[i]; }
    const mint &operator[](int i) const{ return p[i]; }
    operator vector<mint>() const { return p; }
    typename vector<mint>::iterator begin(){ return p.begin(); }
    typename vector<mint>::iterator end(){ return p.end(); }
    mint evaluate(mint x) const {
        mint px = 0;
        for(auto i = p.size(); i; --i)
            px = px * x + p[i-1];
        return px;
    }

    polynomial& operator*=(const polynomial& f) {
        ntt::convolute(p, f.p);
        return *this;
    }
    polynomial operator*(const polynomial& f) const {
        return polynomial(*this) *= f;
    }
    template<typename U>
    polynomial& operator*=(const U x) {
        for(mint &v: p) v *= mint(x);
        return *this;
    }
    template<typename U>
    polynomial operator*(const U x) const {
        return polynomial(*this) *= x;
    }
    template<typename U>
    friend polynomial operator*(const U x, polynomial P) {
        for(auto &v: P) v *= mint(x);
        return P;
    }

    polynomial& operator/= (const polynomial& f) {
        if(p.size() < f.size())
            return p.clear(), *this;
        static polynomial b;
        const int n = p.degree() - f.degree() + 1;
        b.assign(n, mint());
        copy(f.rbegin(), f.rbegin() + min(f.size(), b.size()), b.begin());
        ntt::convolute(p, b.inverse(n), n);
        reverse(all(p));
        return *this;
    }
    polynomial operator/ (const polynomial& f) {
        return polynomial(*this) /= f;
    }
    polynomial &operator/=(const mint x){
        assert(x.val);
        mint inv_x = 1 / x;
        for(auto &c: p) c *= inv_x;
        return *this;
    }
    polynomial operator/(const mint x) const{
        return polynomial(*this) /= x;
    }
    pair<polynomial, polynomial> divrem(const polynomial &f) const{
        auto q = *this / f, r = *this - q * f;
        while(!r.empty() && r.back() == 0) r.pop_back();
        return {q, r};
    }
    polynomial operator%(const polynomial &f) const{
        return this->divrem(f).second;
    }
    polynomial &operator%=(const polynomial &f) const{
        return *this = *this % f;
    }

    polynomial &operator+=(const polynomial &f){
        p.resize(max(size(), f.size()));
        for(auto i = 0; i < (int)f.size(); ++i) p[i] += f[i];
        return *this;
    }
    polynomial operator+(const polynomial &f) const{
        return polynomial(*this) += f;
    }
    polynomial &operator+=(const mint &x){
        if(p.empty()) p.emplace_back();
        p[0] += x;
        return *this;
    }
    polynomial operator+(const mint &x) const{
        return polynomial(*this) += x;
    }
    friend polynomial operator+(const mint &x, const polynomial &f){
        return f + x;
    }
    polynomial &operator-=(const polynomial &f){
        p.resize(max(size(), f.size()));
        for(auto i = 0; i < (int)f.size(); ++i) p[i] -= f[i];
        return *this;
    }
    polynomial operator-(const polynomial &f) const{
        return polynomial(*this) -= f;
    }
    polynomial &operator-=(const mint &x){
        if(p.empty()) p.emplace_back();
        return p[0] -= x, *this;
    }
    polynomial operator-(const mint &x) const{
        return polynomial(*this) -= x;
    }
    friend polynomial operator-(const mint &x, const polynomial &f){
        return f - x;
    }
    polynomial operator-() const{
        polynomial res(*this);
        for(auto i = 0; i < size(); ++i) res[i] = mint() - res[i];
        return res;
    }


    polynomial inverse (size_t MAX) const {
        assert(p.size() and p[0].val);
        static vector<mint> a, b;
        b = {1 / p[0]};
        size_t n = 1;

        while(n < MAX) {
            a = vector<mint>(p.begin(), p.begin() + min(n << 1, p.size()));

            a.resize(n << 2), ntt::fft(a);
            b.resize(n << 2), ntt::fft(b);

            for(int i = 0; i < b.size(); i++)
                b[i] = b[i] * (2 - a[i] * b[i]);

            ntt::fft(b, 1), b.resize(n <<= 1);
        }

        b.resize(MAX), a.clear();
        return b;
    }
    polynomial& invert (size_t MAX) {
        return *this = this -> inverse(MAX);
    }

    polynomial& differentiate () {
        for(int i = 0; i < p.size() - 1; i++)
            p[i] = p[i+1] * (i+1);
        p.pop_back(); return *this;
    }
    polynomial derivative () const { return polynomial(*this).differentiate(); }

    polynomial& integrate () {
        p.emplace_back();
        for(int i = p.size() - 1; i; i--)
            p[i] = p[i-1] / i;
        p[0] = 0; return *this;
    }
    polynomial integral () const { return polynomial(*this).integrate(); }

    polynomial log (const size_t length) const {
        assert(p.size() and p[0] == 1);
        if(length == 0)
            return polynomial();
        return (inverse(length) * derivative()).resize(length - 1).integrate();
    }
    polynomial& take_log (const size_t length) { 
        return *this = this -> log(length);
    }

    polynomial exp (size_t length) const {
        assert(p.size() and p[0] == 0);
        static polynomial a, b;
        b = {1};
        size_t n = 1;

        while(n < length) {
            n <<= 1;
            a.p = vector<mint>(p.begin(), p.begin() + min(n, p.size()));
            b *= a + mint(1) - b.log(n), b.resize(n);
        }
        b.resize(length);
        return b;
    }
    polynomial& take_exp (size_t length) {
        return *this = this -> exp(length);
    }

    polynomial& take_pow (long long e, size_t length) {
        p.resize(length);
        if(e == 0 || length == 0){
            if(length) p[0] = 1;
            return *this;
        }
        if(e < 0) return invert(length).take_pow(-e, length);
        if(all_of(p.begin(), p.end(), [&](auto x){ return x == 0; })) return *this;

        size_t pivot = find_if(p.begin(), p.end(), [&](auto x){ return x; }) - p.begin();
        p.erase(p.begin(), p.begin() + pivot);
        length -= pivot * e;
        if(length <= 0) return fill(p.begin(), p.end(), 0), *this;

        mint norm = p[0];
        ((*this /= norm).take_log(length) *= e).take_exp(length);

        p.insert(p.begin(), pivot * e, mint());

        return *this *= norm[e];
    }
    polynomial pow (long long e, const size_t length) const {
        return polynomial(*this).take_pow(e, length);
    }
    // test: https://codeforces.com/problemset/problem/1342/E
    // EGF = (exp(x)-1)^k / k!
};
using poly = polynomial<mint, ntt>;
