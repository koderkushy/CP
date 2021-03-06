// Usage: https://judge.yosupo.jp/submission/95195
struct DSU{
    int *z;
    DSU(int n): z(new int [n]) { std::fill(z, z+n, -1); }
    int head(const int x) { return z[x] < 0 ? x: (z[x] = head(z[x])); }
    int size(const int x) { return z[x] < 0 ? -z[x]: (z[x] = head(z[x])); }
    bool same(const int x, const int y) { return head(x) == head(y); }
    bool unite(int x, int y){
        const int& px = head(x), py = head(y);
        if(px == py) return false;
        if(z[px] > z[py]) z[py] += z[px], z[px] = py;
        else z[px] += z[py], z[py] = px;
        return true;
    }
};
