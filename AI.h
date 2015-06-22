#include <algorithm>
#include <cassert>
#include <queue>
#include <Windows.h>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <array>
#include <functional>
#include <vector>

using namespace std;

static const int FIELD_HEIGHT = 21;
static const int FIELD_WIDTH = 10;

struct Cell {
  Cell(){cell_=0;}
  int cell()const{ return cell_; }
  void setCell(int cell){ cell_ = cell; }
private:
  int cell_;
};

class Mino {
public:
  const vector<vector<vector<int>>> s_;
  Mino(const vector<vector<vector<int>>>& s):s_(s){
    //printf("%zd %zd %zd\n",s_.size(),s_[0].size(),s_[0][0].size());
  }
  int size()const{return s_.size();}
  vector<vector<int>> rotate(int r) const {
    return s_[r];
  }
};

struct MinoType {
  int k;
  MinoType(){}
  MinoType(int k):k(k){}
};

class Field {
public:
  Field(){
    clear();
  }
  Field(const Field& f){
    memcpy(field_,f.field_,sizeof(f.field_));
  }
  void clear() {
    for( int i = 0; i < FIELD_HEIGHT; i++ ) {
      for( int j = 0; j < FIELD_WIDTH; j++ ) field_[i][j].setCell(0);
    }
  }
  Cell get(int x,int y)const{return field_[y][x];}
  void set(int x,int y,int v) {
    field_[y][x].setCell(v);
  }
  int top(int x) const {
    for( int i = 0; i < FIELD_HEIGHT; i++ ) {
      if( field_[i][x].cell() ) return i;
    }
    return FIELD_HEIGHT;
  }
  int height(int x) const {
    for( int i = 0; i < FIELD_HEIGHT; i++ ) {
      if( field_[i][x].cell() ) return FIELD_HEIGHT-i;
    }
    return 0;
  }
private:
  Cell field_[FIELD_HEIGHT][FIELD_WIDTH];
};

