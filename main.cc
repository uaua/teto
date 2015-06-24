#include <deque>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <windows.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "AI.h"

using namespace std;

static const int MINO_WIDTH = 18;
static const int MINO_HEIGHT = 18;
static const int BOUND_X = 97;
static const int BOUND_Y = 135;

struct  cell{
  HWND hWnd;
  char WindowName[256];
};

BOOL  CALLBACK  EnumWndProc( HWND hWnd, LPARAM lParam )
{
  char buff[256]="";
  GetWindowText( hWnd,buff, sizeof(buff));//ウインドウの文字を取得して、
  if(strcmp(buff,((cell*)lParam)->WindowName)==0){//名前が一致したら、
    ((cell*)lParam)->hWnd = hWnd;//ウィンドウハンドルを渡す
  }
  return true;
}

char img[1111*1111*20];

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
  switch(bpp) {
  case 1:
    return *p;
    break;

  case 2:
    return *(Uint16 *)p;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
    break;

  case 4:
    return *(Uint32 *)p;
    break;

  default:
    return 0;       /* shouldn't happen, but avoids warnings */
  }
}

SDL_Surface *load_image(char * buff,int size)
{
  SDL_RWops *rw = SDL_RWFromMem(buff,size);
  SDL_Surface *temp = IMG_LoadBMP_RW(rw);

  if (temp == NULL)
    {
      printf("IMG_Load_RW: %s\n", IMG_GetError());
      system("pause");
      exit(1);
    }

  //Convert the image to optimal display format
  SDL_Surface *myimage;
  myimage = SDL_ConvertSurfaceFormat(temp,SDL_PIXELFORMAT_RGBA8888,0);

  //Free the temporary surface
  SDL_FreeSurface(temp);

  //Return our loaded image
  return myimage;
}

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* tex;
SDL_Surface* suf;
TTF_Font* font;

int R(unsigned c){return (c>>24)&0xff;}
int G(unsigned c){return (c>>16)&0xff;}
int B(unsigned c){return (c>>8)&0xff;}

enum STATE {
  NAZO,READY,GO,FETCH_NEXT,ACTIVE,THINK,PUT1,
};
STATE state;

bool jdgReady()
{
  const int x = 306;
  const int y = 265;
  unsigned c = getpixel(suf,x,y);
  //if(c!=0x1a1a1aff)printf("%08x\n",c);
  //printf("%d %d %d\n",R(c),G(c),B(c));
  if( /*R(c) == 7 && G(c) == 60 && B(c) == 146*/ c == 0xa2b0beff ) {
    return true;
  }
  return false;
}

bool jdgGo()
{
  const int x = 203;
  const int y = 271;
  unsigned c = getpixel(suf,x,y);
  //if(c!=0xffffffff && c != 0x1a1a1aff ) printf("%08x\n",c);
  if( c == 0x611105ff ) {
    return true;
  }
  return false;
}

int jdgMino(unsigned c)
{
  if( c == 0x7619feff ) return 0;
  if( c == 0x0358f4ff ) return 1;
  if( c == 0xfe9800ff ) return 2;
  if( c == 0xfe3333ff ) return 3;
  if( c == 0xb1eb2aff ) return 4;
  if( c == 0x32d1ffff ) return 5;
  if( R(c) == 255 && G(c) == 210 && B(c) == 0 ) {
    return 6;
  }
  if( c == 0x616161ff ) return 7;
  return -1;
}

int next1()
{
  const int x = 337;
  const int y = 207;
  unsigned c = getpixel(suf,x,y);
  //printf("0x%08x\n",c);
  int res = jdgMino(c);
  if( res >= 0 ) return res;
  int xx = 308;
  int yy = 204;
  c = getpixel(suf,xx,yy);
  res = jdgMino(c);
  if( res >= 0 ) return res;
  return 6;
}

int next2()
{
  const int x=343;
  const int y=271;
  unsigned c = getpixel(suf,x,y);
  int res = jdgMino(c);
  if( res >= 0 ) return res;
  const int xx=339;
  const int yy=276;
  return jdgMino(getpixel(suf,xx,yy));
}

