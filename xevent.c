/* 
* Xlib �ǲ� 
* 
* 
* XEvent  
* XMotion 
* 
* 
* 
* 
* �sĶ���Xlib���{�ǻݭn�PXlib�w�s���C�i�H�ϥΤU�����R�O��G 
  
  cc prog.c -o prog -lX11 
  
  �p�G�sĶ�����i�䤣��X11�w�A�i�H�յۥ[�W"-L"�лx�A���o�ˡG 
  
   cc prog.c -o prog -L/usr/X11/lib -lX11 
  
   �Ϊ̳o�ˡ]�w��ϥ�X11������6�^ 
  
   cc prog.c -o prog -L/usr/X11R6/lib -lX11 
  
   �bSunOs 4 �t�ΤW�AX���w�Q���F/usr/openwin/lib 
  
   cc prog.c -o prog -L/usr/openwin/lib -lX11 
* 
* GC-�ϧΤW�U��.[�ϧ�,�奻��, ø�s,�e��,�I��,�ϥΤ����C��,�ϥΤ���r�鵥��.] 
* ��H�y�`:�Ҧp: ���f,ø�ϰϩM����-��������ƴN�|��^�@�ӥy�`. 
* ���񤺦s: XFree() 
* �ƥ�: XEvent,(�p�X��), XMotion, XButon. 
* */  
   
/* 
*      
Display* display;    
display = XOpenDisplay("simey:0"); 
if (NULL == display){ 
fprintf(stderr, "�s�����WX Server %s\n", "simey:0"); 
exit(-1); 
} 
����X�A�Ⱦ��s�� 
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
          fprintf(stderr,  "�s�����WX Server %s\n" ,  "simey:0" );  
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
           
     puts( "���տ�X:" );  
     printf( "�y�`:%x�e��:%d����:%d\n" ,  
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
       
     /*�Ыؤ@�ӵ��f*/       
     win = XCreateSimpleWindow(  
          display,  
          RootWindow(display, screen_num),  
          win_x, win_y,  
          width, height,  
          win_border_width, win_border_height,  
          WhitePixel(display, screen_num)  
          );  
     /*���U�ƥ�*/  
     /* XSelectInput(display, win, ExposureMask); */  
     /*ExposureMask�b�Y���"Xh"���Q�w�q�A�p�G�ڭ̷Q���U��h���ƥ������A�ڭ̥i�H�ϥ��޿�"or"*/  
       
     XSelectInput(display, win,  
                  ExposureMask | ButtonPressMask |  
                  ButtonReleaseMask | ButtonPress |  
                  ButtonRelease | EnterWindowMask |  
                  LeaveWindowMask | EnterNotify |  
                  LeaveNotify  
          );  
     /*���Ъ��i�J�M���}Enter Leave�MGTK�ܹ�.*/  
       
     XMapWindow(display, win);  
       
     /*�e��*/  
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
     /*�e�e.*/  
       
     XSetBackground(display, gc, WhitePixel(display, screen_num)); /*�]�m�I���C��*/  
     XSetForeground(display, gc, BlackPixel(display, screen_num)); /*�]�m�e����*/  
     unsigned  int  line_width = 2;  
     int  line_style = LineSolid;  
     int  cap_style = CapButt;  
     int  join_style = JoinBevel;  
   
     XSetLineAttributes(display, gc,  
                        line_width, line_style, cap_style, join_style);  
     XSetFillStyle(display, gc, FillSolid);  
       
   
     /* sleep(14); */  
     XEvent an_event;  
     /*�ƥ�`��*/  
     while  (1)  
     {  
          XNextEvent(display, &an_event);  
          /*�o�̴N�O�P�_�Ҧ��ƥ�*/  
   
           
          switch (an_event.type)  
          {  
                 
          case  KeyPress:  
               printf( "��L�Q���U" );  
               break ;  
          case  Expose:  /*��ø*/  
               if (an_event.xexpose.count > 0)       
               {  
                    break ;  
               }  
               XDrawLine(display, win, gc, 0, 100, 400, 100);  
               XDrawPoint(display, win, gc, 5, 5);  
               XDrawRectangle(display, win, gc, 120, 150, 50, 60);       
               XFillRectangle(display, win, gc, 60, 150, 50, 60);  
               /*��s*/  
               XFlush(display);  
               printf( "���b��ø�ƥ�\n" );  
               break ;  
          case  ButtonPress:  /*���U�ƥ�*/  
               /* int x; */  
               /* int y; */  
               /* x = an_event.xbutton.x; */  
               /* y = an_event.xbutton.window; */                 
               switch (an_event.xbutton.button){  
               case  Button1:  
                    XDrawRectangle(display, win, gc, 120, 150, 50, 60);       
                    puts( "���Х�����U" );  
                    break ;  
               case  Button2:       
                    puts( "button2..." );  
                    break ;  
               default :  
                    break ;  
               }  
          }                 
           
     }  
     /*����X�A�Ⱦ��s��*/  
     XCloseDisplay(display);  
     return  0;  
}  
   
   
/* 
  Display* display 
  ���V��ܵ��c�����w 
  Window parent 
  �s���f�������f��ID�C 
  int x 
  ���f�����WX���С]��쬰�̹������^ 
  int y 
  ���f�����WY���С]��쬰�̹������^ 
  unsigned int width 
  ���f���e�ס]��쬰�̹������^ 
  unsigned int height 
  ���f�����ס]��쬰�̹������^ 
  unsigned int border_width 
  ���f����ؼe�ס]��쬰�̹������^ 
  unsigned long border 
  �Ψ�ø�s���f��ت��C�� 
  unsigned long background 
  �Ψ�ø�s���f�I�����C�� 
* */  
   
