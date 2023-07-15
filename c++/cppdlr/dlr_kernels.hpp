// Copyright (c) 2022-2023 Simons Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0.txt
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors: Jason Kaye

#pragma once
#include <complex>

namespace cppdlr {

  /**
   * The Particle Statistic: Boson or Fermion
   */

  enum statistic_t { Boson = 0, Fermion = 1 };

  /**
  * @brief Evaluate analytic continuation kernel in imaginary time (relative
  * time format) using dimensionless variables (beta = 1)
  *
  * @param[in] t Imaginary time value (relative format)
  * @param[in] om Real frequency value
  *
  * @return Value K(t,om) of analytic continuation kernel
  *
  * \note The definition of the analytic continuation kernel using dimensionless
  * variables is K(t,om) = -exp(-t * om)/(1 + exp(-om)).
  */
  double k_it(double t, double om);

  /**
  * @brief Evaluate analytic continuation kernel in imaginary time (relative
  * time format) for a given inverse temperature
  *
  * @param[in] t Imaginary time value (relative format)
  * @param[in] om Real frequency value
  * @param[in] beta Inverse temperature
  *
  * @return Value K(t,om) of analytic continuation kernel
  *
  * \note The definition of the analytic continuation kernel at a given inverse
  * temperature beta is K(t,om) = -exp(-t*om)/(1+exp(-beta * om)).
  */
  double k_it(double t, double om, double beta);

  /**
  * @brief Evaluate analytic continuation kernel in imaginary time (absolute
  * time format) using dimensionless variables (beta = 1)
  *
  * @param[in] t Imaginary time value (absolute format)
  * @param[in] om Real frequency value
  *
  * @return Value K(t,om) of analytic continuation kernel
  *
  * \note The definition of the analytic continuation kernel using dimensionless
  * variables is K(t,om) = -exp(-t * om)/(1 + exp(-om)).
  */
  double k_it_abs(double t, double om);

  /**
  * @brief Evaluate analytic continuation kernel in imaginary frequency using
  * dimensionless variables (beta = 1)
  *
  * @param[in] n Imaginary frequency index
  * @param[in] om Real frequency value
  * @param[in] statistic Particle Statistic: Boson or Fermion
  *
  * @return Value K(i nu_n,om) of analytic continuation kernel
  *
  * \note The definition of the analytic continuation kernel using dimensionless
  * variables is K(i nu_n, om) = 1 / (i nu_n - om) with i nu_n = (2n+1) * i *
  * pi (fermionic case) or i nu_n = 2n * i * pi (bosonic case).
  */
  std::complex<double> k_if(int n, double om, statistic_t statistic);

  /**
  * @brief Evaluate analytic continuation kernel in imaginary frequency for a
  * given inverse temperature
  *
  * @param[in] n Imaginary frequency index
  * @param[in] om Real frequency value
  * @param[in] statistic Particle Statistic: Boson or Fermion
  * @param[in] beta Inverse temperature
  *
  * @return Value K(i nu_n,om) of analytic continuation kernel
  *
  * \note The definition of the analytic continuation kernel at a given inverse
  * temperature beta is K(i nu_n, om) = 1 / (i nu_n - om) with i nu_n = (2n+1) *
  * i * pi / beta (fermionic case) or i nu_n = 2n * i * pi / beta (bosonic case).
  */
  std::complex<double> k_if(int n, double om, statistic_t statistic, double beta);

} // namespace cppdlr
