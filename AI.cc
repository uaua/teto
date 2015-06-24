#include <time.h>
#include "AI.h"

using namespace std;

int main(void)
{
  int sz = 7;
  int S=10000;
  vector<MinoType> seq(4),seqs(S*3);
  srand(time(0));
  for( int j = 0; j < S*3; j++ ) {
    seqs[j] = MinoType(rand()%sz);
  }
  Field f;
  int pat = 0;
  AI ai;
  Decision d;
  //for( int i = 0; i < 10; i++ ) printf("%d ",seqs[i].k); puts("");
  for( int i = 0; i < S; i++ ) {
    // printf(":%d: ",ai.hold.k);
    for( size_t j = 0; j < seq.size(); j++ ) {
      seq[j] = seqs[i+j];
      //printf("%d ",seq[j].k);
    }
    //puts("");
    int pre=ai.hold.k;
    Printer(f,seq,ai.hold).print();
    d = ai.think(f,seq);
    //printf("%d %d %d %d %d\n",d.x,d.r,d.y,d.h,ai.hold.k);
    if( d.r >= 0 ) {
      if( pre<0 && ai.hold.k>=0 ) i++;
      Simulator::putMino(f,d.h?(pre<0?seq[1]:pre):seq[0],d);
      //printf("x:%d r:%d hold:%d\n",d.x,d.r,ai.hold.k);
      //printf("%d\n",ai.plen);
      Sleep(1000);
    } else {
      break;
    }
  }
  return 0;
}
