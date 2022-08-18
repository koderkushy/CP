struct DSU {
    DSU (int n): z(n, -1) {}

    int head (const int x) {
        if (z[x] < 0) return x;
        return z[x] = head(z[x]);
    }

    int size (const int x) {
        return -z[head(x)];
    }

    bool same (const int x, const int y) {
        return head(x) == head(y);
    }

    bool unite (const int x, const int y) {
        const int& px = head(x), py = head(y);
        if(px == py) return false;
        if(z[px] > z[py])
            z[py] += z[px], z[px] = py;
        else
            z[px] += z[py], z[py] = px;
        return true;
    }

  private:
    std::vector<int> z;
    
};
