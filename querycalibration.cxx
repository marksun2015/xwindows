//
// "$Id: threads.cxx,v 1.13 2002/12/13 19:42:59 easysw Exp $"
//
// Threading example program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <fcntl.h>
#include <fltk/Widget.h>
#include <fltk/Window.h>
#include <fltk/run.h>
#include <fltk/Style.h>
#include <fltk/Button.h>
#include <fltk/Input.h>
#include <fltk/Box.h>
#include <fltk/LabelType.h>
#include <fltk/InvisibleBox.h>
#include <fltk/Output.h>
#include <fltk/string.h>
#include <fltk/events.h>
#include <fltk/Flags.h>
#include <fltk/Browser.h>
#include <fltk/Choice.h>
#include <fltk/ask.h>
#include <fltk/Color.h>
#include <fltk/ProgressBar.h>
#include <FL/fl_ask.H>
#include <time.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/reboot.h>

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <libgen.h>
#include <ctype.h>

#include <fltk/x.h>

#include <dlfcn.h>
#include "defs.h"
#include "emt_platform.h"

#include <libintl.h>
#include <locale.h>

#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

using namespace fltk;

fltk::Font *pfont = fltk::font("Weintek Sans");

using namespace fltk;


//#define DBGMSG( x... )  fprintf(stdout, ##x )
#define DBGMSG( x... )

#define ASW_YES             1
#define ASW_NO              0

#if 1
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define MWM_HINTS_FUNCTIONS     (1L << 0)
#define PROP_MWM_HINTS_ELEMENTS 5

#define MWM_FUNC_ALL            (1L << 0)
#define MWM_FUNC_RESIZE         (1L << 1)
#define MWM_FUNC_MOVE           (1L << 2)
#define MWM_FUNC_MINIMIZE       (1L << 3)
#define MWM_FUNC_MAXIMIZE       (1L << 4)
#define MWM_FUNC_CLOSE          (1L << 5)


#define MWM_DECOR_ALL           (1L << 0)
#define MWM_DECOR_BORDER        (1L << 1)
#define MWM_DECOR_RESIZEH       (1L << 2)
#define MWM_DECOR_TITLE         (1L << 3)
#define MWM_DECOR_MENU          (1L << 4)
#define MWM_DECOR_MINIMIZE      (1L << 5)
#define MWM_DECOR_MAXIMIZE      (1L << 6)
typedef struct _mwmhints {
        uint32_t flags;
        uint32_t functions;
        uint32_t decorations;
        int32_t  input_mode;
        uint32_t status;
} MWMHints;
#endif


static int small_lcd;

void relocate_pos(int x,int y)
{
	small_lcd = 0;

	if (x<640 && y<=480) {
		small_lcd =1;
	}
	else{
		small_lcd =0;
	}

}


void ShowAboveAll(const fltk::Window * win, bool active) {
	#if !WIN32 // XOrg / XWindows(TM)
	XEvent ev;
	static char *names[ ] = { "_NET_WM_STATE",  "_NET_WM_STATE_ABOVE" };
	Atom atoms[ 2 ];
	fltk::open_display();
#if 1
      MWMHints mwmhints;
      Atom prop;
      memset(&mwmhints, 0, sizeof(mwmhints));
      prop = XInternAtom(fltk::xdisplay, "_MOTIF_WM_HINTS", False);
      mwmhints.flags = MWM_HINTS_DECORATIONS;
      mwmhints.decorations = MWM_DECOR_TITLE|MWM_DECOR_BORDER |MWM_DECOR_RESIZEH;
       mwmhints.functions= 0;
     XChangeProperty(fltk::xdisplay, fltk::xid(win), prop, prop, 32, PropModeReplace,
                      (unsigned char *) &mwmhints,
                      PROP_MWM_HINTS_ELEMENTS);
#endif
	XInternAtoms(fltk::xdisplay, names, 2, False, atoms );
	Atom net_wm_state = atoms[ 0 ];
	Atom net_wm_state_above = atoms[ 1 ];
	ev.type = ClientMessage;
	ev.xclient.window = fltk::xid(win);
	ev.xclient.message_type = net_wm_state; ev.xclient.format = 32;
	ev.xclient.data.l[ 0 ] = active ? 1 : 0;
	ev.xclient.data.l[ 1 ] = net_wm_state_above; ev.xclient.data.l[ 2 ] = 0;
	XSendEvent(fltk::xdisplay,  DefaultRootWindow(fltk::xdisplay),  False,  SubstructureNotifyMask|SubstructureRedirectMask,  &ev);

	#else // Microsoft (R) Windows(TM)
	SetWindowPos(fltk::xid(win), HWND_TOPMOST, 0, 0, 0, 0, 0);
	#endif
}


static void yes_cb(Widget *widget, void *v)
{
	int * number = (int*)v;

	(*number) = ASW_YES;
	((Button*)widget)->window()->destroy();
}

static void no_cb(Widget *widget, void *v)
{
	int * number = (int*)v;
	(*number) = ASW_NO;
	((Button*)widget)->window()->destroy();
}


 int query_dlg(int w, int h, int pw, int ph)
{
	int x = (pw-w)/2;
	int y = (ph-h)/2;
	int user_answer = 0;
	int bias = 0;

	int fontsize=14;

	int yn_x=0,yn_y=0;
	int yn_w=0,yn_h=0;
	int t_w=0;
	int width=0,high=0;


	if (small_lcd){
		width=240;
		high=70;
		t_w=230;
		yn_x=160;
		yn_y=35;
		yn_w=68;
		yn_h=30;
		fontsize=12;
		x = (pw-w)/2-70;
		y = (ph-h)/2;
	}else{
		width=280;
		high=90;
		t_w=260;
		yn_x=200;
		yn_y=45;
		yn_w=68;
		yn_h=35;
		fontsize=14;
	}


	Window * wquery = new Window( width, high, _("Ask"));
	wquery->labelfont(pfont);
	wquery->begin();

	InvisibleBox *iba = new InvisibleBox(5, 5, t_w, 20, _("Continue to touch panel calibration?"));

	iba->labelfont(pfont);
	iba->labelsize(fontsize);

	Button * bty = new Button(yn_x,yn_y , yn_w, yn_h, _("&Yes"));
	Button * btn = new Button(yn_x-84, yn_y , yn_w, yn_h, _("&No"));

	bty->labelfont(pfont);
	bty->labelsize(fontsize);
	btn->labelfont(pfont);
	btn->labelsize(fontsize);

	bty->callback(yes_cb, &user_answer);
	btn->callback(no_cb, &user_answer);

	wquery->end();
	wquery->position(x, y);
	wquery->show();

	ShowAboveAll(wquery,true);

	wquery->exec();

	return user_answer;
}

int main()
{
    setlocale(LC_ALL,"");

    bindtextdomain("querycalibration", "/root/po");
    textdomain("querycalibration");

	int ret;
	int w,h;
	char tmp;
	const char *p;
	int xres=800;
	int yres=480;

	//p = gettypename();

	platform_get_resolution(&xres,&yres);

	relocate_pos(xres,yres);

	if(query_dlg(xres/4+60, yres/8+30, xres, yres)){
		system("/root/touch_calibrate.sh &");
	}

  	return 0;
}


