/*
 *  https://www.linuxquestions.org/questions/programming-9/simulating-a-mouse-click-594576/
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void mouseClick(int button,Display *display)
{
    XEvent event;

    if(display == NULL)
    {
        fprintf(stderr, "Errore nell'apertura del Display !!!\n");
        exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, 
            RootWindow(display, DefaultScreen(display)), 
            &event.xbutton.root, 
            &event.xbutton.window, 
            &event.xbutton.x_root, 
            &event.xbutton.y_root, 
            &event.xbutton.x, 
            &event.xbutton.y, 
            &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    //printf("event xroot:%d, event yroot:%d \n",event.xbutton.x_root, event.xbutton.y_root);
    //printf("event x:%d, event y:%d \n",event.xbutton.x, event.xbutton.y);

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, 
                event.xbutton.window, 
                &event.xbutton.root, 
                &event.xbutton.subwindow, 
                &event.xbutton.x_root, 
                &event.xbutton.y_root, 
                &event.xbutton.x, 
                &event.xbutton.y, 
                &event.xbutton.state);
    }

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) 
        fprintf(stderr, "Error\n");

    XFlush(display);

    usleep(100000);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) 
        fprintf(stderr, "Error\n");
}

int main(int argc, char * argv[]) {

    int x , y;

    if(argc < 3) {
        printf("  send button1 please input: fakexy x y \n");
        printf("     capture button, x need larger than window border: 1 \n");
        printf("     capture button, y need larger than title height: 24 \n");
        return 0;
    }

    x = atoi(argv[1]);
    y = atoi(argv[2]);

    Display *display = XOpenDisplay(0);
    Window root = DefaultRootWindow(display);
    XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
    mouseClick(Button1, display);
    XFlush(display);
    XCloseDisplay(display);

    return 0;
}
