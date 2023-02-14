/*
MIT License

Copyright (c) 2023 Niloy Dey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// See https://github.com/niloyroot/biconnected-graphgen for details about the project
// Thanks to ouuan for their Tree Generator (https://github.com/ouuan/Tree-Generator)

#include <bits/stdc++.h>
#include "testlib.h"
#include "treegenerator.h"
using namespace std;
using namespace tree_generator_by_ouuan;
 
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
 
// graph
vector<int> adj[100005];
int depth[100005];
int parent[100005];
vector<pair<int,int>> edges;
vector<array<int,3>> addtl_edges, queries;
map<pair<int,int>, bool> mp;
Tree T;
 
// lca
int tin[100005];
int tou[100005];
int up[100005][20];
int timer=0, lg;
 
// choosing nodes
int order[100005];
int start[100005];
int endd[100005];
int cnt=0;
 
// generating
bool covered[100005];
int ufds_sz[100005];
int ufds_par[100005];
int least_depth_edge[100005];
 
void dfs(int i, int pa) { 
    tin[i]=timer++;
    depth[i]=depth[pa]+1;
    parent[i]=pa;
    
    up[i][0]=pa;
    for(int j = 1; j <= lg; j++){
        up[i][j]=up[up[i][j-1]][j-1];
    }
 
    cnt++;
    order[cnt]=i;
    start[i]=cnt;
    
    for(auto u:adj[i]){
        if(u!=pa){
            dfs(u,i);
        }
    }
 
    endd[i]=cnt;
    
    tou[i]=timer++;
}
 
bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tou[u] >= tou[v];
}
 
int lca(int u, int v) {
    if (is_ancestor(u, v))
        return u;
    if (is_ancestor(v, u))
        return v;
    for (int i = lg; i >= 0; --i) {
        if (!is_ancestor(up[u][i], v))
            u = up[u][i];
    }
    return up[u][0];
}
 
int find(int x){
    if(x==ufds_par[x]) {
        return x;
    }
 
    return ufds_par[x] = find(ufds_par[x]);
}
 
void cover_path(int X, int lcaa) {
 
    X = find(X);
    covered[X] = true;
 
    while(depth[least_depth_edge[X]] > depth[lcaa]+1) {
        int a = X;
        a = find(a);
 
        int b = parent[least_depth_edge[a]];
        b = find(b);
 
        if (ufds_sz[a] < ufds_sz[b]) {swap(a,b);}
        ufds_par[b] = a;
        ufds_sz[a]+=ufds_sz[b];
 
        if(depth[least_depth_edge[a]] > depth[least_depth_edge[b]]) {
            least_depth_edge[a] = least_depth_edge[b];
        }
 
        X = a;
        covered[X] = true;
    }
}
 
int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);
 
    // constraints
    int N,M; string tree_type;
    cin >> N >> M >> tree_type;
    
    // initializing
    if(tree_type=="random") T.random(N-1,0);
    else if(tree_type=="silkworm") T.silkworm(N-1,0);
    else if(tree_type=="binary") T.binary(N-1,0);
    else if(tree_type=="tall") T.tall(N-1,10,0);
    else if(tree_type=="star") T.star(N-1,0);
    else if(tree_type=="completeBinary") T.completeBinary(N-1,0);
    else if(tree_type=="maxDegree")T.maxDegree(N-1,5,0);
    else if(tree_type=="chain") T.chain(N-1,0);
    else T.random(N-1,0);
    
    T.shuffleNodes(); T.shuffleEdges();
    lg = ceil(log2(N));
 
    stringstream x; x<<T;
    for(int i = 1; i <= N-1; i++){
        int a,b; x>>a>>b;
        adj[a].push_back(b);
        adj[b].push_back(a);
        edges.push_back({a,b});
        mp[{a,b}]=mp[{b,a}]=1;
    }
    dfs(1,1);
 
    for(int i = 1; i <= N; i++) {
        ufds_sz[i]=1;
        ufds_par[i]=i;
        least_depth_edge[i]=i;
    }
    
    // generating
    vector<int> perm;
    for(int i = 2; i <= N; i++){
        perm.push_back(i);
    }

    shuffle(perm.begin(), perm.end());
    
    for(int j = 0; j <= N-2; j++){
        int i = perm[j];
        int k = find(i);
        if(covered[k]) {continue;}
        
        int X = i; // node in the subtree of i
        int Y = parent[i]; // node outside subtree of i
        
        int cnt=0;
        while(mp[{X,Y}] || mp[{Y,X}]) {
            if(cnt==6) {cout<<"Failure"<<'\n'; return 0;}
            
            X = order[rnd.next(start[i], endd[i])];
            int index = rnd.next(1,N-(endd[i]-start[i]+1));
            if(index < start[i]){
                Y = order[index];
            } else {
                index = rnd.next(endd[i]+1,N);
                Y = order[index];
            }
            cnt++;
        }
        
        if(rnd.next(0,1) == 1) {
           swap(X,Y);
        }
        addtl_edges.push_back({X, Y, rnd.next(0,500000000)});
        
        mp[{X,Y}]=mp[{Y,X}]=1;
        
        int l = lca(X,Y);
        cover_path(X,l);
        cover_path(Y,l);
    }
    
    // additional random edges
    int cnt = addtl_edges.size()+N-1;
    while(cnt < M) {
        int X = rnd.next(1,N);
        int Y = rnd.next(1,N);
        while(X==Y || mp[{X,Y}] || mp[{Y,X}]) {
            X = rnd.next(1,N);
            Y = rnd.next(1,N);
        }
        mp[{X,Y}]=mp[{Y,X}]=1;
        addtl_edges.push_back({X, Y, rnd.next(0,500000000)});
        cnt++;
    }

    for(auto u:edges){
        addtl_edges.push_back({u.first, u.second, rnd.next(0,500000000)});
    }
    
    // output
    cout<<N<<" "<<M<<'\n';
    
    shuffle(addtl_edges.begin(), addtl_edges.end());
    for(auto u:addtl_edges){
        cout<<u[0]<<" "<<u[1]<<" "<<u[2]<<'\n';
    }
}
