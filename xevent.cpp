#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <X11/keysymdef.h>
#include <X11/Xlib.h>

int main(int argc, char *argv[])
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

    puts( "test output:" );
    printf( "fd:%x, width:%d, height:%d\n",
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

    XSetLineAttributes(display, gc, line_width, line_style, cap_style, join_style);
    XSetFillStyle(display, gc, FillSolid);

    XTextItem  item;
    item.nchars = 2;
    item.font = None;
    item.chars = "B1";
    item.delta = 0;

    /* sleep(14); */
    XEvent an_event;
    /*事件循環*/
    while  (1)
    {
        XNextEvent(display, &an_event);
        XWindowAttributes xgwa;
        XGetWindowAttributes (display, win, &xgwa);

        /*這裡就是判斷所有事件*/
        switch (an_event.type)
        {
            case  KeyPress:
                printf( "keyPress \n" );
                break ;
            case  Expose:  /*重繪*/
                if (an_event.xexpose.count > 0){
                    break ;
                }
                XDrawLine(display, win, gc, 0, 20, xgwa.width, 20);
                XDrawPoint(display, win, gc, xgwa.width/2, xgwa.height/2 - 10);
                XDrawRectangle(display, win, gc, 60, 70, xgwa.width/4 , xgwa.width/3);
                XFillRectangle(display, win, gc, 20, 30, xgwa.width/4 , xgwa.width/3);
                /*刷新*/
                XFlush(display);
                printf( "Expose \n" );
                break ;
            case  ButtonPress:  /*按下事件*/
                /* int x; */
                /* int y; */
                /* x = an_event.xbutton.x; */
                /* y = an_event.xbutton.window; */
                int x; 
                int y; 
                x = an_event.xbutton.x; 
                y = an_event.xbutton.y; 
                XSetForeground(display, gc, white_pixel); /*設置前景色*/
                XFillRectangle (display, win, gc, 0, 0, xgwa.width, xgwa.height);
                XSetForeground(display, gc, black_pixel); /*設置前景色*/
                //srand( time(NULL) );
                //x = (rand() % (width-25))+25;
                //y = (rand() % (height-25)+25);
                switch (an_event.xbutton.button){
                    case  Button1:
                        printf( "Button1... x:%d, y:%d \n",x ,y );
                        item.chars = "B1";
                        XDrawText(display, win, gc, x, y, &item, 1);
                        XFlush(display);
                        break ;
                    case  Button2:
                        printf( "Button2... x:%d, y:%d \n",x ,y );
                        item.chars = "B2";
                        XDrawText(display, win, gc, x, y, &item, 1);
                        XFlush(display);
                        break ;
                    case  Button3:
                        printf( "Button3... x:%d, y:%d \n",x ,y );
                        item.chars = "B3";
                        XDrawText(display, win, gc, x, y, &item, 1);
                        XFlush(display);
                        break ;
                    case  Button4:
                        printf( "Button4... x:%d, y:%d \n",x ,y );
                        item.chars = "B4";
                        XDrawText(display, win, gc, x, y, &item, 1);
                        XFlush(display);
                        break ;
                    case  Button5:
                        printf( "Button5... x:%d, y:%d \n",x ,y );
                        item.chars = "B5";
                        XDrawText(display, win, gc, x, y, &item, 1);
                        XFlush(display);
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