int next3()
{
  const int x=344;
  const int y=326;
  unsigned c = getpixel(suf,x,y);
  int res = jdgMino(c);
  if( res >= 0 ) return res;
  return 4;
  //const int xx=339;
  //const int yy=276;
  //return jdgMino(getpixel(suf,xx,yy));
}

int next4()
{
  const int x=344;
  const int y=326+50;
  unsigned c = getpixel(suf,x,y);
  int res = jdgMino(c);
  if( res >= 0 ) return res;
  return 4;
  //const int xx=339;
  //const int yy=276;
  //return jdgMino(getpixel(suf,xx,yy));
}

int next5()
{
  const int x=344;
  const int y=326+50*2;
  unsigned c = getpixel(suf,x,y);
  int res = jdgMino(c);
  if( res >= 0 ) return res;
  return 4;
  //const int xx=339;
  //const int yy=276;
  //return jdgMino(getpixel(suf,xx,yy));
}

int next6()
{
  const int x=344;
  const int y=326+50*3;
  unsigned c = getpixel(suf,x,y);
  int res = jdgMino(c);
  if( res >= 0 ) return res;
  return 4;
  //const int xx=339;
  //const int yy=276;
  //return jdgMino(getpixel(suf,xx,yy));
}

bool jdgOwari()
{
  const int x = 88;
  const int y = 427;
  unsigned c = getpixel(suf,x,y);
  //printf("0x%08x\n",c);
  if( c == 0x90cf46ff ) return true;
  return false;
}

bool jdgMinoAru(int x,int y)
{
  const int bx = 101;
  const int by = 136;
  unsigned c = getpixel(suf,bx+MINO_WIDTH*x+MINO_WIDTH/2,by+MINO_HEIGHT*y+MINO_HEIGHT/2);
  /*
  //printf("0x%08x\n",c);
  if( c != 0x1a1a1aff ) {
    //printf("o");
    return true;
  } else {
    printf(" ");
    return false;
  }
  */
  return jdgMino(c)>=0;
}

Field genField()
{
  Field f;
  for( int y = 0; y < FIELD_HEIGHT-1; y++ ) {
    for( int x = 0; x < FIELD_WIDTH; x++ ) {
      f.set(x,y+1,jdgMinoAru(x,y) ? 1 : 0);
    }
  }
  return f;
}

int nextmino=-1;
deque<int> nekumino;
AI ai;
Field f;

