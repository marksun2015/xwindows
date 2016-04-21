/* 
* Xlib 學習 
* 
* 
* XEvent  
* XMotion 
* 
* 
* 
* 
* 編譯基於Xlib的程序需要與Xlib庫連接。可以使用下面的命令行： 
  
  cc prog.c -o prog -lX11 
  
  如果編譯器報告找不到X11庫，可以試著加上"-L"標誌，像這樣： 
  
   cc prog.c -o prog -L/usr/X11/lib -lX11 
  
   或者這樣（針對使用X11的版本6） 
  
   cc prog.c -o prog -L/usr/X11R6/lib -lX11 
  
   在SunOs 4 系統上，X的庫被放到了/usr/openwin/lib 
  
   cc prog.c -o prog -L/usr/openwin/lib -lX11 
* 
* GC-圖形上下文.[圖形,文本等, 繪製,前景,背景,使用什麼顏色,使用什麼字體等等.] 
* 對象句柄:例如: 窗口,繪圖區和光標-相應的函數就會返回一個句柄. 
* 釋放內存: XFree() 
* 事件: XEvent,(聯合體), XMotion, XButon. 
* */  
   
/* 
*      
Display* display;    
display = XOpenDisplay("simey:0"); 
if (NULL == display){ 
fprintf(stderr, "連接不上X Server %s\n", "simey:0"); 
exit(-1); 
} 
關閉X服務器連接 
XCloseDisplay(display) 
* */  
   
   
   
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <X11/keysymdef.h>  
#include <X11/Xlib.h>  
   
