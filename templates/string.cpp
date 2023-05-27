// KMP
struct KMP {
  string s;
  vector<int> next;

  KMP(const string& s_) : s(s_) {
    next.resize(s.length());
    std::fill(next.begin(), next.end(), 0);
    for (int i = 1, j = 0; i < s.length(); i++) {
      while (j && s[j] != s[i]) j = next[j - 1];
      if (s[j] == s[i]) j++;
      next[i] = j;
    }
  }

  vector<int> Match(const string& t) {
    vector<int> res;
    for (int i = 0, j = 0; i < t.length(); i++) {
      while (j && s[j] != t[i]) j = next[j - 1];
      if (s[j] == t[i]) j++;
      if (j == s.length()) {
        res.push_back(i - s.length() + 1);
        j = next[j - 1];
      }
    }
    return res;
  }
};
