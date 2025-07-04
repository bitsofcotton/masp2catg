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

template <typename T> static inline SimpleMatrix<T> swapM(const SimpleMatrix<T>& m, const int& i, const int& j) {
  auto res(m);
  if(i != j) std::swap(res.row(i), res.row(j));
  return res;
}

template <typename T> static inline SimpleMatrix<T> exchg3(const int& i0, const int& j0, const int& k0) {
  SimpleMatrix<T> Q(4, 4);
  Q.O();
  for(int i = 0; i < 2; i ++)
    for(int j = 0; j < Q.cols(); j ++)
      Q(i, j) = j == i0 ? num_t(int(1)) :
        (j == k0 ? - num_t(int(1)) : num_t(int(0)) );
  for(int j = 0; j < Q.cols(); j ++)
    Q(2, j) = j == j0 ? num_t(int(1)) :
      (j == k0 ? - num_t(int(1)) : num_t(int(0)) );
  for(int j = 0; j < Q.cols(); j ++)
    Q(3, j) = num_t(int(0));
  return Q;
}

template <typename T> static inline SimpleVector<T> diag(const SimpleMatrix<T>& m) {
  assert(m.rows() == m.cols());
  SimpleVector<T> res(m.rows());
  for(int i = 0; i < res.size(); i ++)
    res[i] = m(i, i);
  return res;
}

template <typename T> static inline SimpleMatrix<T> pullQ(const SimpleMatrix<T>& L) {
  // N.B. masp2catg - [extQ, extQ.transpose() * L] == [I, L].
  //  <=> [[q00 q01 q02 q03] ... [q30 q31 q32 q33]] v == L.row(3)
  //      [[q00 ... q33]] exchg(0, 3) v == L.row(0)
  //      [[q00 ... q33]] exchg(1, 3) v == L.row(1)
  //      [[q00 ... q33]] exchg(2, 3) v == L.row(2)
  //      ..
  //  <=> [exchg(id)v, exchg(0, 3)v, exchg(1, 3)v, exchg(2, 3)v] ==
  //      extQ.transpose() * L
  //       ...
  //  <=> v0 = L.transpose() extQ.row(0) == L.transpose() extQ.row(3)
  //      v1 = L.transpose() extQ.row(1) == L.transpose() extQ.row(3)
  //      v2 = L.transpose() extQ.row(2) == L.transpose() extQ.row(3)
  //      ...
  //  <=> L.transpose() (extQ.row(k) - extQ.row(3)) == 0.
  //  <=> (R0.transpose() * Q0.transpose()) orthogonal to
  //        (extQ.row(k) - extQ.row(3))
  //   => Q0.transpose().dot(extQ.row(k) - extQ.row(3)) == R0.upper(4, 4).
  //   => Q0.col(0).dot(extQ.row(2) - extQ.row(3)) == 0.
  //      Q0.col(1).dot(extQ.row(2) - extQ.row(3)) == 0.
  //      Q0.col(2).dot(extQ.row(1) - extQ.row(3)) == 0.
  //  ... first to L=QR, then extQ : Q.transpose() * extQ cond,
  //      Q.transpose() * extQ.transpose() result.
  SimpleMatrix<num_t> QQ(4, 4);
  const auto ex3(exchg3<num_t>(2, 1, 3));
  QQ.row(0) = diag<num_t>(swapM<num_t>(ex3 * swapM<num_t>(L, 3, 3).QR(), 3, 3));
  QQ.row(1) = diag<num_t>(swapM<num_t>(ex3 * swapM<num_t>(L, 2, 3).QR(), 2, 3));
  QQ.row(2) = diag<num_t>(swapM<num_t>(ex3 * swapM<num_t>(L, 1, 3).QR(), 1, 3));
  QQ.row(3) = diag<num_t>(swapM<num_t>(ex3 * swapM<num_t>(L, 0, 3).QR(), 0, 3));
  return (QQ.QR() * L.QR()).transpose();
}

#undef int
int main(int argc, const char* argv[]) {
//#define int int64_t
#define int int32_t
  SimpleMatrix<num_t> L;
  if(argc < 2) goto usage;
  std::cin >> L;
  if(argv[1][0] == '-') {
    L = L.transpose();
    for(int i0 = 0; i0 < L.rows() / L.cols() - 1; i0 ++) {
      SimpleMatrix<num_t> res(L.cols(), L.rows());
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
  } else if(argv[1][0] == '+') {
    const auto extQ(pullQ<num_t>(L));
    cerr << " *** Transpose might not proper: *** " << extQ << endl;
    for(int i = 2; i < argc; i ++) {
      vector<SimpleMatrix<num_t> > work;
      if(! loadp2or3<num_t>(work, argv[i])) continue;
      SimpleVector<num_t> in(work.size() * work[0].rows() * work[0].cols());
      in.O();
      for(int j = 0; j < work.size(); j ++)
        for(int k = 0; k < work[j].rows(); k ++)
          in.setVector(j * work[0].rows() * work[0].cols() +
            k * work[0].cols(), work[j].row(k));
      vector<SimpleMatrix<num_t> > out;
      out.resize(1, SimpleMatrix<num_t>(1, 4));
      out[0].row(0) = revertProgramInvariant<num_t>(make_pair(- extQ.transpose() * (L * makeProgramInvariant<num_t>(in).first), num_t(int(1)) ) );
      if(! savep2or3<num_t>((std::string(argv[i]) + std::string("-m2c4.pgm")).c_str(), out) )
        cerr << "failed to save." << endl;
    }
  } else if(argv[1][0] == 't') {
    const auto extQ(pullQ<num_t>(L));
    const auto LQ(L.QR());
    cerr << " *** Transpose might not proper: *** " << extQ << endl;
    SimpleVector<num_t> seed(4);
    for(int i = 0; i < 16; i ++) {
      for(int j = 0; j < seed.size(); j ++)
        seed[j] = i & (j ? 1 << j : 1) ? num_t(int(1)) : num_t(int(0));
      vector<SimpleMatrix<num_t> > lwork;
      lwork.resize(1, SimpleMatrix<num_t>(1, 4).O());
      lwork[0].row(0) = revertProgramInvariant<num_t>(make_pair(LQ.transpose() * extQ.transpose() * LQ.transpose() * seed, num_t(int(1)) ) );
      if(! savep2or3<num_t>((std::string("transient-") + std::to_string(i) + std::string(".pgm")).c_str(), lwork) )
        cerr << "failed to save." << endl;
    }
  } else goto usage;
  return 0;
 usage:
  cerr << "Usage:" << std::endl;
  cerr << "# strip down masp result:" << std::endl;
  cerr << argv[0] << " - < ..." << std::endl;
  cerr << "# add to original stream, different vector meaning:" << std::endl;
  cerr << argv[0] << " + <input0.ppm> ... < ..." << std::endl;
  cerr << "# transient on meaning also returns same meaning:" << std::endl;
  cerr << argv[0] << " t < ..." << std::endl;
  return - 1;
}

