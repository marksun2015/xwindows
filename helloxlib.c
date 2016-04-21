#include <X11/Xlib.h>  
#include <X11/Xutil.h>  
#include <X11/Xos.h>  
#include <X11/Xatom.h>  
#define DUMP_ERR printf  
int main()  
{  
    Display *display;  
    int screennum;  
    int width;  
    int height;  
      
    const char *strdraw = "hello world";  
      
    Window win;  
    GC gc;  
    //打??示 ?有指明打?哪??示?使用?境?量中的DESPLAY  
    display = XOpenDisplay(NULL);  
    if( !display ){  
        //DUMP_ERR("call XOpenDisplay(%s) fail/n", XDisplayName(NULL));  
        return 1;  
    }  
      
    //得到系??前的屏幕 (如果使用??屏幕的? 一般?有几?)  
    // get default screen  
    screennum = DefaultScreen(display);  
    //根据屏幕的大小?定窗口的大小  
    width = DisplayWidth(display,screennum)/4;  
    height = DisplayHeight(display,screennum)/4;  
      
    //?建窗口  
    // create window  
    win = XCreateSimpleWindow(display,  
        RootWindow(display,screennum),  
        0,0,width,height,3,  
        BlackPixel(display,screennum),  
        WhitePixel(display,screennum));  
     
    // ??窗口?心的事件  
    // select event  
    XSelectInput(display,win,  
        ExposureMask|KeyPressMask  
        |ButtonPressMask|StructureNotifyMask);  
      
    // ?建GC(?形上下文)  
    // create gc  
    {  
        unsigned long valuemask = 0;  
        XGCValues values;  
        gc = XCreateGC(display,win,valuemask,&values);  
    }  
      
    // 映射窗口 只有map后窗口才?在屏幕上?示  
    // show window  
    XMapWindow(display,win);  
      
    // ?入事件循?  
    // event loop  
    while(1){  
        XEvent event;  
        XNextEvent(display,&event); // ?一?事件  
          
        switch(event.type){  
        case Expose: // expose window ?示事件  
            if( event.xexpose.count != 0) break;  
            XDrawString(display,win,gc,10,10,  
                strdraw,strlen(strdraw));  
            break;  
              
            // 按?事件  
        case ButtonPress:  
        case KeyPress:  
            XFreeGC(display,gc);  
            //XCloseWindow(win);  
            XCloseDisplay(display);  
            return 0;  
        default:  
            break;  
        }  
    }  
}  