/* 
  �b�ƥ󵲺c�̡A�q�L"an_event.xbutton"����o�ƥ������A�t�~���٥]�A�U���o�Ǧ��쪺���e�G 
  
  Window window 
  �ƥ�o�e���ؼе��f��ID�]�p�G�ڭ̬��h�ӵ��f���U�F�ƥ�^ 
  
  int x, y 
  �q���f�����W���к�_�A��������U�ɥ��Цb���f�������� 
  
  int button 
  ���ФW���Ӽи������s�Q���U�F�A�ȥi��OButton1�AButton2�AButton3 
  
  Time time 
  �ƥ�Q��i���C���ɶ��C�i�H�Q�Ψӹ�{�������B�z 
* */  
   
   
/* 
  
  ���Х��Ъ��i�J�M���}�ƥ� 
  
  �t�@�ӵ{�ǳq�`�|�P���쪺�ƥ�O�A�������Х��жi�J�@�ӵ��f�����H�����}���ӵ��f����쪺�ƥ�C���ǵ{�ǧQ�θӨƥ�ӧi�D�Τ�{�ǲ{�b�b�J�I�̭��C���F���U�o�بƥ�A�ڭ̱N�|�b���XSelectInput()�̵��U�X�ӭ���C 
  
  EnterWindowMask 
  �q���ڭ̹��Х��жi�J�F�ڭ̪����f�������N�@�� 
  
  LeaveWindowMask 
  �q���ڭ̹��Х������}�F�ڭ̪����f�������N�@�� 
  
  �ڭ̪��ƥ�`�����������ˬd�N�ˬd�H�U���ƥ����� 
  
  EnterNotify 
  ���Х��жi�J�F�ڭ̪����f 
  
  LeaveNotify 
  ���Х������}�F�ڭ̪����f 
  
  �o�Ǩƥ��������ƾڵ��c�q�L�Ҧp"an_event.xcrossing"�ӳX�ݡA���٥]�t�H�U���쪺�����ܶq�G 
  
  Window window 
  �ƥ�o�e���ؼе��f��ID�]�p�G�ڭ̬��h�ӵ��f���U�F�ƥ�^ 
  
  Window subwindow 
  �b�@�Ӷi�J�ƥ󤤡A�����N��O�q���Ӥl���f�i�J�ڭ̪����f�A�b�@�����}�ƥ󤤡A�����N��O�i�J�F���Ӥl���f�A�p�G�O"none"�A�����N��O�q�~���i�J�F�ڭ̪����f�C 
  
  int x, y 
  �q���f�����W���к�_�A�ƥ󲣥ͮɹ��Х��Цb���f�������� 
  
  int mode 
  ���ФW���Ӽи������s�Q���U�F�A�ȥi��OButton1�AButton2�AButton3 
  
  Time time 
  �ƥ�Q��i���C���ɶ��C�i�H�Q�Ψӹ�{�������B�z 
  
  unsigned int state 
  �o�Өƥ�o�ͮɹ��Ы��s�]�άO��L��^�Q���U�����p- �p�G�����ܡC�o�Ӧ����ϥΫ���Ϊ��覡�Ӫ�� 
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
  
  ���̪��W�r�O�i�H�X�i���A��Ĥ��ӹ��жs�Q���U�ɡA�ѤU���ݩʴN�����䥦�S����]�ҦpMod1�@��O"ALT"�Ϊ̬O"META"��^ 
  
  Bool focus 
  ��ȬOTrue���ɭԻ������f��o�F��L�J�I�AFalse�Ϥ� 
* */  
   
   
/* 
* ��L����U�M�P�}�ƥ� 
�p�G�ڭ̵{�Ǳ�����f��o�F��L�J�I�A���N�i�H�������䪺���U�M�P�}�ƥ�C���F���U�o�Ǩƥ������A�ڭ̴N�ݭn�q�L���XSelectInput()�ӵ��U�U��������C 
  
  
KeyPressMask 
�q���ڭ̪��{�Ǥ���ɭԫ���Q���U�F 
  
KeyPressMask 
�q���ڭ̪��{�Ǥ���ɭԫ���Q�P�}�F 
  
�ڭ̪��ƥ�`�����������ˬd�N�ˬd�H�U���ƥ����� 
  
Window window 
�ƥ�o�e���ؼе��f��ID�]�p�G�ڭ̬��h�ӵ��f���U�F�ƥ�^ 
  
unsigned int keycode 
�Q���U�]���P�}�^���䪺�s�X�C�o�O�@��X�����s�X�A���ӳQ½Ķ���@����L��Ÿ��~���K�ϥΡA�N�|�b�U�����СC 
  
int x, y 
�q���f�����W���к�_�A�ƥ󲣥ͮɹ��Х��Цb���f�������� 
  
Time time 
�ƥ�Q��i���C���ɶ��C�i�H�Q�Ψӹ�{�������B�z 
  
unsigned int state 
�o�Өƥ�o�ͮɹ��Ы��s�]�άO��L��^�Q���U�����p- �p�G�����ܡC�o�Ӧ����ϥΫ���Ϊ��覡�Ӫ�� 
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
  
���̪��W�r�O�i�H�X�i���A��Ĥ��ӹ��жs�Q���U�ɡA�ѤU���ݩʴN�����䥦�S����]�ҦpMod1�@��O"ALT"�Ϊ̬O"META"��^ */  
   