void KeyAction(WORD VirtualKey, BOOL bKeepPressing)
{
  INPUT input[1];
  input[0].type = INPUT_KEYBOARD;
  input[0].ki.wVk = VirtualKey;
  input[0].ki.wScan = MapVirtualKey(input[0].ki.wVk, 0);
  input[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
  input[0].ki.time = 0;
  input[0].ki.dwExtraInfo = ::GetMessageExtraInfo();
  ::SendInput(1, input, sizeof(INPUT));
  if (!bKeepPressing) {
    input[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
    ::SendInput(1, input, sizeof(INPUT));
  }
}

void go()
{
  static int cnt = 0;
  static int kazu = 0;
  static const int INPUT_INTERVAL=40;
  static Decision dc;
  static Field prevField,putField;
  
  switch(state) {
  case NAZO:
    if( jdgReady() ) {
      state = READY;
      nekumino.clear();
      nextmino = next1();
      ai.reset();
    }
    break;
    
  case READY:
    if( jdgGo() ) state = GO;
    break;
    
  case GO:
    if( jdgOwari() ) state = NAZO;
    kazu = 0;
    state = FETCH_NEXT;
    Sleep(200);
    break;
    
  case FETCH_NEXT:
    //puts("fetch");
    nekumino.clear();
    nekumino.push_back(nextmino);
    nekumino.push_back(next1());
    nekumino.push_back(next2());
    nekumino.push_back(next3());
    //nekumino.push_back(next4());
    //nekumino.push_back(next5());
    //nekumino.push_back(next6());
    nextmino=next1();
    state = THINK;
    if( jdgOwari() ) state = NAZO;
    break;
    
  case ACTIVE:
    break;
    
  case THINK:
    {
#ifdef DEBUG
      puts("think");
#endif
      Field f(kazu < 10 ? genField() : putField);
      bool match = true;
      for( int y = 0; y < FIELD_HEIGHT; y++ ) {
        for( int x = 0; x < FIELD_WIDTH; x++ ) {
          if( f.get(x,y).cell() != prevField.get(x,y).cell() ) {
            match = false;
          }
        }
      }
      if( !match && false ) {
        printf("x:%d r:%d next:%d\n",dc.x,dc.r,nekumino[0]);
        puts("cap: ");
        Printer(f,ai.hold).print();
        puts("pre: ");
        //Printer::print(prevField);
        puts("put: ");
        //Printer::print(putField);
        static int file=0;
        if( file < 50 ) {
          char filename[256];
          sprintf(filename,"%d.bmp",file++);
          SDL_SaveBMP(suf,filename);
        }
      }
      static int file=0;
      if( file < 50 ) {
        char filename[256];
        sprintf(filename,"%d.bmp",file++);
        SDL_SaveBMP(suf,filename);
      }
      vector<MinoType> s;
      s.reserve(11);
      //ai.think();
      for( size_t i = 0; i < nekumino.size(); i++ ) {
        s.emplace_back(nekumino[i]);
      }
      Printer(f,s,ai.hold).print();
      int preHold = ai.hold.k;
      dc = ai.think(f,s);
      if( dc.h && preHold == -1 ) nextmino = next2();
      prevField = f;
      printf("x:%d r:%d next:%d\n",dc.x,dc.r,nekumino[0]);
      //Simulator::putMino(f,d.h?(pre<0?seq[1]:pre):seq[0],d);
      nekumino[0] = dc.h?(preHold<0?s[1].k:preHold):s[0].k;
      Simulator::putMino(f,nekumino[0],dc);
      putField = f;
      state = PUT1;
    }
#ifdef DEBUG
    puts("thikend");
#endif
    break;
    
  case PUT1:
#ifdef DEBUG
    puts("put1");
#endif
    state = FETCH_NEXT;
    if( cnt++ >= 1000 ) {
      cnt = 0;
    }
    if( dc.h ) {
      KeyAction(VK_SHIFT, true);
      Sleep(INPUT_INTERVAL);
      KeyAction(VK_SHIFT, false);
      Sleep(INPUT_INTERVAL);
    }
    for( int i = 0; i < dc.r; i++ ) {
      KeyAction(VK_UP, true);
      Sleep(INPUT_INTERVAL);
      KeyAction(VK_UP, false);
      Sleep(INPUT_INTERVAL);
    }
    for( int i = dc.x-(nekumino[0]==6?1:0); i < 3; i++ ) {
      KeyAction(VK_LEFT, true);
      Sleep(INPUT_INTERVAL);
      KeyAction(VK_LEFT, false);
      Sleep(INPUT_INTERVAL);
    }
    for( int i = 3+(nekumino[0]==6?1:0); i < dc.x; i++ ) {
      KeyAction(VK_RIGHT, true);
      Sleep(INPUT_INTERVAL);
      KeyAction(VK_RIGHT, false);
      Sleep(INPUT_INTERVAL);
    }
    KeyAction(VK_SPACE, true);
    Sleep(INPUT_INTERVAL);
    KeyAction(VK_SPACE, false);
    //Sleep(350); 
    Sleep(350); 
#ifdef DEBUG
    puts("put2");
#endif
    break;
  }
}

void sim(HWND hwnd)
{
  RECT rec;
  int dwWidth, dwHeight;
  HDC hdc, hdcMem;
  HBITMAP hBMP;
  int dwLength, dwFSize;
  LPBYTE lpBuf, lpPixel;
  LPBITMAPFILEHEADER lpHead;
  LPBITMAPINFOHEADER lpInfo;
  bool quit = false;
  SDL_Event event;
  SetForegroundWindow(hwnd);
  //state = FETCH_NEXT;
  while (!quit) {
    /* Check for new events */
    while(SDL_PollEvent(&event)) {
      /* If a quit event has been sent */
      if (event.type == SDL_QUIT) {
        /* Quit the application */
        quit = true;
      }
    }
    GetClientRect(hwnd,&rec); /* クライアント領域取得 */
    
    dwWidth=rec.right;
    dwHeight=rec.bottom;
    /* ウインドウのデバイスコンテキスト取得 */
    hdc=GetDC(hwnd);
    /* ウインドウのデバイスコンテキスト互換のBITMAP作成 */
    hBMP=CreateCompatibleBitmap(hdc,dwWidth,dwHeight);
  
    /* BITMAPにウインドウのクライアント領域をコピー */
    hdcMem=CreateCompatibleDC(hdc);
    SelectObject(hdcMem,hBMP);
    BitBlt(hdcMem,0,0,dwWidth,dwHeight,hdc,0,0,SRCCOPY);
  
    if ((dwWidth*4) % 4==0) /* バッファの１ラインの長さを計算 */
      dwLength=dwWidth*4;
    else
      dwLength=dwWidth*4+(4-(dwWidth*4) % 4);

    /* 書き込み用バッファのサイズ計算 */
    dwFSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwLength*dwHeight;

    /* バッファ確保とポインタ設定 */
    lpBuf=(LPBYTE)GlobalAlloc(GPTR,dwFSize);
    lpHead=(LPBITMAPFILEHEADER)lpBuf;
    lpInfo=(LPBITMAPINFOHEADER)(lpBuf+sizeof(BITMAPFILEHEADER));
    lpPixel=lpBuf+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

    /* 24ビットBMPファイルのヘッダ作成 */
    lpHead->bfType='M'*256+'B';
    lpHead->bfSize=dwFSize;
    lpHead->bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    lpInfo->biSize=sizeof(BITMAPINFOHEADER);
    lpInfo->biWidth=dwWidth;
    lpInfo->biHeight=dwHeight;
    lpInfo->biPlanes=1;
    lpInfo->biBitCount=32;

    GetDIBits(hdc,hBMP,0,dwHeight,lpPixel,(LPBITMAPINFO)lpInfo,DIB_RGB_COLORS);

    ReleaseDC(hwnd,hdc);
    DeleteObject(hBMP);
    DeleteObject(hdcMem);
    suf = load_image((char*)lpBuf,  dwFSize);
    GlobalFree(lpBuf);
    //SDL_SaveBMP(suf,"dekita.bmp");

    go();
    //static char str[256];
    static SDL_Rect rect, scr_rect;
    //sprintf(str,"%d %d",state,nextmino);
    stringstream ss;
    ss << state << ":" << nextmino << ":";
    for( size_t i = 0; i < nekumino.size(); i++ ) {
      ss << nekumino[i] << ":";
    }
    ss << next4() << ":" << next5() << ":" << next6();
    //printf("%s\n",ss.str().c_str());
#ifdef USE_GUI
    tex = SDL_CreateTextureFromSurface(renderer, suf);
    SDL_FreeSurface(suf);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    static SDL_Color white = {0x00, 0x00, 0x00, 0xff};
    SDL_Surface* image = TTF_RenderUTF8_Blended(font, ss.str().c_str(), white);

    rect.x = 0;
    rect.y = 0;
    rect.w = image->w;
    rect.h = image->h;

    scr_rect = rect;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, &rect, &scr_rect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(image);
#else
    SDL_FreeSurface(suf);
#endif
  }
}

#undef main

void test()
{
  suf = IMG_Load("tet1.png");
  if( !suf ) {
    puts("err");
    return;
  }
  Field f(genField());
  //Printer()::print();
  SDL_FreeSurface(suf);
}

int main(int argc, char* argv[])
{
  cell c;

  //test();
  
  c.hWnd =NULL;
  strcpy(c.WindowName,"TetrisOnline");//検索するウィンドウの名前

  EnumWindows( EnumWndProc, (LPARAM)&c);

  if(c.hWnd != NULL){
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
#ifdef USE_GUI
    window=SDL_CreateWindow("teto",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,SDL_WINDOW_OPENGL);
    font = TTF_OpenFont("mikachan-p.ttf", 24);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
#endif
    sim(c.hWnd);
#ifdef USE_GUI
    SDL_DestroyWindow(window);
#endif
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
  } else {
    puts("can't get hwnd");
  }
  return 0;
}
