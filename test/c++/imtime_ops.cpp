/** 
* @file imtime_ops.cpp
*
* @brief Tests for imtime_ops class.
*/

#include <gtest/gtest.h>
#include <nda/nda.hpp>
#include <nda/gtest_tools.hpp>

#include <cppdlr/dlr_imtime.hpp>
#include <cppdlr/dlr_build.hpp>
#include <cppdlr/utils.hpp>
#include <cppdlr/dlr_kernels.hpp>
#include <nda/blas.hpp>

using namespace cppdlr;
using namespace nda;

static constexpr auto _ = nda::range::all;

/**
* @brief Green's function which is a random sum of exponentials
*
* G_ij(t) = sum_l c_ijl K(t,om_ijl) with random c_ijl, om_ijl
*
* @param norb Number of orbital indices
* @param beta Inverse temperature
* @param t    Imaginary time evaluation point
*
* @return Green's function evaluated at t
*/
nda::matrix<double> gfun(int norb, double beta, double t) {

  int npeak = 5;

  auto g    = nda::matrix<double>(norb, norb);
  g         = 0;
  auto c    = nda::vector<double>(npeak);
  double om = 0;
  for (int i = 0; i < norb; ++i) {
    for (int j = 0; j < norb; ++j) {

      // Get random weights that sum to 1
      for (int l = 0; l < npeak; ++l) {
        c(l) = (sin(1000.0 * (i + 2 * j + 3 * l + 7)) + 1) / 2; // Quick and dirty rand # gen on [0,1]
      }
      c = c / sum(c);

      // Evaluate Green's function
      for (int l = 0; l < npeak; ++l) {
        om = sin(2000.0 * (3 * i + 2 * j + l + 6)); // Rand # on [-1,1]
        g(i, j) += c(l) * k_it(t, beta * om);
      }
    }
  }

  return g;
}

/**
* @brief Test DLR interpolation and evaluation for matrix-valued Green's
* function
*/
TEST(imtime_ops, interp_matrix) {

  double lambda = 1000;  // DLR cutoff
  double eps    = 1e-10; // DLR tolerance

  double beta = 1000;  // Inverse temperature
  int ntst    = 10000; // # imag time test points

  int norb = 2; // Orbital dimensions

  // Get DLR frequencies
  auto dlr_rf = build_dlr_rf(lambda, eps);

  // Get DLR imaginary time object
  auto itops = imtime_ops(lambda, dlr_rf);

  // Sample Green's function G at DLR imaginary time nodes
  int r = itops.rank();
  // [Q] Is this correct or just auto?
  auto const &dlr_it = itops.get_itnodes();
  auto g             = nda::array<double, 3>(r, norb, norb);
  for (int i = 0; i < r; ++i) { g(i, _, _) = gfun(norb, beta, dlr_it(i)); }

  // DLR coefficients of G
  auto gc = itops.vals2coefs(g);

  // Check that G can be recovered at imaginary time nodes
  EXPECT_LT(max_element(abs(itops.coefs2vals(gc) - g)), 1e-14);

  // Get test points in relative format
  auto ttst = eqptsrel(ntst);

  // Compute L infinity error
  auto gtru  = nda::matrix<double>(norb, norb);
  auto gtst  = nda::matrix<double>(norb, norb);
  double err = 0;
  for (int i = 0; i < ntst; ++i) {
    gtru = gfun(norb, beta, ttst(i));
    gtst = itops.coefs2eval(gc, ttst(i));
    err  = std::max(err, max_element(abs(gtru - gtst)));
  }

  EXPECT_LT(err, 10 * eps);
}

/**
* @brief Test DLR interpolation and evaluation for complex and matrix-valued Green's
* function
*/
TEST(imtime_ops, interp_matrix_complex) {

  double lambda = 1000;  // DLR cutoff
  double eps    = 1e-10; // DLR tolerance

  double beta = 1000;  // Inverse temperature
  int ntst    = 10000; // # imag time test points

  int norb = 2; // Orbital dimensions

  // Get DLR frequencies
  auto dlr_rf = build_dlr_rf(lambda, eps);

  // Get DLR imaginary time object
  auto itops = imtime_ops(lambda, dlr_rf);

  // Sample Green's function G at DLR imaginary time nodes
  int r = itops.rank();
  // [Q] Is this correct or just auto?
  auto const &dlr_it = itops.get_itnodes();
  auto g             = nda::array<std::complex<double>, 3>(r, norb, norb);
  for (int i = 0; i < r; ++i) { g(i, _, _) = gfun(norb, beta, dlr_it(i)); }

  // DLR coefficients of G
  auto gc = itops.vals2coefs(g);

  // Check that G can be recovered at imaginary time nodes
  EXPECT_LT(max_element(abs(itops.coefs2vals(gc) - g)), 1e-14);

  // Get test points in relative format
  auto ttst = eqptsrel(ntst);

  // Compute L infinity error
  auto gtru  = nda::matrix<std::complex<double>>(norb, norb);
  auto gtst  = nda::matrix<std::complex<double>>(norb, norb);
  double err = 0;
  for (int i = 0; i < ntst; ++i) {
    gtru = gfun(norb, beta, ttst(i));
    gtst = itops.coefs2eval(gc, ttst(i));
    err  = std::max(err, max_element(abs(gtru - gtst)));
  }

  EXPECT_LT(err, 10 * eps);
}

