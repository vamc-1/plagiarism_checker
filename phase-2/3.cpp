#include <bits/stdc++.h>
using namespace std;
#define ll long long 
#define vi vector<int>
#define vll vector<ll>
#define pyes cout << "YES" << endl;
#define pno cout << "NO" << endl;
#define f(i, a, b) for(ll i = a; i < b; i++)
#define fe(i, a, b) for(ll i = a; i <= b; i++)
#define fr(i, a, b) for(ll i = a; i >= b; i--)
#define pb push_back
#define MOD 1000000007
#ifdef LOCAL
#include "D:/debug.h"
#else
#define debug(...) 42
#endif
int bfs(vector<int> a){
    int n=a.size();
    int g=a[0];
    f(i,1,n)g=gcd(g,a[i]);
    f(i,0,n) a[i]/=g;
    bool k=true;
    for(int x:a) {
        if(x!=1){k=false;break;}
    }
    if(k)return 0;
    int o=0;
    for(int x:a)if(x==1)o++;
    if(o>0)return n-o;
    set<int> vis;
    queue<pair<int,int>> q;
    for(int x:a){
        q.push({x,0});
        vis.insert(x);
    }
    while(!q.empty()){
        pair<int,int>p=q.front();q.pop();
        if(p.first==1)return p.second+n-1;
        for(int x:a){
            int t=gcd(p.first,x);
            if(!vis.count(t)){
                vis.insert(t);
                q.push({t,p.second+1});
            }
        }
    }
    return -1;
}

void solve() {
    int n;cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;++i)cin>>a[i];
    cout<<bfs(a)<<"\n";
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin>>t;
    while (t--){   
        solve();
    }  

}




