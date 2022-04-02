const vector<pair<int, int>> hash_config = {{29, 1000000007}, {31, 1000000009}};

vector<vector<int>> fact;

struct HashString {
  vector<vector<int>> hash;

  HashString(const string& s) {
    hash.resize(hash_config.size());
    for (int i = 0; i < hash.size(); i++) {
      hash[i].resize(s.length());
      auto [p, mod] = hash_config[i];
      hash[i][0] = s[0] - 'a' + 1;
      for (int j = 1; j < s.length(); j++) {
        hash[i][j] = (int64)hash[i][j - 1] * p % mod;
        hash[i][j] = (hash[i][j] + s[j] - 'a' + 1) % mod;
      }
    }
  }

  vector<int> GetHash(int l, int r) const {
    assert(0 <= l);
    assert(l <= r);
    assert(r < hash[0].size());
    vector<int> res(hash_config.size());
    for (int i = 0; i < hash_config.size(); i++) {
      int mod = hash_config[i].second;
      int a = hash[i][r];
      int b = (int64)(l == 0 ? 0 : hash[i][l - 1]) * fact[i][r - l + 1] % mod;
      res[i] = (a - b + mod) % mod;
    }
    return res;
  }

  int Length() const {
    return hash[0].size();
  }

  static void Prepare(int n) {
    fact.resize(hash_config.size());
    for (int i = 0; i < fact.size(); i++) {
      fact[i].resize(n + 1);
      auto [p, mod] = hash_config[i];
      fact[i][0] = 1;
      for (int j = 1; j <= n; j++) {
        fact[i][j] = (int64)fact[i][j - 1] * p % mod;
      }
    }
  }
};