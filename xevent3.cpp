#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/keysymdef.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#define XI_DeviceChanged                 1
#define XI_KeyPress                      2
#define XI_KeyRelease                    3
#define XI_ButtonPress                   4
#define XI_ButtonRelease                 5
#define XI_Motion                        6
#define XI_Enter                         7
#define XI_Leave                         8
#define XI_FocusIn                       9
#define XI_FocusOut                      10
#define XI_HierarchyChanged              11
#define XI_PropertyEvent                 12
#define XI_RawKeyPress                   13
#define XI_RawKeyRelease                 14
#define XI_RawButtonPress                15
#define XI_RawButtonRelease              16
#define XI_RawMotion 17


static const char* type_to_name(int evtype)
{
    const char *name;

    switch(evtype) {
        case XI_DeviceChanged:    name = "DeviceChanged";       break;
        case XI_KeyPress:         name = "KeyPress";            break;
        case XI_KeyRelease:       name = "KeyRelease";          break;
        case XI_ButtonPress:      name = "ButtonPress";         break;
        case XI_ButtonRelease:    name = "ButtonRelease";       break;
        case XI_Motion:           name = "Motion";              break;
        case XI_Enter:            name = "Enter";               break;
        case XI_Leave:            name = "Leave";               break;
        case XI_FocusIn:          name = "FocusIn";             break;
        case XI_FocusOut:         name = "FocusOut";            break;
        case XI_HierarchyChanged: name = "HierarchyChanged";    break;
        case XI_PropertyEvent:    name = "PropertyEvent";       break;
        case XI_RawKeyPress:      name = "RawKeyPress";         break;
        case XI_RawKeyRelease:    name = "RawKeyRelease";       break;
        case XI_RawButtonPress:   name = "RawButtonPress";      break;
        case XI_RawButtonRelease: name = "RawButtonRelease";    break;
        case XI_RawMotion:        name = "RawMotion";           break;
        default:
                                  name = "unknown event type"; break;
    }
    return name;
}

void select_xi2_events_on_window(Display * display, Window window)
{
    unsigned char mask[XIMaskLen(XI_LASTEVENT)] = {0,};

    XIEventMask event_mask;
    event_mask.deviceid = XIAllMasterDevices;
    event_mask.mask_len = sizeof(mask);
    event_mask.mask = mask;

    XISetMask(event_mask.mask, XI_RawKeyPress);
    XISetMask(event_mask.mask, XI_RawKeyRelease);
    XISetMask(event_mask.mask, XI_RawButtonPress);
    XISetMask(event_mask.mask, XI_RawButtonRelease);
    XISetMask(event_mask.mask, XI_RawMotion);

    XISelectEvents(display, window, &event_mask, 1);
    XSync(display, False);
}

int main(int argc, char *argv[])
{

     Display* display;
     display = XOpenDisplay( NULL);

     if  (NULL == display){
          fprintf(stderr,  "connect X failed, X Server %s\n" ,  "simey:0" );
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

     printf( "screen:%x wieth:%d height:%d\n" ,
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

     width = (win_width / 2);
     height = (win_height / 2);
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
     int xi_opcode(0);
     int first_event(0);
     int first_error(0);
     if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &first_event, &first_error))
     {
         printf( "X Input extension not available.\n" );
     }

     select_xi2_events_on_window(display, root_window);

     /*事件循環*/
     while  (1)
     {
          if (XEventsQueued(display, QueuedAfterReading) <= 0)
          {
              continue;
          }

          XEvent an_event;
          XGenericEventCookie * cookie = reinterpret_cast<XGenericEventCookie *>(&an_event);
          XNextEvent(display, &an_event);

          if (XGetEventData(display, cookie) &&
              cookie->type == GenericEvent &&
              cookie->extension == xi_opcode)
          {
              printf( "emit_input_event - " );
              printf("type %d (%s)\n", cookie->evtype, type_to_name(cookie->evtype));
          }

          XFreeEventData(display, cookie);

#if 0
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
                    puts( "button1..." );
                    break ;
               case  Button2:
                    puts( "button2..." );
                    break ;
               case  Button3:
                    puts( "button3..." );
                    break ;
               default :
                    break ;
               }
          }
#endif

     }
     /*關閉X服務器連接*/
     XCloseDisplay(display);
     return  0;
}