static const vector<Mino> mino = {
  Mino({
      {{0,1,0},{1,1,1},{0,0,0}},
      {{0,1,0},{0,1,1},{0,1,0}},
      {{0,0,0},{1,1,1},{0,1,0}},
      {{0,1,0},{1,1,0},{0,1,0}},
    }),
  Mino({
      {{1,0,0},{1,1,1},{0,0,0}},
      {{0,1,1},{0,1,0},{0,1,0}},
      {{0,0,0},{1,1,1},{0,0,1}},
      {{0,1,0},{0,1,0},{1,1,0}}
    }),
  Mino({
      {{0,0,1},{1,1,1},{0,0,0}},
      {{0,1,0},{0,1,0},{0,1,1}},
      {{0,0,0},{1,1,1},{1,0,0}},
      {{1,1,0},{0,1,0},{0,1,0}}
    }),
  Mino({
      {{1,1,0},{0,1,1},{0,0,0}},
      {{0,0,1},{0,1,1},{0,1,0}},
        /*
      {{0,0,0},{1,1,0},{0,1,1}},
      {{0,1,0},{1,1,0},{1,0,0}},
        */
    }),
  Mino({
      {{0,1,1},{1,1,0},{0,0,0}},
      {{0,1,0},{0,1,1},{0,0,1}},
        /*
      {{0,0,0},{0,1,1},{1,1,0}},
      {{1,0,0},{1,1,0},{0,1,0}},
        */
    }),
  Mino({
      {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
      {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
        /*
      {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},
      {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}},
        */
    }),
  Mino({{
      {1,1},
      {1,1}
      }})
};

class Printer {
private:
  const Field& f;
  MinoType hold;
  vector<MinoType> seq;
public:
 Printer(const Field& f,const MinoType hold) : f(f),hold(hold),seq(0) {
  }
  Printer(const Field& f,const vector<MinoType>& seq,const MinoType hold) : f(f),hold(hold),seq(seq) {
  }
  void print() const { 
    const int H=FIELD_HEIGHT+10;
    const int W=FIELD_WIDTH+30;
    char s[H][W];
#ifdef DEBUG
    puts("print");
#endif
    memset(s,' ',sizeof(s));
    for( int y = 0; y < FIELD_HEIGHT; y++ ) {
      //printf("#");
      s[y][0] = '#';
      for( int x = 0; x < FIELD_WIDTH; x++ ) {
        if(f.get(x,y).cell()) {
          //printf("o");
          s[y][x+1] = 'o';
        } else {
          //printf(" ");
          s[y][x+1] = ' ';
        }
      }
      s[y][FIELD_WIDTH+1] = '#';
      //puts("#");
    }
    if(hold.k>=0){
      const auto& a = mino[hold.k].rotate(0);
      for( int i = 0; i < int(a.size()); i++ ) {
        for( int j = 0; j < int(a[i].size()); j++ ) {
          s[1+i][FIELD_WIDTH+3+9+j] = a[i][j] ? 'o' : ' ';
        }
      }
    }
    for( int ii = 0; ii < int(seq.size()); ii++ ) {
      const auto& a = mino[seq[ii].k].rotate(0);
      for( int i = 0; i < int(a.size()); i++ ) {
        for( int j = 0; j < int(a[i].size()); j++ ) {
          s[1+ii*3+i][FIELD_WIDTH+3+j] = a[i][j] ? 'o' : ' ';
        }
      }
    }
    for( int i = 0; i < FIELD_WIDTH+2; i++ ) {
      //puts("############");
      s[FIELD_HEIGHT][i] = '#';
    }
    for( int y = 0; y < H; y++ ) {
      for( int x = 0; x < W; x++ ) {
        putchar(s[y][x]);
      }
      puts("");
    }
#ifdef DEBUG
    puts("endprint");
#endif
  }
};



class Decision {
public:
  int x,r,y,h;
  Decision(){}
  Decision(int x,int r,int y,int h):x(x),r(r),y(y),h(h){}
};

class Simulator {
public:
  typedef std::function<void(const Field&,const std::vector<Decision>&,int,int,const MinoType&)> Callback;
  static void iterate(const Field& f,const vector<MinoType>& s,const Callback& callback,const MinoType& hold,int len=0) {
    vector<MinoType> ms(s);
    //printf("%zd %zd\n",s.size(),ms.size());
    iterateInternal(f,ms,callback,0,vector<Decision>(),hold,0,len);
  }
  static int putMino(Field& f,const MinoType& ss,const Decision& d) {
    const MinoType& s = ss;
    int msz = mino[s.k].rotate(d.r).size();
    int line = 0;
    const auto& v = mino[s.k].rotate(d.r);
    for( int i = 0; i < msz; i++ ) {
      for( int j = 0; j < msz; j++ ) {
        if( v[i][j] ) f.set(d.x+j,d.y+i,1);
      }
    }
    for( int i = FIELD_HEIGHT-1; i >= 0; i-- ) {
      bool fill = true;
      for( int j = 0; j < FIELD_WIDTH; j++ ) {
        if( !f.get(j,i).cell() ) fill = false;
      }
      if( fill ) {
        line += 1;
        for( int j = 0; j < FIELD_WIDTH; j++ ) {
          f.set(j,i,0);
        }
        bool done = true;
        for( int ii = i; ii > 0; ii-- ) {
          for( int j = 0; j < FIELD_WIDTH; j++ ) {
            f.set(j,0,0);
            if( f.get(j,ii-1).cell() ) done = false;
            f.set(j,ii,f.get(j,ii-1).cell());
          }
        }
        i++;
        if( done ) goto owari;
      }
    }
  owari:;
    return line;
  }
  
private:
  
  static void iterateInternal(const Field& f,vector<MinoType>& s,const Callback& callback,int depth,vector<Decision> d,const MinoType& hold,int line,int len,bool preHold=false) {
    //printf("%d %zd %d %d\n",depth,s.size(),preHold,hold.k);
    if( depth == int(s.size()) ) {
      callback(f,d,line,len,hold);
    } else {
      int sz = mino[s[depth].k].size();
      int msz = mino[s[depth].k].rotate(0).size();
      for( int x = -msz; x < FIELD_WIDTH+msz; x++ ) {
        for( int r = 0; r < sz; r++ ) {
          const auto& v = mino[s[depth].k].rotate(r);
          int pos = -1;
          Field next(f);
          for( int y = 0; y < FIELD_HEIGHT; y++ ) {
            for( int i = 0; i < msz; i++ ) {
              for( int j = 0; j < msz; j++ ) {
                if( v.at(i).at(j) &&
                    ( x+j < 0 || x+j >= FIELD_WIDTH || y+i<0 || y+i >= FIELD_HEIGHT ||
                      f.get(x+j,y+i).cell())) goto next;
              }
            }
            pos = y;
          }
        next:;
          if( pos >= 0 ) {
            /*
            for( int i = 0; i < msz; i++ ) {
              for( int j = 0; j < msz; j++ ) {
                if( v[i][j] ) next.set(x+j,pos+i,1);
              }
            }
            */
            //puts("iei");
            Decision dc(x,r,pos,preHold);
            //assert((!preHold && hold.k<0) || (preHold && hold.k>=0));
            int l = putMino(next,s[depth],dc);
            d.push_back(dc);
            iterateInternal(next,s,callback,depth+1,d,hold,line+l,l>0?len+1:0,false);
            d.pop_back();
          }
        }
      }
      if( !preHold ) {
        Decision dc;
        //dc.h = 1;
        //d.push_back(dc);
        if( hold.k<0 && depth+1 < int(s.size()) ) {
          MinoType nextHold = s[depth];
          iterateInternal(f,s,callback,depth+1,d,nextHold,line,len,true);
        } else if(hold.k>=0) {
          MinoType nextHold = hold;
          swap(s[depth],nextHold);
          iterateInternal(f,s,callback,depth,d,nextHold,line,len,true);
          swap(s[depth],nextHold);
        }
      }
    }
  }
};

class AI {
private:
  int limits;
  int evalHeight(const Field& f) const {
    vector<int> h(FIELD_WIDTH);
    for( int i = 0; i < FIELD_WIDTH; i++ ) {
      h[i] = f.height(i);
    }
    int hs = 0;
    //int m = *min_element(h.begin(),h.end());
    int m = -114514;
    if( *max_element(h.begin(),h.end()) >= limits ) {
      for( int i = 0; i < FIELD_WIDTH-1; i++ ) {
        if( h[i] >= 19 ) return 100000;
        //if( h[i] >= 10 ) return 114514;
        if( h[i] == m || h[i+1] == m ) continue;
        hs += abs(h[i]-h[i+1]) + max(0,h[i]-limits)*20;
        //hs += (abs(h[i]-h[i+1])>=3?1:0);
      }
    } else {
      for( int i = 0; i < FIELD_WIDTH; i++ ) {
        if( i >= 7 && h[i] > 0 ) return 11451400;
        if( i == FIELD_WIDTH-1 ) continue;
        //if( h[i] >= 10 ) return 114514;
        if( h[i] == m || h[i+1] == m ) continue;
        hs += abs(h[i]-h[i+1]);
        //hs += (abs(h[i]-h[i+1])>=3?1:0);
      }
    }
    //sort(h.begin(),h.end());
    //return h.back()-h[0];
    return hs;
  }
  
  int evalHole(const Field& f) const {
    vector<int> t(FIELD_WIDTH);
    for( int i = 0; i < FIELD_WIDTH; i++ ) {
      t[i] = f.top(i);
    }
    int hole = 0;
    for( int i = 0; i < FIELD_WIDTH; i++ ) {
      for( int y = t[i]; y < FIELD_HEIGHT; y++ ) {
        if( !f.get(i,y).cell() ) hole += 1;
      }
    }
    return hole;
  }

  long long eval(const Field& f,int line,int len) const {
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
    return -(hole*1000+hs*10+(limits>=hlim?len*2000:0));
  }


public:
  const int hlim = 13;
  MinoType hold;
  AI(){reset();}
  void reset() {
    plen=limits=0;limits=hlim;
    hold.k = -1;
  }
  int plen;
  struct State {
    Field field;
    long long score;
    Decision first;
    MinoType hold;
    vector<int> l,r;
    State(const Field& field_,long long score_):field(field_),score(score_){}
    bool operator<(const State& e) const {
      return score<e.score;
    }
  };
  
  Decision think(const Field& f,const vector<MinoType>& s) {
    vector<int> h(FIELD_WIDTH);
    for( int i = 0; i < FIELD_WIDTH; i++ ) {
      h[i] = f.height(i);
    }
    int mh = *min_element(h.begin(),h.begin()+7);;
    //int mh = *max_element(h.begin(),h.end());
    /*
    if( limits >= hlim ) {
      //if( mh >= 15 ) limits = 3;
      //else limits = 15;
      if( mh <= 3 ) limits = hlim;
    } else {
      if( mh >= hlim ) limits = 3;
    }
    */
    if( mh >= limits ) {
      if( limits == 3 && mh <= 3 ) {
        limits = hlim;
      } else if( limits == hlim ) {
        limits = 3;
      }
    }
    printf("lim:%d\n",limits);
    long long best = numeric_limits<long long>::min();
    //bool hakka=false;
    Decision d(-1,-1,-1,0);
    //printf("kangaeru\n");
    vector<MinoType> v,w(s.size()-1);
    v.push_back(s[0]);
    v.push_back(s[1]);
    for( size_t i = 0; i < s.size()-1; i++ ) {
      w[i] = s[i+1];
    }
    priority_queue<State> state;
    Simulator::iterate(f,v,[&](const Field& f,const vector<Decision>& dc,int line,int len,const MinoType& hold){
        long long sc = eval(f,line,len);
        State s(f,sc);
        s.first = dc[0];
        s.l.push_back(line);
        s.r.push_back(len);
        s.hold = hold;
        state.push(s);
      },hold,plen);
    printf("sz:%zd\n",state.size());

    for( int i = 0; i < int(w.size()-1); i+=2 ) {
      priority_queue<State> q;
      v[0] = w[i];
      v[1] = w[i+1];
      for( int j = 0; j < 5 && state.size() > 0; j++ ) {
        Simulator::iterate(state.top().field,v,[&](const Field& f,const vector<Decision>& dc,int line,int len,const MinoType& hold){
            long long sc = eval(f,line,len);
            State s(f,sc);
            s.first = state.top().first;
            s.l = state.top().l;
            s.r = state.top().r;
            s.l.push_back(line);
            s.r.push_back(len);
            s.hold = hold;
            q.push(s);
          },state.top().hold,state.top().r.back());
        state.pop();
      }
      printf("szz:%zd\n",state.size());
      state = std::move(q);
    }
    long long akan_best=numeric_limits<long long>::min();
    Decision akan(-1,-1,-1,0);
    assert(state.size() != 0);
    int pplen=plen;
    MinoType phold = hold;
    while(!state.empty()) {
      const auto& a = state.top().l;
      const auto& b = state.top().r;
      int line = accumulate(a.begin(),a.end(),0);
      int len = b.back();
      const Field& f = state.top().field;
      long long sc = eval(f,line,len);
      //*
      if( akan_best < sc ) {
        akan_best = sc;
        akan = state.top().first;
        if(akan.h) hold = s[0];
        else hold = phold;
        //plen = len;
        if( a[0] > 0 ) plen = pplen+1;
        else plen = 0;
      }
      state.pop();
    }
    printf("score:%lld %lld %d\n",best,akan_best,plen);
    assert(d.r>=0||akan.r>=0);
    if(d.r<0)return akan;
    return d;
  }
};

