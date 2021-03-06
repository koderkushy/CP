#pragma once

vector used(0, false);
vector subtree(0, 0);
vector g(0, vector(0, 0));

int subtree_DFS(int u, int p = -1){
    subtree[u] = 1;
    for(int v: g[u]) if(v != p && !used[v])
        subtree[u] += subtree_DFS(v, u);
    return subtree[u];
}
int find_centroid(int u, int n, int p = -1){
    for(int v: g[u]) if(v != p and !used[v] and subtree[v]*2 > n)
        return find_centroid(v, n, u);
    return u;
}
void decompose(int u){
    int cen = find_centroid(u, subtree_DFS(u));

    //do something
    

    used[cen] = 1;
    for(int v: g[cen]) if(!used[v])
        decompose(v);
}
