#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
#include "wm.hpp"

using namespace wm;
using namespace std;

WM::WM() {}

WM::WM(const std::vector<WM::char_t>& text) {
  Build(text);
}

class Comparator {
public:
  Comparator(size_t d_) : d(d_){}
  bool operator()(WM::char_t a, WM::char_t b) {
    return ((a >> d) & 1) < ((b >> d) & 1);
  }
private:
  size_t d;
};

void WM::Build(const std::vector<WM::char_t>& text_) {
  vector<WM::char_t> text(text_);

  bs.resize(kDepth);
  for(size_t d = 0; d < kDepth; ++d) {
    ux::BitVec bv;
    for(size_t i = 0; i < text.size(); ++i) {
      bv.push_back((text[i] >> d) & 1);
    }
    bs[d] = new ux::RSDic();
    bs[d]->build(bv);
    Comparator c(d);
    stable_sort(text.begin(), text.end(), c);
  }

  zs.resize(kDepth);
  for(size_t d = 0; d < kDepth; ++d) {
    zs[d] = bs[d]->rank(text.size()-1, 0);
  }
}
  
WM::~WM() {
}

WM::id_t WM::Rank(WM::id_t pos, WM::char_t c) const{
  WM::id_t beg = 0;
  for(size_t d = 0; d < kDepth; ++d) {
    if (((c >> d) & 1) == 0) {
      pos = bs[d]->rank(pos-1, 0);
      beg = bs[d]->rank(beg-1, 0);
    }else {
      pos = zs[d] + bs[d]->rank(pos-1, 1);
      beg = zs[d] + bs[d]->rank(beg-1, 1);
    }
  }
  return pos - beg;
}

WM::id_t WM::Select(WM::id_t pos, WM::char_t c) const{
  WM::id_t beg = 0;

  for(size_t d = 0; d < kDepth; ++d) {
    if (((c >> d) & 1) == 0) {
      beg = bs[d]->rank(beg-1, 0);
    }else {
      beg = zs[d] + bs[d]->rank(beg-1, 1);
    }
  }

  pos += beg;

  for(int d = kDepth-1; d >= 0; --d) {
    if (((c >> d) & 1) == 0) {
      pos = bs[d]->select(pos+1, 0);
    }else {
      pos = bs[d]->select(pos-zs[d]+1, 1);
    }
  }
  return pos;
}

WM::char_t WM::Access(WM::id_t pos) const{
  char_t cur = 0;
  for(size_t d = 0; d < kDepth; ++d) {
    if(bs[d]->getBit(pos) == 0) {
      pos = bs[d]->rank(pos-1, 0);
    }else {
      cur |= (1 << d);
      pos = zs[d] + bs[d]->rank(pos-1, 1);
    }
  }
  return cur;
}

