// Copyright (c) 2022 Simons Foundation
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
// Authors: jasonkaye

#include <gtest/gtest.h>
#include <cppdlr/utils.hpp>

using namespace cppdlr;

TEST(barycheb, interp) {

  int n = 16;

  barycheb mybarycheb(n);

  auto& xc = mybarycheb.getnodes();

  nda::vector<double> f(n);

  for (int i = 0; i < n; i++) { f(i) = std::cos(xc(i)); }

  double x = 0.378492;

  auto val1 = mybarycheb.interp(x,f);
  auto val2 = std::cos(x);

  EXPECT_NEAR(val1, val2, 1.0e-14);
}
