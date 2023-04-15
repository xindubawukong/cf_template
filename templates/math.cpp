
void GetCC() {
  vector<vector<int>> cc(1005, vector<int>(1005));
  cc[0][0] = 1;
  for (int i = 1; i <= 1000; i++) {
    cc[i][0] = 1;
    for (int j = 1; j <= i; j++) {
      cc[i][j] = (cc[i - 1][j] + cc[i - 1][j - 1]) % mod;
    }
  }
}
