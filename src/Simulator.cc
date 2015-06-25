#include "Simulator.h"

#include "Decision.h"
#include "Field.h"
#include "Mino.h"
#include "MinoType.h"

void Simulator::iterate(const Field& f,
                        const std::vector<MinoType>& s,
                        const Callback& callback,
                        const MinoType& hold,
                        bool prohibits[],
                        int len) {
  std::vector<MinoType> ms(s);
  // printf("%zd %zd\n",s.size(),ms.size());
  iterateInternal(f, ms, callback, 0, std::vector<Decision>(), hold,
                  prohibits, 0, len, false);
}

int Simulator::putMino(Field* f, const MinoType& ss, const Decision& d) {
  const MinoType& s = ss;
  int msz = mino[s.k].rotate(d.r).size();
  int line = 0;
  const auto& v = mino[s.k].rotate(d.r);
  for (int i = 0; i < msz; i++) {
    for (int j = 0; j < msz; j++) {
      if (v[i][j]) f->set(d.x+j, d.y+i, 1);
    }
  }
  for (int i = FIELD_HEIGHT-1; i >= 0; i--) {
    bool fill = true;
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (!f->get(j, i).cell()) fill = false;
    }
    if (fill) {
      line += 1;
      for (int j = 0; j < FIELD_WIDTH; j++) {
        f->set(j, i, 0);
      }
      bool done = true;
      for (int ii = i; ii > 0; ii--) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
          f->set(j, 0, 0);
          if (f->get(j, ii-1).cell()) done = false;
          f->set(j, ii, f->get(j, ii-1).cell());
        }
      }
      i++;
      if (done) goto owari;
    }
  }
 owari:
  return line;
}

void Simulator::iterateInternal(const Field& f,
                                const std::vector<MinoType>& s,
                                const Callback& callback,
                                int depth,
                                std::vector<Decision> d,
                                const MinoType& hold,
                                bool prohibits[],
                                const int line,
                                const int len,
                                bool preHold) {
  // printf("%d %zd %d %d\n",depth,s.size(),preHold,hold.k);
  if (depth == static_cast<int>(s.size())) {
    callback(f, d, line, len, hold);
  } else {
    int sz = mino[s[depth].k].size();
    int msz = mino[s[depth].k].rotate(0).size();
    for (int x = -msz; x < FIELD_WIDTH+msz; x++) {
      for (int r = 0; r < sz; r++) {
        const auto& v = mino[s[depth].k].rotate(r);
        int pos = -1;
        Field next(f);
        for (int y = 0; y < FIELD_HEIGHT; y++) {
          for (int i = 0; i < msz; i++) {
            for (int j = 0; j < msz; j++) {
              if (v[i][j] &&
                  (x+j < 0 || x+j >= FIELD_WIDTH || y+i<0 ||
                   y+i >= FIELD_HEIGHT || f.get(x+j, y+i).cell())) {
                if (prohibits[x+j]) pos = -1;
                goto next;
              }
            }
          }
          pos = y;
        }
      next:
        if (pos >= 0) {
          /*
            for( int i = 0; i < msz; i++ ) {
            for( int j = 0; j < msz; j++ ) {
            if( v[i][j] ) next.set(x+j,pos+i,1);
            }
            }
          */
          // puts("iei");
          Decision dc(x, r, pos, preHold);
          // assert((!preHold && hold.k<0) || (preHold && hold.k>=0));
          int l = putMino(&next, s[depth], dc);
          d.push_back(dc);
          iterateInternal(next, s, callback, depth+1, d, hold, prohibits,
                          line+l, l > 0?len+1:0, false);
          d.pop_back();
        }
      }
    }
    if (!preHold) {
      Decision dc;
      // dc.h = 1;
      // d.push_back(dc);
      if (hold.k < 0 && depth+1 < static_cast<int>(s.size())) {
        MinoType nextHold = s[depth];
        iterateInternal(f, s, callback, depth+1, d, nextHold, prohibits, line,
                        len, true);
      } else if (hold.k >= 0) {
        MinoType nextHold = hold;
        std::vector<MinoType> nextSeq(s);
        std::swap(nextSeq[depth], nextHold);
        iterateInternal(f, s, callback, depth, d, nextHold, prohibits, line,
                        len, true);
        std::swap(nextSeq[depth], nextHold);
      }
    }
  }
}

