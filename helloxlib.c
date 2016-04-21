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
    //��??�� ?��������?��??��?�ϥ�?��?�q����DESPLAY  
    display = XOpenDisplay(NULL);  
    if( !display ){  
        //DUMP_ERR("call XOpenDisplay(%s) fail/n", XDisplayName(NULL));  
        return 1;  
    }  
      
    //�o��t??�e���̹� (�p�G�ϥ�??�̹���? �@��?���L?)  
    // get default screen  
    screennum = DefaultScreen(display);  
    //���u�̹����j�p?�w���f���j�p  
    width = DisplayWidth(display,screennum)/4;  
    height = DisplayHeight(display,screennum)/4;  
      
    //?�ص��f  
    // create window  
    win = XCreateSimpleWindow(display,  
        RootWindow(display,screennum),  
        0,0,width,height,3,  
        BlackPixel(display,screennum),  
        WhitePixel(display,screennum));  
     
    // ??���f?�ߪ��ƥ�  
    // select event  
    XSelectInput(display,win,  
        ExposureMask|KeyPressMask  
        |ButtonPressMask|StructureNotifyMask);  
      
    // ?��GC(?�ΤW�U��)  
    // create gc  
    {  
        unsigned long valuemask = 0;  
        XGCValues values;  
        gc = XCreateGC(display,win,valuemask,&values);  
    }  
      
    // �M�g���f �u��map�Z���f�~?�b�̹��W?��  
    // show window  
    XMapWindow(display,win);  
      
    // ?�J�ƥ�`?  
    // event loop  
    while(1){  
        XEvent event;  
        XNextEvent(display,&event); // ?�@?�ƥ�  
          
        switch(event.type){  
        case Expose: // expose window ?�ܨƥ�  
            if( event.xexpose.count != 0) break;  
            XDrawString(display,win,gc,10,10,  
                strdraw,strlen(strdraw));  
            break;  
              
            // ��?�ƥ�  
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