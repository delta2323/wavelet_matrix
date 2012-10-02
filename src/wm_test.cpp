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

TEST(wm, confirmation_of_notation_and_index) {
  vector<char_t> vs;
  vs.push_back(1);
  vs.push_back(2);
  vs.push_back(2);
  vs.push_back(1);
  vs.push_back(2);

  WM w(vs);
  for(size_t i = 0; i < 5; ++i) {
    ASSERT_EQ(vs[i], w.Access(i));
  }

  ASSERT_EQ(0, w.Rank(0, 1));
  ASSERT_EQ(1, w.Rank(1, 1));
  ASSERT_EQ(1, w.Rank(2, 1));
  ASSERT_EQ(1, w.Rank(3, 1));
  ASSERT_EQ(2, w.Rank(4, 1));
  ASSERT_EQ(2, w.Rank(5, 1));

  ASSERT_EQ(0, w.Rank(0, 2));
  ASSERT_EQ(0, w.Rank(1, 2));
  ASSERT_EQ(1, w.Rank(2, 2));
  ASSERT_EQ(2, w.Rank(3, 2));
  ASSERT_EQ(2, w.Rank(4, 2));
  ASSERT_EQ(3, w.Rank(5, 2));

  ASSERT_EQ(0, w.Select(0, 1));
  ASSERT_EQ(3, w.Select(1, 1));

  ASSERT_EQ(1, w.Select(0, 2));
  ASSERT_EQ(2, w.Select(1, 2));
  ASSERT_EQ(4, w.Select(2, 2));
}

class SameCharTest : public testing::TestWithParam<size_t> {
public:
  static void SetUpTestCase() {
    vs = new vector<char_t>(kMax, 0);
    w = new WM(*vs);
  }
protected:
  static vector<char_t>* vs;
  static WM* w;
};

vector<char_t>* SameCharTest::vs = NULL;
WM* SameCharTest::w = NULL;

INSTANTIATE_TEST_CASE_P(SameCharTestInstance,
                        SameCharTest,
			::testing::Range((size_t)0, kMax));

TEST_P(SameCharTest, access) {
  id_t idx = GetParam();
  ASSERT_EQ((*vs)[idx], w->Access(idx));
}

TEST_P(SameCharTest, rank) {
  id_t idx = GetParam();
  ASSERT_EQ(idx, w->Rank(idx, 0));
}

TEST_P(SameCharTest, select) {
  id_t idx = GetParam();
  ASSERT_EQ(idx, w->Select(idx, 0));
}

class DifferentTypeTest : public testing::TestWithParam<size_t> {
public:
  static void SetUpTestCase() {
    srand(0);
    vs = new vector<char_t>(kMax);
    for(size_t n = 0; n < kMax; ++n) {
      (*vs)[n] = n;
    }
    random_shuffle(vs->begin(), vs->end());
    w = new WM(*vs);
  }
protected:
  static vector<char_t>* vs;
  static WM* w;
};

vector<char_t>* DifferentTypeTest::vs = NULL;
WM* DifferentTypeTest::w = NULL;

INSTANTIATE_TEST_CASE_P(DifferentTypeTestInstance,
                        DifferentTypeTest,
			::testing::Range((size_t)0, kMax));

TEST_P(DifferentTypeTest, access) {
  id_t idx = GetParam();
  ASSERT_EQ((*vs)[idx], w->Access(idx));
}

TEST_P(DifferentTypeTest, rank) {
  id_t idx = GetParam();
  ASSERT_EQ(0, w->Rank(idx, (*vs)[idx]));
}

TEST_P(DifferentTypeTest, rank_1) {
  id_t idx = GetParam();
  ASSERT_EQ(1, w->Rank(idx+1, (*vs)[idx]));
}

TEST_P(DifferentTypeTest, select) {
  id_t idx = GetParam();
  ASSERT_EQ(idx, w->Select(0, (*vs)[idx]));
}

class RandomTest : public testing::TestWithParam<size_t> {
public:
  static void SetUpTestCase() {
    srand(100);
    vs = new vector<char_t>(N);
    for(size_t n = 0; n < N; ++n) {
      (*vs)[n] = rand() % M;
    }

    ranks = new vector<vector<char_t> >(N+1, vector<char_t>(M));
    for(size_t n = 1; n <= N; ++n) {
      for(size_t m = 0; m < M; ++m) {
	(*ranks)[n][m] = (*ranks)[n-1][m] + ((*vs)[n-1] == m ? 1 : 0);
      }
    }    

    selects = new vector<pair<id_t, char_t> >(N);
    for(size_t n = 0; n < N; ++n) {
      char_t c = (*vs)[n];
      (*selects)[n] = make_pair((*ranks)[n][c], c);
    }    

    w = new WM(*vs);
  }
  const static size_t N = 1000;
protected:
  const static size_t M = 10;
  static vector<char_t>* vs;
  static WM* w;
  static vector<vector<char_t> >* ranks;
  static vector<pair<id_t, char_t> >* selects;
};

vector<vector<char_t> >* RandomTest::ranks = NULL;
vector<pair<id_t, char_t> >* RandomTest::selects = NULL;
vector<char_t>* RandomTest::vs = NULL;
WM* RandomTest::w = NULL;

INSTANTIATE_TEST_CASE_P(RandomTestInstance,
                        RandomTest,
			::testing::Range((size_t)0, RandomTest::N));

TEST_P(RandomTest, access) {
  id_t idx = GetParam();
  ASSERT_EQ((*vs)[idx], w->Access(idx));
}

TEST_P(RandomTest, rank) {
  size_t n = GetParam();
  for(size_t m = 0; m < RandomTest::M; ++m) {
    ASSERT_EQ((*ranks)[n][m], w->Rank(n, m));
  }
}

TEST_F(RandomTest, rank_N) {
  for(size_t m = 0; m < RandomTest::M; ++m) {
    ASSERT_EQ((*ranks)[RandomTest::N][m], w->Rank(RandomTest::N, m));
  }
}

TEST_P(RandomTest, select) {
  size_t n = GetParam();
  id_t pos = (*selects)[n].first;
  char_t c = (*selects)[n].second;
  ASSERT_EQ(n, w->Select(pos, c));
}

} // test
} // wm
