//#define PBDS
//#define POINT
//#define ROPE
//#define XTRASIZE
//#define CUSTOM_NEW

#ifndef BZ
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("no-stack-protector")
//#pragma GCC target ("tune=native")
//#pragma GCC target ("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx")
//#pragma GCC optimize ("conserve-stack")
//#pragma GCC optimize ("no-stack-limit")
//#pragma clang optimize on
//#pragma clang loop unroll(enable)
#endif

#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using str = string;
template <typename T>
using vec = vector<T>;
using pll = pair<ll, ll>;
template <class T>
using heap_min = priority_queue<T, vector<T>, greater<T>>;
template <class T>
using heap_max = priority_queue<T, vector<T>, less<T>>;

#ifdef POINT
using c_type = ll;
using point = complex<c_type>;
#define X real
#define Y imag
istream& operator>>(istream& cin, point& p) {
  c_type x, y;
  cin >> x >> y;
  p.X(x);
  p.Y(y);
  return cin;
}
#endif

#ifdef CUSTOM_NEW
char buf_[10000000];
size_t buf_ptr_ = 0;
void* operator new(size_t count) {
  auto ans = (buf_ + buf_ptr_);
  buf_ptr_ += count;
  return ans;
}

// void operator delete(void* ptr, size_t n) noexcept {}
void operator delete(void*) noexcept {}

void* operator new[](size_t count) {
  auto ans = (buf_ + buf_ptr_);
  buf_ptr_ += count;
  return ans;
}

void operator delete[](void*) noexcept {}
#endif

#ifdef XTRASIZE
using xld = __float128;
using xll = __int128;
ostream& operator<<(ostream& cout, xll v) {
  string ans;
  if (v == 0)
    ans = "0";
  bool less = v < 0;
  if (less)
    v *= -1;
  while (v) {
    ans.push_back('0' + v % 10);
    v /= 10;
  }
  if (less)
    ans.push_back('-');
  reverse(ans.begin(), ans.end());
  cout << ans;
  return cout;
}

istream& operator>>(istream& cin, xll& v) {
  string s;
  cin >> s;
  reverse(s.begin(), s.end());
  bool less = s.back() == '-';
  if (less)
    s.pop_back();
  v = 0;
  while (!s.empty()) {
    v *= 10;
    v += s.back() - '0';
  }
  if (less)
    v *= -1;
  return cin;
}
#endif

#ifdef ROPE
#include <ext/rope>
using namespace __gnu_cxx;
#endif

#ifdef PBDS
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
//using namespace __gnu_pbds::detail;
template <class T, class V = null_type>
using indexed_tree =
    tree<T, V, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
#endif

constexpr ll INF = 0x7fffffffffffffff;
constexpr ll BASE = 1000000007;
// 3 * 10**9 + 19
// 998244353: rev = pow(n, BASE - 2) - root=15311432 - root_pw = 1LL << 23

template <class T>
constexpr T max() {
  return std::numeric_limits<T>::max();
}

template <class T>
constexpr T min() {
  return std::numeric_limits<T>::min();
}

#ifdef Debug
#define LOG(v) cout << #v << " " << v << "\n";
#else
#define LOG(v) ;
#endif

#define pb push_back
#define eb emplace_back
#define all(c) c.begin(), c.end()
#define rall(c) c.rbegin(), c.rend()
#define F first
#define S second
#define forEach(c) for (auto& it : c)
#define loop while (true)
#define mp make_pair
#define mt make_tuple

template <class T>
void chMax(T& a, T b) {
  if (a < b)
    a = b;
}

template <class T>
void chMin(T& a, T b) {
  if (b < a)
    a = b;
}

inline ll divUp(ll a, ll b) {
  return (a + b - 1) / b;
}

[[noreturn]] void panic() {
  cout << "-1\n";
  exit(0);
}

#include <algorithm>
#include <ranges>

struct Spirit {
  Spirit() {}
  int gangs_count{1};
  unordered_set<int>* cur{nullptr};
};

void solve() {
  int n, m;
  cin>>n>>m;
  vec<Spirit> spirits;
  vec<unordered_set<int>> gangs;
  gangs.reserve(n+1);
  spirits.resize(n+1);
  for (int i = 0; i < n+1; ++i) {
    unordered_set<int> tmp;
    tmp.insert(i);
    gangs.push_back(std::move(tmp));
    spirits[i].cur = &gangs.back();
  }
  for (int i = 0;i <m; ++i) {
    int q;
    cin>>q;
    if (q == 1) {
      int id1, id2;
      cin>>id1>>id2;
      for (auto spirit_id : *spirits[id1].cur) {
        spirits[spirit_id].gangs_count++;
      }
      for (auto spirit_id : *spirits[id2].cur) {
        auto& new_gang = *spirits[id1].cur;
        auto& spirit =spirits[spirit_id];
        spirit.gangs_count++;
        spirit.cur = &new_gang;
        new_gang.insert(spirit_id);
      }
    } else if (q == 2) {
      int id1, id2;
      cin>>id1>>id2;
      if (spirits[id1].cur == spirits[id2].cur) {
        cout<<"YES\n";
      } else {
        cout<<"NO\n";
      }
    } else if (q == 3) {
      int id;
      cin>>id;
      cout<<spirits[id].gangs_count << '\n';
    }
  }
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
  cout << setprecision(10) << fixed;
#ifdef LOCAL
  auto start = std::chrono::system_clock::now();
#endif
#ifdef VI
  freopen("in.txt", "r", stdin);
#endif
  solve();
#ifdef LOCAL
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<ld> elapsed_seconds = end - start;
  cout << "\ntime: " << elapsed_seconds.count() << " s\n";
#endif
}
