#include <numbers>
#include "utils.hpp"

using namespace nda;
using std::numbers::pi;

namespace cppdlr {

  barycheb::barycheb(int n) : xc(n), wc(n), n(n) {

    //auto ns = arange(1,n+1); // This isn't implemented yet
     
    // vector<int> ns(n);
    // for (int i = 0; i < n; ++i) { ns(i) = i; }

    // auto c = (2 * ns + 1) * pi / (2 * n);
    // xc     = cos(c(range(n-1,-1,-1))); // This line isn't working
    // wc     = sin(c);
    // for (int i = 0; i < n; ++i) {wc(i) = pow(-1,i+1)*wc(i); };

    for (int i = 0; i < n; i++) {
      auto c        = (2 * i + 1) * pi / (2 * n);
      xc(n - i - 1) = std::cos(c);
      wc(n - i - 1) = (1 - 2 * (i % 2)) * std::sin(c);
    }
    
  }

  nda::vector<double> const& barycheb::getnodes() { return xc; }

  double barycheb::interp(double x, nda::vector_const_view<double> f) {

    for (int i = 0; i < n; ++i) {
      if (x == xc(i)) { return f(i); }
    }

    double num = 0.0, den = 0.0;
    double dif, q;

    for (int i = 0; i < n; ++i) {
      dif = x - xc(i);
      q   = wc(i) / dif;
      num = num + q * f(i);
      den = den + q;
    }

    return num / den;
  }

} // namespace cppdlr
