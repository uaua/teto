#include "Onore.h"

#include <inttypes.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <array>
#include <functional>
#include <limits>
#include <queue>
#include <sstream>

#include "Decision.h"
#include "Field.h"
#include "Simulator.h"

class State {
 public:
  Field field;
  int64_t score;
  Decision first;
  MinoType hold;
  std::vector<int> l, r;
  State(const Field& field_, int64_t score_) : field(field_), score(score_) {}
  bool operator<(const State& e) const {
    return score < e.score;
  }
};

int Onore::evalHole(const Field& f) const {
  std::vector<int> t(FIELD_WIDTH);
  for (int i = 0; i < FIELD_WIDTH; i++) {
    t[i] = f.top(i);
  }
  int hole = 0;
  for (int i = 0; i < FIELD_WIDTH; i++) {
    for (int y = t[i]; y < FIELD_HEIGHT; y++) {
      if ( !f.get(i, y).cell() ) hole += 1;
    }
  }
  return hole;
}

int Onore::evalHeight(const Field& f) const {
  std::vector<int> h(FIELD_WIDTH);
  for (int i = 0; i < FIELD_WIDTH; i++) {
    h[i] = f.height(i);
  }
  int hs = 0;
  // int m = *min_element(h.begin(),h.end());
  int m = -114514;
  if (*max_element(h.begin(), h.end()) >= limits) {
    for (int i = 0; i < FIELD_WIDTH-1; i++) {
      if (h[i] >= 19) return 100000;
      // if(h[i] >= 10) return 114514;
      if (h[i] == m || h[i+1] == m) continue;
      hs += abs(h[i]-h[i+1]) + std::max(0, h[i]-limits)*20;
      // hs += (abs(h[i]-h[i+1])>=3?1:0);
    }
  } else {
    int mmm = *max_element(h.begin(), h.end());
    for (int i = 0; i < FIELD_WIDTH; i++) {
      if (mmm <= 17 && i >= 7 && h[i] > 0) return 11451400;
      if (i == FIELD_WIDTH-1) continue;
      if (i+1 >= 7) continue;
      // if(h[i] >= 10) return 114514;
      if (h[i] == m || h[i+1] == m) continue;
      hs += abs(h[i]-h[i+1]);
      // hs += (abs(h[i]-h[i+1])>=3?1:0);
    }
  }
  // sort(h.begin(),h.end());
  // return h.back()-h[0];
  return hs;
}

int64_t Onore::eval(const Field& f, int line, int len) const {
  int hs = evalHeight(f);
  int hole = evalHole(f);
  /*
    printf("h: ");
    for( int i = 0; i < FIELD_WIDTH; i++ ) {
    printf("%d ",f.height(i));
    }
    puts("");
    printf("t: ");
    for( int i = 0; i < FIELD_WIDTH; i++ ) {
    printf("%d ",f.top(i));
    }
    puts("");
    printf("%d %d\n",hs,hole);
  */
  return -(hole*1000+hs*10+(limits >= hlim?len*2000:0));
}

void Onore::reset() {
  plen = 0;
  limits = hlim;
  hold_.k = -1;
}

Decision Onore::think(const Field& f, const std::vector<MinoType>& s) {
  std::vector<int> h(FIELD_WIDTH);
  for (int i = 0; i < FIELD_WIDTH; i++) {
    h[i] = f.height(i);
  }
  int mh = *min_element(h.begin(), h.begin()+7);
  // int mh = *max_element(h.begin(),h.end());
  /*
    if( limits >= hlim ) {
    // if( mh >= 15 ) limits = 3;
    // else limits = 15;
    if( mh <= 3 ) limits = hlim;
    } else {
    if( mh >= hlim ) limits = 3;
    }
  */
  if (mh >= limits) {
    if (limits == 3 && mh <= 3) {
      limits = hlim;
    } else if (limits == hlim) {
      limits = 3;
    }
  }
  printf("lim:%d\n", limits);
  int64_t best = std::numeric_limits<int64_t>::min();
  // bool hakka=false;
  Decision d(-1, -1, -1, 0);
  // printf("kangaeru\n");
  std::vector<MinoType> v, w(s.size()-1);
  v.push_back(s[0]);
  v.push_back(s[1]);
  for (size_t i = 0; i < s.size()-1; i++) {
    w[i] = s[i+1];
  }
  std::priority_queue<State> state;
  bool prohibits[FIELD_WIDTH] = {};
  for (int i = FIELD_WIDTH-3; i < FIELD_WIDTH; i++) {
    prohibits[i] = (limits <= 16 && limits >= 13);
  }
  Simulator::iterate(f, v, [this, &state](const Field& f,
                                          const std::vector<Decision>& dc,
                                          int line,
                                          int len,
                                          const MinoType& hold) {
                       int64_t sc = eval(f, line, len);
                       State s(f, sc);
                       s.first = dc[0];
                       s.l.push_back(line);
                       s.r.push_back(len);
                       s.hold = hold;
                       state.push(s);
                     }, hold_, prohibits, plen);
  printf("sz:%zd\n", state.size());

  for (size_t i = 0; i+1 < w.size(); i+=2) {
    std::priority_queue<State> q;
    v[0] = w[i];
    v[1] = w[i+1];
    for (int j = 0; j < 5 && !state.empty(); j++) {
      Simulator::iterate(state.top().field, v,
                         [this, &q, &state](const Field& f,
                                            const std::vector<Decision>& dc,
                                            int line,
                                            int len,
                                            const MinoType& hold) {
                           int64_t sc = eval(f, line, len);
                           State s(f, sc);
                           s.first = state.top().first;
                           s.l = state.top().l;
                           s.r = state.top().r;
                           s.l.push_back(line);
                           s.r.push_back(len);
                           s.hold = hold;
                           q.push(s);
                         }, state.top().hold, prohibits, state.top().r.back());
      state.pop();
    }
    printf("szz:%zd\n", state.size());
    state = std::move(q);
  }
  int64_t akan_best = std::numeric_limits<int64_t>::min();
  Decision akan(-1, -1, -1, 0);
  assert(state.size() != 0);
  int pplen = plen;
  MinoType phold = hold_;
  std::vector<int> ls;

  while (!state.empty()) {
    const auto& a = state.top().l;
    const auto& b = state.top().r;
    int line = std::accumulate(a.begin(), a.end(), 0);
    int len = b.back();
    const Field& f = state.top().field;
    int64_t sc = eval(f, line, len);
    //*
    if (akan_best < sc) {
      akan_best = sc;
      akan = state.top().first;
      ls = b;
      if (akan.h) {
        hold_ = s[0];
      } else {
        hold_ = phold;
      }
      // plen = len;
      if (b[0] > 1) {
        plen = pplen+1;
      } else {
        plen = 0;
      }
    }
    state.pop();
  }
  printf("score:%" PRId64 " %" PRId64 " %d %d\n",
         best, akan_best, plen, ls[0]);
  assert(d.r >= 0 || akan.r >= 0);
  return akan;
  // if(d.r<0)return akan;
  // return d;
}

