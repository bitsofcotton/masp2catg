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
  if(argc < 2) goto usage;
  std::cin >> L;
  if(argv[1][0] == '-') {
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
  } else if(argv[1][0] == '+') {
    SimpleMatrix<num_t> extQ(4, 4);
    // N.B. masp2catg - [extQ, extQ.transpose() * L] == [O, L].
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
          auto Lwork(L);
    const auto Qid((Lwork * Lwork.transpose()).QR());
    std::swap(Lwork.row(2), Lwork.row(3));
    const auto Q23((Lwork * Lwork.transpose()).QR());
    Lwork = L;
    std::swap(Lwork.row(1), Lwork.row(3));
    const auto Q13((Lwork * Lwork.transpose()).QR());
    Lwork = L;
    std::swap(Lwork.row(0), Lwork.row(3));
    const auto Q03((Lwork * Lwork.transpose()).QR());
    // (2-3, 2-3, 1-3) : Qid
    // (3-2, 3-2, 1-2) : Q23
    // (2-1, 2-1, 3-1) : Q13
    // (2-0, 2-0, 1-0) : Q03
    cerr << Qid.rows() << ", " << Qid.cols() << endl;
    SimpleMatrix<num_t> QQ(Qid.rows() * 4, Qid.cols());
    SimpleMatrix<num_t> Qwork(Qid.rows(), Qid.cols());
    Qwork.O();
    for(int i = 0; i < Qwork.rows() - 1; i ++)
      for(int j = 0; j < Qwork.cols(); j ++)
        Qwork(i, j) = j == 2 ? num_t(int(1)) :
          (j == 3 ? - num_t(int(1)) : num_t(int(0)) );
      for(int j = 0; j < Qwork.cols(); j ++)
        Qwork(Qwork.rows() - 1, j) = j == 1 ? num_t(int(1)) :
          (j == 3 ? - num_t(int(1)) : num_t(int(0)) );
    QQ.setMatrix(0, 0, Qwork * Qid);
    Qwork.O();
    for(int i = 0; i < Qwork.rows() - 1; i ++)
      for(int j = 0; j < Qwork.cols(); j ++)
        Qwork(i, j) = j == 3 ? num_t(int(1)) :
          (j == 2 ? - num_t(int(1)) : num_t(int(0)) );
      for(int j = 0; j < Qwork.cols(); j ++)
        Qwork(Qwork.rows() - 1, j) = j == 1 ? num_t(int(1)) :
          (j == 2 ? - num_t(int(1)) : num_t(int(0)) );
    QQ.setMatrix(Qid.rows(), 0, Qwork * Q23);
    Qwork.O();
    for(int i = 0; i < Qwork.rows() - 1; i ++)
      for(int j = 0; j < Qwork.cols(); j ++)
        Qwork(i, j) = j == 2 ? num_t(int(1)) :
          (j == 1 ? - num_t(int(1)) : num_t(int(0)) );
      for(int j = 0; j < Qwork.cols(); j ++)
        Qwork(Qwork.rows() - 1, j) = j == 3 ? num_t(int(1)) :
          (j == 1 ? - num_t(int(1)) : num_t(int(0)) );
    QQ.setMatrix(Qid.rows(), 0, Qwork * Q13);
    Qwork.O();
    for(int i = 0; i < Qwork.rows() - 1; i ++)
      for(int j = 0; j < Qwork.cols(); j ++)
        Qwork(i, j) = j == 2 ? num_t(int(1)) :
          (j == 0 ? - num_t(int(1)) : num_t(int(0)) );
      for(int j = 0; j < Qwork.cols(); j ++)
        Qwork(Qwork.rows() - 1, j) = j == 3 ? num_t(int(1)) :
          (j == 0 ? - num_t(int(1)) : num_t(int(0)) );
    QQ.setMatrix(Qid.rows() * 3, 0, Q03);
    extQ = QQ.transpose().QR();
    cerr << " *** Non proper one: *** " << extQ << endl;
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
      out[0].row(0) = (- extQ.transpose() * (L * makeProgramInvariant<num_t>(in).first) );
      if(! savep2or3<num_t>((std::string(argv[i]) + std::string("-m2c4.pgm")).c_str(), out) )
        cerr << "failed to save." << endl;
    }
  } else goto usage;
  return 0;
 usage:
  cerr << "Usage:" << std::endl;
  cerr << "# strip down masp result:" << std::endl;
  cerr << argv[0] << " - < ..." << std::endl;
  cerr << "# add to original stream:" << std::endl;
  cerr << argv[0] << " + < ..." << std::endl;
  return - 1;
}