int  main( int  argc,  char  *argv[])  
{  
       
     Display* display;  
     display = XOpenDisplay( NULL);
       
     if  (NULL == display){  
          fprintf(stderr,  "連接不上X Server %s\n" ,  "simey:0" );  
          exit(-1);  
     }  
     int  screen_num;  
     int  screen_width;  
     int  screen_height;  
     Window root_window;  
       
     unsigned  long  white_pixel;  
     unsigned  long  black_pixel;  
       
     screen_num = DefaultScreen(display);  
       
     screen_width = DisplayWidth(display, screen_num);  
     screen_height = DisplayHeight(display, screen_num);  
           
     puts( "測試輸出:" );  
     printf( "句柄:%x寬度:%d高度:%d\n" ,  
            screen_num, screen_width, screen_height);  
           
     root_window = RootWindow(display, screen_num);  
     white_pixel = WhitePixel(display, screen_num);  
     black_pixel = BlackPixel(display, screen_num);  
       
     Window win;  
     int  win_width;  
     int  win_height;  
     int  win_x;  
     int  win_y;       
     int  win_border_width;  
     int  win_border_height;  
           
     int  width;  
     int  height;  
       
     win_x = win_y = win_border_width= win_border_height = 0;  
     win_width = DisplayWidth(display, screen_num);  
     win_height = DisplayHeight(display, screen_num);  
       
     width = (win_width / 3);  
     height = (win_height / 3);  
     win_border_width = 2;  
       
     /*創建一個窗口*/       
     win = XCreateSimpleWindow(  
          display,  
          RootWindow(display, screen_num),  
          win_x, win_y,  
          width, height,  
          win_border_width, win_border_height,  
          WhitePixel(display, screen_num)  
          );  
     /*註冊事件*/  
     /* XSelectInput(display, win, ExposureMask); */  
     /*ExposureMask在頭文件"Xh"中被定義，如果我們想註冊更多的事件類型，我們可以使用邏輯"or"*/  
       
     XSelectInput(display, win,  
                  ExposureMask | ButtonPressMask |  
                  ButtonReleaseMask | ButtonPress |  
                  ButtonRelease | EnterWindowMask |  
                  LeaveWindowMask | EnterNotify |  
                  LeaveNotify  
          );  
     /*鼠標的進入和離開Enter Leave和GTK很像.*/  
       
     XMapWindow(display, win);  
       
     /*畫筆*/  
     GC gc;  
     XGCValues values;  
     unsigned  long  valuemask = 0;  
     /* XGCValues?? values?? = CapButt | JoinBevel; */  
     /* unsigned long valuemask = GCCapStyle | GCJoinStyle; */  
     gc = XCreateGC(display, win, valuemask, &values);  
     XSync(display, False);  
     if  (gc < 0)  
     {  
          fprintf(stderr,  "XCreateGC:\n" );  
     }  
     /*畫畫.*/  
       
     XSetBackground(display, gc, WhitePixel(display, screen_num)); /*設置背景顏色*/  
     XSetForeground(display, gc, BlackPixel(display, screen_num)); /*設置前景色*/  
     unsigned  int  line_width = 2;  
     int  line_style = LineSolid;  
     int  cap_style = CapButt;  
     int  join_style = JoinBevel;  
   
     XSetLineAttributes(display, gc,  
                        line_width, line_style, cap_style, join_style);  
     XSetFillStyle(display, gc, FillSolid);  
       
   
     /* sleep(14); */  
     XEvent an_event;  
     /*事件循環*/  
     while  (1)  
     {  
          XNextEvent(display, &an_event);  
          /*這裡就是判斷所有事件*/  
   
           
          switch (an_event.type)  
          {  
                 
          case  KeyPress:  
               printf( "鍵盤被按下" );  
               break ;  
          case  Expose:  /*重繪*/  
               if (an_event.xexpose.count > 0)       
               {  
                    break ;  
               }  
               XDrawLine(display, win, gc, 0, 100, 400, 100);  
               XDrawPoint(display, win, gc, 5, 5);  
               XDrawRectangle(display, win, gc, 120, 150, 50, 60);       
               XFillRectangle(display, win, gc, 60, 150, 50, 60);  
               /*刷新*/  
               XFlush(display);  
               printf( "正在重繪事件\n" );  
               break ;  
          case  ButtonPress:  /*按下事件*/  
               /* int x; */  
               /* int y; */  
               /* x = an_event.xbutton.x; */  
               /* y = an_event.xbutton.window; */                 
               switch (an_event.xbutton.button){  
               case  Button1:  
                    XDrawRectangle(display, win, gc, 120, 150, 50, 60);       
                    puts( "鼠標左鍵按下" );  
                    break ;  
               case  Button2:       
                    puts( "button2..." );  
                    break ;  
               default :  
                    break ;  
               }  
          }                 
           
     }  
     /*關閉X服務器連接*/  
     XCloseDisplay(display);  
     return  0;  
}  
   
   
/* 
  Display* display 
  指向顯示結構的指針 
  Window parent 
  新窗口的父窗口的ID。 
  int x 
  窗口的左上X坐標（單位為屏幕像素） 
  int y 
  窗口的左上Y坐標（單位為屏幕像素） 
  unsigned int width 
  窗口的寬度（單位為屏幕像素） 
  unsigned int height 
  窗口的高度（單位為屏幕像素） 
  unsigned int border_width 
  窗口的邊框寬度（單位為屏幕像素） 
  unsigned long border 
  用來繪製窗口邊框的顏色 
  unsigned long background 
  用來繪製窗口背景的顏色 
* */  
   
