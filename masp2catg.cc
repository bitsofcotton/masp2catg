#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <random>
#include <assert.h>

//#define int int64_t
#define int int32_t
#include "lieonn.hh"
typedef myfloat num_t;

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::to_string;
using std::make_pair;

#include <stdlib.h>

#undef int
int main(int argc, const char* argv[]) {
//#define int int64_t
#define int int32_t
  SimpleMatrix<num_t> L;
  SimpleMatrix<num_t> res;
  std::cin >> L;
  L = L.transpose();
  for(int i0 = 0; i0 < L.rows() / L.cols() - 1; i0 ++) {
    res.resize(L.cols(), L.rows());
    res.O();
    for(int i = 0; i < L.cols(); i ++) {
      auto workL(L);
      if(i < L.rows() - 1)
        std::swap(workL.row(i), workL.row(workL.rows() - 1));
      res.row(i) = workL.QR().row(workL.cols() - 1);
    }
    L = res.transpose();
    std::cout << res;
  }
  return 0;
}

