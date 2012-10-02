#include <ux/rsDic.hpp>
#include <ux/uxUtil.hpp>

namespace wm {
class WM {
public:
  typedef uint8_t char_t;
  typedef uint64_t id_t;
  WM();
  WM(const std::vector<WM::char_t>& text);
  ~WM();
  void Build(const std::vector<WM::char_t>& text);
  id_t Rank(id_t pos, WM::char_t c) const;  // return the # of occurence of c in text[0:pos)
  id_t Select(id_t pos, WM::char_t c) const; // return the position of pos-th (first occurence if counted as 0th)  c in text.
  char_t Access(id_t pos) const; // return text[pos]
  id_t size();
private:
  const static size_t kDepth = sizeof(char_t) * 8;
  std::vector<ux::RSDic*> bs;
  std::vector<id_t> zs;
};
} //wm