/**
* @brief Test DLR interpolation and evaluation for scalar-valued Green's
* function
*/
TEST(imtime_ops, interp_scalar) {

  double lambda = 1000;  // DLR cutoff
  double eps    = 1e-10; // DLR tolerance

  double beta = 1000;  // Inverse temperature
  int ntst    = 10000; // # imag time test points

  // Get DLR frequencies
  auto dlr_rf = build_dlr_rf(lambda, eps);

  // Get DLR imaginary time object
  auto itops = imtime_ops(lambda, dlr_rf);

  // Sample Green's function G at DLR imaginary time nodes
  int r = itops.rank();
  // [Q] Is this correct or just auto?
  auto const &dlr_it = itops.get_itnodes();
  auto g             = nda::vector<double>(r);
  for (int i = 0; i < r; ++i) { g(i) = gfun(1, beta, dlr_it(i))(0, 0); }

  // DLR coefficients of G
  auto gc = itops.vals2coefs(g);

  // Check that G can be recovered at imaginary time nodes
  EXPECT_LT(max_element(abs(itops.coefs2vals(gc) - g)), 1e-14);

  // Get test points in relative format
  auto ttst = eqptsrel(ntst);

  // Compute L infinity error
  double gtru = 0, gtst = 0, err = 0;
  for (int i = 0; i < ntst; ++i) {
    gtru = gfun(1, beta, ttst(i))(0, 0);
    gtst = itops.coefs2eval(gc, ttst(i));
    err  = std::max(err, abs(gtru - gtst));
  }

  EXPECT_LT(err, 10 * eps);

  // Test that constructing vector of evaluation at a point and then applying to
  // coefficients gives same result as direct evaluation method
  auto kvec = itops.build_evalvec(ttst(ntst - 1));
  EXPECT_LT((abs(blas::dot(gc, kvec) - gtst)), 1e-14);
}

/**
* @brief Test convolution of two scalar-valued Green's functions
*
* We use Green's functions f and g given by a single exponential, so that the
* result of the convolution is easy to compute analytically.
*/
TEST(imtime_ops, convolve_scalar) {

  double lambda = 1000;  // DLR cutoff
  double eps    = 1e-12; // DLR tolerance

  double beta = 1000;  // Inverse temperature
  int ntst    = 10000; // # imag time test points

  // Specify frequency of single exponentials to be used for f and g
  double omf = 0.1234, omg = -0.5678;

  // Get DLR frequencies
  auto dlr_rf = build_dlr_rf(lambda, eps);

  // Get DLR imaginary time object
  auto itops = imtime_ops(lambda, dlr_rf);

  // Sample Green's function G at DLR imaginary time nodes
  int r = itops.rank();
  // [Q] Is this correct or just auto?
  auto const &dlr_it = itops.get_itnodes();
  auto f             = nda::array<double, 1>(r);
  auto g             = nda::array<double, 1>(r);
  for (int i = 0; i < r; ++i) { f(i) = k_it(dlr_it(i), beta * omf); };
  for (int i = 0; i < r; ++i) { g(i) = k_it(dlr_it(i), beta * omg); };

  // Get DLR coefficients of f and g
  auto fc = itops.vals2coefs(f);
  auto gc = itops.vals2coefs(g);

  // Get convolution of f and g directly
  auto h = itops.convolve(beta, Fermion, fc, gc);

  // Get convolution of f and g by first forming matrix of convolution by f and
  // then applying it to g
  auto h2 = itops.convolve(itops.convmat(beta, Fermion, fc), g);

  // Check that the two methods give the same result
  EXPECT_LT(max_element(abs(h - h2)), 1e-14);

  // Check error of h

  auto hc = itops.vals2coefs(h); // DLR coefficients of h

  // Get test points in relative format
  auto ttst = eqptsrel(ntst);

  // Compute L infinity error
  double gtru = 0, gtst = 0, err = 0;
  for (int i = 0; i < ntst; ++i) {
    gtru = (k_it(ttst(i), beta * omf) - k_it(ttst(i), beta * omg)) / (omg - omf); // Exact result
    gtst = itops.coefs2eval(hc, ttst(i));
    err  = std::max(err, abs(gtru - gtst));
  }

  EXPECT_LT(err, 10 * eps);
}

