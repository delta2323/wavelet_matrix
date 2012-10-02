#include <iostream>
#include <gtest/gtest.h>
#include <ux/rsDic.hpp>

using namespace std;
using namespace ux;

namespace wm {
namespace test {

TEST(rsDic, confimation_of_notation_and_index) {
  ux::BitVec v;
  v.push_back(0);
  v.push_back(1);
  v.push_back(1);
  v.push_back(0);
  v.push_back(1);
  ux::RSDic d;
  d.build(v);

  ASSERT_EQ(0, d.select(1, 0));
  ASSERT_EQ(3, d.select(2, 0));

  ASSERT_EQ(1, d.select(1, 1));
  ASSERT_EQ(2, d.select(2, 1));
  ASSERT_EQ(4, d.select(3, 1));
}

}
}
