#include <gtest/gtest.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "wm.hpp"

using namespace std;

namespace wm {
namespace test {

typedef uint8_t char_t;
typedef uint64_t id_t;
const static size_t kBitlen = sizeof(char_t)*8;
const static size_t kMax = (1 << kBitlen);

TEST(wm, same_char) {
  vector<char_t> vs;
  for(size_t n = 0; n < kMax; ++n) {
    vs.push_back(static_cast<char_t>(0));
  }
  
  WM w(vs);
  for(size_t i = 0; i < kMax; ++i) {
    ASSERT_EQ(i, w.Rank(static_cast<char_t>(i), 0));
  }
}

TEST(wm, same_char_access) {
  vector<char_t> vs;
  for(size_t n = 0; n < kMax; ++n) {
    vs.push_back(static_cast<char_t>(0));
  }
  
  WM w(vs);
  for(size_t i = 0; i < kMax; ++i) {
    ASSERT_EQ(vs[i], w.Access(static_cast<char_t>(i)));
  }
}

TEST(wm, different_type) {
  srand(0);
  vector<char_t> vs;
  for(size_t n = 0; n < kMax; ++n) {
    vs.push_back(static_cast<char_t>(n));
  }
  random_shuffle(vs.begin(), vs.end());
  
  WM w(vs);
  for(size_t i = 0; i < kMax; ++i) {
    for(size_t j = 0; i < kMax; ++i) {
      id_t expected = i < j ? 1 : 0;
      ASSERT_EQ(expected, w.Rank(static_cast<char_t>(j), vs[i]));
    }
  }
}

TEST(wm, different_type_access) {
  srand(0);
  vector<char_t> vs;
  for(size_t n = 0; n < kMax; ++n) {
    vs.push_back(static_cast<char_t>(n));
  }
  random_shuffle(vs.begin(), vs.end());

  WM w(vs);
  for(size_t i = 0; i < kMax; ++i) {
    cout << i << " ";
    ASSERT_EQ(static_cast<int>(vs[i]), static_cast<int>(w.Access(static_cast<char_t>(i))));
  }
}

} // test
} // wm