/**
* @brief Test convolution of two matrix-valued Green's functions
*
* We use Green's functions f and g given by the product of a single exponential and a
* matrix of ones, so that the result of the convolution is easy to compute
* analytically
*/
TEST(imtime_ops, convolve_matrix) {

  double lambda = 1000;  // DLR cutoff
  double eps    = 1e-12; // DLR tolerance

  double beta = 1000;  // Inverse temperature
  int ntst    = 10000; // # imag time test points

  int norb = 2; // Orbital dimensions

  // Specify frequency of single exponentials to be used for f and g
  double omf = 0.1234, omg = -0.5678;

  // Get DLR frequencies
  auto dlr_rf = build_dlr_rf(lambda, eps);

  // Get DLR imaginary time object
  auto itops = imtime_ops(lambda, dlr_rf);

  // Sample Green's function G at DLR imaginary time nodes
  int r = itops.rank();
  // [Q] Is this correct or just auto?
  auto const &dlr_it = itops.get_itnodes();
  auto f             = nda::array<double, 3>(r, norb, norb);
  auto g             = nda::array<double, 3>(r, norb, norb);
  for (int i = 0; i < r; ++i) { f(i, _, _) = k_it(dlr_it(i), beta * omf) / sqrt(1.0 * norb); };
  for (int i = 0; i < r; ++i) { g(i, _, _) = k_it(dlr_it(i), beta * omg) / sqrt(1.0 * norb); };

  // Get DLR coefficients of f and g
  auto fc = itops.vals2coefs(f);
  auto gc = itops.vals2coefs(g);

  // Get convolution of f and g directly
  auto h = itops.convolve(beta, Fermion, fc, gc);

  // Get convolution of f and g by first forming matrix of convolution by f and
  // then applying it to g
  auto h2 = itops.convolve(itops.convmat(beta, Fermion, fc), g);

  // Check that the two methods give the same result
  EXPECT_LT(max_element(abs(h - h2)), 1e-14);

  // Check error of h

  auto hc = itops.vals2coefs(h); // DLR coefficients of h

  // Get test points in relative format
  auto ttst = eqptsrel(ntst);

  // Compute L infinity error
  auto gtru  = nda::array<double, 2>(norb, norb);
  auto gtst  = nda::array<double, 2>(norb, norb);
  double err = 0;
  for (int i = 0; i < ntst; ++i) {
    gtru(_, _) = (k_it(ttst(i), beta * omf) - k_it(ttst(i), beta * omg)) / (omg - omf); // Exact result
    gtst       = itops.coefs2eval(hc, ttst(i));
    err        = std::max(err, max_element(abs(gtru - gtst)));
  }

  EXPECT_LT(err, 10 * eps);
}

TEST(dlr_imtime, h5_rw) {

  double lambda = 1000;  // DLR cutoff
  double eps    = 1e-10; // DLR tolerance

  // Get DLR frequencies
  auto dlr_rf = build_dlr_rf(lambda, eps);

  // Get DLR imaginary time object
  auto itops = imtime_ops(lambda, dlr_rf);

  auto filename = "data_imtime_ops_h5_rw.h5";
  auto name     = "itops";

  {
    h5::file file(filename, 'w');
    h5::write(file, name, itops);
  }

  imtime_ops itops_ref;
  {
    h5::file file(filename, 'r');
    h5::read(file, name, itops_ref);
  }

  // Check equal
  EXPECT_EQ(itops.lambda(), itops_ref.lambda());
  EXPECT_EQ(itops.rank(), itops_ref.rank());
  EXPECT_EQ_ARRAY(itops.get_rfnodes(), itops_ref.get_rfnodes());
  EXPECT_EQ_ARRAY(itops.get_itnodes(), itops_ref.get_itnodes());
  EXPECT_EQ_ARRAY(itops.get_cf2it(), itops_ref.get_cf2it());
  EXPECT_EQ_ARRAY(itops.get_it2cf_lu(), itops_ref.get_it2cf_lu());
  EXPECT_EQ_ARRAY(itops.get_it2cf_piv(), itops_ref.get_it2cf_piv());
}