/* 
  在事件結構裡，通過"an_event.xbutton"來獲得事件的類型，另外它還包括下面這些有趣的內容： 
  
  Window window 
  事件發送的目標窗口的ID（如果我們為多個窗口註冊了事件） 
  
  int x, y 
  從窗口的左上坐標算起，鼠標鍵按下時光標在窗口中的坐標 
  
  int button 
  鼠標上那個標號的按鈕被按下了，值可能是Button1，Button2，Button3 
  
  Time time 
  事件被放進隊列的時間。可以被用來實現雙擊的處理 
* */  
   
   
/* 
  
  鼠標光標的進入和離開事件 
  
  另一個程序通常會感興趣的事件是，有關鼠標光標進入一個窗口的領域以及離開那個窗口的領域的事件。有些程序利用該事件來告訴用戶程序現在在焦點裡面。為了註冊這種事件，我們將會在函數XSelectInput()裡註冊幾個面具。 
  
  EnterWindowMask 
  通知我們鼠標光標進入了我們的窗口中的任意一個 
  
  LeaveWindowMask 
  通知我們鼠標光標離開了我們的窗口中的任意一個 
  
  我們的事件循環中的分支檢查將檢查以下的事件類型 
  
  EnterNotify 
  鼠標光標進入了我們的窗口 
  
  LeaveNotify 
  鼠標光標離開了我們的窗口 
  
  這些事件類型的數據結構通過例如"an_event.xcrossing"來訪問，它還包含以下有趣的成員變量： 
  
  Window window 
  事件發送的目標窗口的ID（如果我們為多個窗口註冊了事件） 
  
  Window subwindow 
  在一個進入事件中，它的意思是從那個子窗口進入我們的窗口，在一個離開事件中，它的意思是進入了那個子窗口，如果是"none"，它的意思是從外面進入了我們的窗口。 
  
  int x, y 
  從窗口的左上坐標算起，事件產生時鼠標光標在窗口中的坐標 
  
  int mode 
  鼠標上那個標號的按鈕被按下了，值可能是Button1，Button2，Button3 
  
  Time time 
  事件被放進隊列的時間。可以被用來實現雙擊的處理 
  
  unsigned int state 
  這個事件發生時鼠標按鈕（或是鍵盤鍵）被按下的情況- 如果有的話。這個成員使用按位或的方式來表示 
  Button1Mask 
  Button2Mask 
  Button3Mask 
  Button4Mask 
  ShiftMask 
  LockMask 
  ControlMask 
  Mod1Mask 
  Mod2Mask 
  Mod3Mask 
  Mod4Mask 
  
  它們的名字是可以擴展的，當第五個鼠標鈕被按下時，剩下的屬性就指明其它特殊鍵（例如Mod1一般是"ALT"或者是"META"鍵） 
  
  Bool focus 
  當值是True的時候說明窗口獲得了鍵盤焦點，False反之 
* */  
   
   
/* 
* 鍵盤鍵按下和鬆開事件 
如果我們程序控制的窗口獲得了鍵盤焦點，它就可以接受按鍵的按下和鬆開事件。為了註冊這些事件的類型，我們就需要通過函數XSelectInput()來註冊下面的面具。 
  
  
KeyPressMask 
通知我們的程序什麼時候按鍵被按下了 
  
KeyPressMask 
通知我們的程序什麼時候按鍵被鬆開了 
  
我們的事件循環中的分支檢查將檢查以下的事件類型 
  
Window window 
事件發送的目標窗口的ID（如果我們為多個窗口註冊了事件） 
  
unsigned int keycode 
被按下（或鬆開）的鍵的編碼。這是一些X內部編碼，應該被翻譯成一個鍵盤鍵符號才能方便使用，將會在下面介紹。 
  
int x, y 
從窗口的左上坐標算起，事件產生時鼠標光標在窗口中的坐標 
  
Time time 
事件被放進隊列的時間。可以被用來實現雙擊的處理 
  
unsigned int state 
這個事件發生時鼠標按鈕（或是鍵盤鍵）被按下的情況- 如果有的話。這個成員使用按位或的方式來表示 
Button1Mask 
Button2Mask 
Button3Mask 
Button4Mask 
ShiftMask 
LockMask 
ControlMask 
Mod1Mask 
Mod2Mask 
Mod3Mask 
Mod4Mask 
  
它們的名字是可以擴展的，當第五個鼠標鈕被按下時，剩下的屬性就指明其它特殊鍵（例如Mod1一般是"ALT"或者是"META"鍵） */  
   
