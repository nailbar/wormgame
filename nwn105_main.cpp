#include <windows.h>
#include <math.h>
#define class_name "daClass"

/******* My stuff *******/
#define torad 0.017453292519943295769236907684886
#define WIN_WID 640
#define WIN_HGT 480
#define mtail 100
struct vec{
  float x,y;
  vec(){x=0.0; y=0.0;}
  vec(float vx, float vy){x=vx; y=vy;}
  vec operator-(vec pnt){return vec(x-pnt.x,y-pnt.y);}
  vec operator+(vec pnt){return vec(x+pnt.x,y+pnt.y);}
  vec operator*(float d){return vec(x*d,y*d);}
};
vec getcpnt(vec ln1,vec ln2,vec pnt){
  float dis1=sqrt((ln1.x-ln2.x)*(ln1.x-ln2.x)+(ln1.y-ln2.y)*(ln1.y-ln2.y));
  vec nrm1=vec((ln2.x-ln1.x)/dis1,(ln2.x-ln1.x)/dis1);
  vec pnt1=pnt-ln1;
  float dis2=nrm1.x*pnt1.x+nrm1.y*pnt1.y;
  if(dis2<0) return ln1;
  if(dis2>dis1) return ln2;
  return ln1+nrm1*dis2;
}
class head{
  public:
  vec p,n,s;
  float r;
  int idl,lloss;
  bool aimode;
  head();
  ~head();
  void change(vec as,float ar);
  void move();
};
head::head(){
  p.x=rand()%WIN_WID;
  p.y=rand()%WIN_HGT;
  float s1=rand()%3600*torad*.1;
  n.x=cos(s1);
  n.y=sin(s1);
  r=0.0;
  idl=400;
  lloss=0;
}
head::~head(){}
void head::change(vec as,float ar){
  r+=ar;
  s=s+as;
}
void head::move(){
  p=p+s;
  n=vec(n.x*cos(r)-n.y*sin(r),n.x*sin(r)+n.y*cos(r));
  s=s*.95;
  r=r*.9;
  if(p.x<0){
    s.x=0-s.x;
    p.x=0;
    r+=s.y*.05;
  }
  if(p.x>640){
    s.x=0-s.x;
    p.x=640;
    r-=s.y*.05;
  }
  if(p.y<0){
    s.y=0-s.y;
    p.y=0;
    r-=s.x*.05;
  }
  if(p.y>480){
    s.y=0-s.y;
    p.y=480;
    r+=s.x*.05;
  }
}
class tail{
  public:
  vec p,n;
  float d;
  tail();
  ~tail();
  void follow(vec dp);
};
tail::tail(){  
  p.x=rand()%WIN_WID;
  p.y=rand()%WIN_HGT;
  float s1=rand()%3600*torad*.1;
  n.x=cos(s1);
  n.y=sin(s1);
  d=15;
}
tail::~tail(){}
void tail::follow(vec dp){
  vec v1=dp-p;
  float s1=sqrt(v1.x*v1.x+v1.y*v1.y);
  v1.x/=s1;
  v1.y/=s1;
  p=dp-v1*d;
  n=v1;
}
float getx(vec dn,vec dp){
  return dn.x*dp.x-dn.y*dp.y;
       //dn.x*dp.x-dn.y*dp.y
}
float gety(vec dn,vec dp){
  return dn.y*dp.x+dn.x*dp.y;
       //dn.y*dp.x+dn.x*dp.y
}
/************************/
void ChangeToFullScreen(int width,int height){
	DEVMODE dmSettings;
	memset(&dmSettings,0,sizeof(dmSettings));
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings)){
		MessageBox(NULL,"Could Not Enum Display Settings","Error",MB_OK);
		return;
	}
	dmSettings.dmPelsWidth=width;
	dmSettings.dmPelsHeight=height;
	int result=ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);
	if(result!=DISP_CHANGE_SUCCESSFUL){
		MessageBox(NULL,"Display Mode Not Compatible","Error",MB_OK);
		PostQuitMessage(0);
	}
}
struct SDBuffer{
	HWND win_hwnd;
	HDC win_dc;
	HDC back_dc;
	RECT rect;
	HBITMAP back_bmp;
	HBITMAP old_bmp;
};
bool InitDoubleBuffer(SDBuffer &doubleBuff);
void FreeDoubleBuffer(SDBuffer &doubleBuff);
bool LockFrameRate(int frame_rate=50);
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprev, PSTR cmdline, int ishow){
  /******* My stuff *******/
  srand(55);
  head a,c;
  tail b[mtail],d[mtail];
  POINT tpol[4];
  vec food=vec(rand()%WIN_WID,rand()%WIN_HGT);
  int aaa=mtail;
  int ccc=mtail;
  vec tmpnrm,tnrm2,tmppnt;
  float tmpdis;
  int hit;
  /************************/
  HWND hwnd;
  MSG msg;
  WNDCLASSEX wndclassex={0};
  DWORD dwStyle;
  SDBuffer doubleBuff={0};
  
  wndclassex.cbSize=sizeof(WNDCLASSEX);
  wndclassex.style=CS_HREDRAW | CS_VREDRAW;
  wndclassex.lpfnWndProc=WinProc;
  wndclassex.hInstance=hinstance;
  wndclassex.hIcon=LoadIcon(NULL, IDI_APPLICATION);
  wndclassex.hCursor=LoadCursor(NULL, IDC_ARROW);
  wndclassex.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
  wndclassex.lpszClassName=class_name;
  RegisterClassEx(&wndclassex);
  dwStyle=WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	ChangeToFullScreen(WIN_WID, WIN_HGT);
	ShowCursor(FALSE);
	
  hwnd=CreateWindowEx(NULL, class_name, "BackSimWin", dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, WIN_WID, WIN_HGT, NULL, NULL, hinstance, NULL);
  if(!hwnd){
    UnregisterClass(class_name,hinstance);
    return EXIT_FAILURE;
  }
  doubleBuff.win_hwnd=hwnd;
  if(!InitDoubleBuffer(doubleBuff)){
    UnregisterClass(class_name,hinstance);
    return EXIT_FAILURE;
  }
  HDC img_dc=CreateCompatibleDC(doubleBuff.win_dc);
  if(!img_dc){
    UnregisterClass(class_name,hinstance);
    return EXIT_FAILURE;
  }
  
  HPEN hpen;
  HPEN old_pen;
  PAINTSTRUCT ps;
  hpen=CreatePen(PS_SOLID,1,RGB(0,0,0));
  old_pen=(HPEN)SelectObject(img_dc,hpen);
  
  ShowWindow(hwnd, ishow);
  UpdateWindow(hwnd);
    
  while(1){
    if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
      if(msg.message == WM_QUIT) break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else if(LockFrameRate()){
      FillRect(doubleBuff.back_dc,&doubleBuff.rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
      
      /******* My stuff *******/
      if(GetKeyState(VK_ESCAPE)&0x80){
        ChangeDisplaySettings(NULL, 0);
        PostQuitMessage(0);
      }
      if(a.idl<500) a.idl++;
      a.change(vec(a.n.x*.3,a.n.y*.3),0);
      if(GetKeyState(VK_DOWN)&0x80){
        a.change(vec(a.n.x*-.3,a.n.y*-.3),0);
        a.idl=0;
      }
      if(GetKeyState(VK_LEFT)&0x80){
        a.change(vec(0,0),-.01);
        a.idl=0;
      }
      if(GetKeyState(VK_RIGHT)&0x80){
        a.change(vec(0,0),.01);
        a.idl=0;
      }
      tmpdis=sqrt((a.p.x-food.x)*(a.p.x-food.x)+(a.p.y-food.y)*(a.p.y-food.y));
      if(rand()%20<1) a.aimode=rand()%2;
      if(rand()%10<1) a.aimode=1;
      if(a.idl==500 && tmpdis>0 && a.aimode){
        tmpnrm=vec((food.x-a.p.x)/tmpdis,(food.y-a.p.y)/tmpdis);
        tnrm2=vec(-a.n.y,a.n.x);
        if(tmpnrm.x*tnrm2.x+tmpnrm.y*tnrm2.y<0) a.change(vec(0,0),-.01);
        else a.change(vec(0,0),.01);
      }
      a.move();
      if(sqrt((a.p.x-food.x)*(a.p.x-food.x)+(a.p.y-food.y)*(a.p.y-food.y))<10){
        if(aaa>0) aaa-=1;
        food=vec(rand()%WIN_WID,rand()%WIN_HGT);
      }
      tpol[0].x=(int)(a.p.x+getx(a.n,vec(15.0,-4.0)));
      tpol[0].y=(int)(a.p.y+gety(a.n,vec(15.0,-4.0)));
      tpol[1].x=(int)(a.p.x+getx(a.n,vec(15.0,4.0)));
      tpol[1].y=(int)(a.p.y+gety(a.n,vec(15.0,4.0)));
      tpol[2].x=(int)(a.p.x+getx(a.n,vec(-5.0,8.0)));
      tpol[2].y=(int)(a.p.y+gety(a.n,vec(-5.0,8.0)));
      tpol[3].x=(int)(a.p.x+getx(a.n,vec(-5.0,-8.0)));
      tpol[3].y=(int)(a.p.y+gety(a.n,vec(-5.0,-8.0)));
      Polygon(doubleBuff.back_dc,tpol,4);
      
      if(c.lloss>0) c.lloss--;
      for(int i=aaa;i<mtail;i++){
        if(i==aaa) b[i].follow(a.p);
        else{
          b[i].follow(b[i-1].p);
          tmppnt=getcpnt(b[i-1].p,b[i].p,c.p);
          tmpdis=sqrt((c.p.x-tmppnt.x)*(c.p.x-tmppnt.x)+(c.p.y-tmppnt.y)*(c.p.y-tmppnt.y));
          if(tmpdis<10 && tmpdis>0){
            tmpnrm=vec((tmppnt.x-c.p.x)/tmpdis,(tmppnt.y-c.p.y)/tmpdis);
            tmpdis=c.s.x*tmpnrm.x+c.s.y*tmpnrm.y;
            c.s=c.s-tmpnrm*tmpdis*2-tmpnrm*.2;
            if(ccc<mtail && c.lloss==0){
              ccc+=1;
              c.lloss=10;
            }
          }
        }
        if(i==mtail-1){        
          tpol[0].x=(int)(b[i].p.x+getx(b[i].n,vec(5.0,-5)));
          tpol[0].y=(int)(b[i].p.y+gety(b[i].n,vec(5.0,-5)));
          tpol[1].x=(int)(b[i].p.x+getx(b[i].n,vec(5.0,5)));
          tpol[1].y=(int)(b[i].p.y+gety(b[i].n,vec(5.0,5)));
          tpol[2].x=(int)(b[i].p.x+getx(b[i].n,vec(-5.0,1.0)));
          tpol[2].y=(int)(b[i].p.y+gety(b[i].n,vec(-5.0,1.0)));
          tpol[3].x=(int)(b[i].p.x+getx(b[i].n,vec(-5.0,-1.0)));
          tpol[3].y=(int)(b[i].p.y+gety(b[i].n,vec(-5.0,-1.0)));
          Polygon(doubleBuff.back_dc,tpol,4);
        }else{
          tpol[0].x=(int)(b[i].p.x+getx(b[i].n,vec(3.0,-2.5)));
          tpol[0].y=(int)(b[i].p.y+gety(b[i].n,vec(3.0,-2.5)));
          tpol[1].x=(int)(b[i].p.x+getx(b[i].n,vec(3.0,2.5)));
          tpol[1].y=(int)(b[i].p.y+gety(b[i].n,vec(3.0,2.5)));
          tpol[2].x=(int)(b[i].p.x+getx(b[i].n,vec(-3.0,3.0)));
          tpol[2].y=(int)(b[i].p.y+gety(b[i].n,vec(-3.0,3.0)));
          tpol[3].x=(int)(b[i].p.x+getx(b[i].n,vec(-3.0,-3.0)));
          tpol[3].y=(int)(b[i].p.y+gety(b[i].n,vec(-3.0,-3.0)));
          Polygon(doubleBuff.back_dc,tpol,4);
        }
      }
      if(c.idl<500) c.idl++;
      c.change(vec(c.n.x*.3,c.n.y*.3),0);
      if(GetKeyState('S')&0x80){
        c.change(vec(c.n.x*-.3,c.n.y*-.3),0);
        c.idl=0;
      }
      if(GetKeyState('A')&0x80){
        c.change(vec(0,0),-.01);
        c.idl=0;
      }
      if(GetKeyState('D')&0x80){
        c.change(vec(0,0),.01);
        c.idl=0;
      }
      tmpdis=sqrt((c.p.x-food.x)*(c.p.x-food.x)+(c.p.y-food.y)*(c.p.y-food.y));
      if(rand()%20<1) c.aimode=rand()%2;
      if(rand()%10<1) c.aimode=1;
      if(c.idl==500 && tmpdis>0 && c.aimode){
        tmpnrm=vec((food.x-c.p.x)/tmpdis,(food.y-c.p.y)/tmpdis);
        tnrm2=vec(-c.n.y,c.n.x);
        if(tmpnrm.x*tnrm2.x+tmpnrm.y*tnrm2.y<0) c.change(vec(0,0),-.01);
        else c.change(vec(0,0),.01);
      }
      c.move();
      if(tmpdis<10){
        if(ccc>0) ccc-=1;
        food=vec(rand()%WIN_WID,rand()%WIN_HGT);
      }
      tpol[0].x=(int)(c.p.x+getx(c.n,vec(15.0,-2.0)));
      tpol[0].y=(int)(c.p.y+gety(c.n,vec(15.0,-2.0)));
      tpol[1].x=(int)(c.p.x+getx(c.n,vec(15.0,2.0)));
      tpol[1].y=(int)(c.p.y+gety(c.n,vec(15.0,2.0)));
      tpol[2].x=(int)(c.p.x+getx(c.n,vec(-5.0,7.0)));
      tpol[2].y=(int)(c.p.y+gety(c.n,vec(-5.0,7.0)));
      tpol[3].x=(int)(c.p.x+getx(c.n,vec(-5.0,-7.0)));
      tpol[3].y=(int)(c.p.y+gety(c.n,vec(-5.0,-7.0)));
      Polygon(doubleBuff.back_dc,tpol,4);
      
      if(a.lloss>0) a.lloss--;
      for(int i=ccc;i<mtail;i++){
        if(i==ccc) d[i].follow(c.p);
        else{
          d[i].follow(d[i-1].p);
          tmppnt=getcpnt(d[i-1].p,d[i].p,a.p);
          tmpdis=sqrt((a.p.x-tmppnt.x)*(a.p.x-tmppnt.x)+(a.p.y-tmppnt.y)*(a.p.y-tmppnt.y));
          if(tmpdis<10 && tmpdis>0){
            tmpnrm=vec((tmppnt.x-a.p.x)/tmpdis,(tmppnt.y-a.p.y)/tmpdis);
            tmpdis=a.s.x*tmpnrm.x+a.s.y*tmpnrm.y;
            a.s=a.s-tmpnrm*tmpdis*2-tmpnrm*.2;
            if(aaa<mtail && a.lloss==0){
              aaa+=1;
              a.lloss=10;
            }
          }
        }
        if(i==mtail-1){        
          tpol[0].x=(int)(d[i].p.x+getx(d[i].n,vec(7.0,-3)));
          tpol[0].y=(int)(d[i].p.y+gety(d[i].n,vec(7.0,-3)));
          tpol[1].x=(int)(d[i].p.x+getx(d[i].n,vec(7.0,3)));
          tpol[1].y=(int)(d[i].p.y+gety(d[i].n,vec(7.0,3)));
          tpol[2].x=(int)(d[i].p.x+getx(d[i].n,vec(-7.0,0.2)));
          tpol[2].y=(int)(d[i].p.y+gety(d[i].n,vec(-7.0,0.2)));
          tpol[3].x=(int)(d[i].p.x+getx(d[i].n,vec(-7.0,-0.2)));
          tpol[3].y=(int)(d[i].p.y+gety(d[i].n,vec(-7.0,-0.2)));
          Polygon(doubleBuff.back_dc,tpol,4);
        }else{
          tpol[0].x=(int)(d[i].p.x+getx(d[i].n,vec(1.7,-2.5)));
          tpol[0].y=(int)(d[i].p.y+gety(d[i].n,vec(1.7,-2.5)));
          tpol[1].x=(int)(d[i].p.x+getx(d[i].n,vec(1.7,2.5)));
          tpol[1].y=(int)(d[i].p.y+gety(d[i].n,vec(1.7,2.5)));
          tpol[2].x=(int)(d[i].p.x+getx(d[i].n,vec(-1.7,3.0)));
          tpol[2].y=(int)(d[i].p.y+gety(d[i].n,vec(-1.7,3.0)));
          tpol[3].x=(int)(d[i].p.x+getx(d[i].n,vec(-1.7,-3.0)));
          tpol[3].y=(int)(d[i].p.y+gety(d[i].n,vec(-1.7,-3.0)));
          Polygon(doubleBuff.back_dc,tpol,4);
        }
      }        
      tpol[0].x=(int)(food.x-3);
      tpol[0].y=(int)(food.y-3);
      tpol[1].x=(int)(food.x+3);
      tpol[1].y=(int)(food.y-3);
      tpol[2].x=(int)(food.x+3);
      tpol[2].y=(int)(food.y+3);
      tpol[3].x=(int)(food.x-3);
      tpol[3].y=(int)(food.y+3);
      Polygon(doubleBuff.back_dc,tpol,4);
      /************************/
      
      BitBlt(doubleBuff.win_dc,0,0,doubleBuff.rect.right,
      doubleBuff.rect.bottom,doubleBuff.back_dc,0,0,SRCCOPY);
    }
  }
  
  SelectObject(img_dc,old_pen);
  DeleteObject(img_dc);
  
  FreeDoubleBuffer(doubleBuff);
  UnregisterClass(class_name,hinstance);
  return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){
  PAINTSTRUCT ps;
  switch(message){       		
  case WM_PAINT:
    BeginPaint(hwnd,&ps);
    EndPaint(hwnd,&ps);
    return 0;
  case WM_DESTROY:
  case WM_CLOSE:
    ChangeDisplaySettings(NULL, 0);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, message, wparam, lparam);
}

bool InitDoubleBuffer(SDBuffer &doubleBuff){
  if(!doubleBuff.win_hwnd) return false;
  GetClientRect(doubleBuff.win_hwnd,&doubleBuff.rect);
  doubleBuff.win_dc=GetDC(doubleBuff.win_hwnd);
  if(doubleBuff.win_dc == NULL) return false;
  doubleBuff.back_dc=CreateCompatibleDC(doubleBuff.win_dc);
  if(doubleBuff.back_dc == NULL) return false;
  doubleBuff.back_bmp=CreateCompatibleBitmap(doubleBuff.win_dc, doubleBuff.rect.right, doubleBuff.rect.bottom);
  if(doubleBuff.back_bmp == NULL) return false;
  doubleBuff.old_bmp=(HBITMAP)SelectObject(doubleBuff.back_dc,doubleBuff.back_bmp);
  return true;
}

void FreeDoubleBuffer(SDBuffer &doubleBuff){
  if(!doubleBuff.win_hwnd) return;
  if(doubleBuff.old_bmp){
    SelectObject(doubleBuff.back_dc,doubleBuff.old_bmp);
    DeleteObject(doubleBuff.back_bmp);
    DeleteDC(doubleBuff.back_dc);
  }
  if(doubleBuff.win_dc) ReleaseDC(doubleBuff.win_hwnd,doubleBuff.win_dc);
  memset(&doubleBuff,0,sizeof(SDBuffer));
}

bool LockFrameRate(int frame_rate){
  static float lastTime=0.0f;
  float currentTime=GetTickCount()*0.001f;
  if((currentTime-lastTime) > (1.0f/frame_rate)){
    lastTime=currentTime;	
    return true;
  }
  return false;
}
