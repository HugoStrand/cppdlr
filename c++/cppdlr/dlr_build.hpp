#pragma once
#include <nda/nda.hpp>

using namespace nda;

namespace cppdlr {

  /**
   * Contains empirically-chosen fine grid parameters to use in
   * construction of DLR basis.
   */

  class fineparams {

    public:
    fineparams(double lambda, int p = 24);

    const double lambda; /// DLR cutoff
    const int p;         /// Fine grid Chebyshev order
    const double iommax; /// Imaginary frequency cutoff
    const int npom;       /// Number fine frequency grid panels
    const int npt;       /// Number fine imaginary time grid panels
    const int npiom;      /// Number fine imaginary frequency grid panels
    const int nom;        /// Total number fine frequency grid points
    const int nt;        /// Total number fine imaginary time grid points
    const int niom;       /// Total number fine imaginary frequency grid points
  };

  /**
   * Get fine grid (composite Chebyshev grid) in frequency
   */

  nda::vector<double> get_omfine(fineparams &fine);

  /**
   * Get fine grid (composite Chebyshev grid) in imaginary time; note that relative time format is used
   */

  nda::vector<double> get_tfine(fineparams &fine);

  /**
   * Get fine grid (composite Chebyshev grid) in imaginary frequency
   */

  nda::vector<double> get_iomfine(fineparams &fine);

  /**
   * Get fine discretization of Lehmann kernel in imaginary time
   */

  nda::matrix<double> get_kfine(nda::vector_const_view<double> t, nda::vector_const_view<double> om);

  /**
   * Get fine discretization of Lehmann kernel in imaginary frequency
   */

  nda::matrix<dcomplex> get_kfineif(nda::vector_const_view<double> iom, nda::vector_const_view<double> om);

  /** 
   * Test accuracy of fine discretization of Lehmann kernel produced by function
   * get_kfine 
   * */

  std::tuple<double, double> get_kfineerr(fineparams &fine, nda::vector_const_view<double> t, nda::vector_const_view<double> om,
                                          nda::matrix_const_view<double> kmat);

} // namespace cppdlr
