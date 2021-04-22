#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

char *menu[] = {"rofi","-show","run",NULL};
char *term[] = {"alacritty",NULL};
char *wall[] = {"nitrogen","--restore",NULL};

void spawn(char* arg[],Display *dpy);
void keypressfunc(KeySym Sym,Display *dpy,Window sw);

int main(){
		Window root;
		Display *dpy;
		char *list;
		XWindowAttributes attr;
		XButtonEvent bevent;
		XEvent event; 
		XKeyEvent kevent;
		KeySym ks;
		int xdiff, ydiff;
		bool running=true;


		/*
		 * initialize display and checking whether any error or not
		 */

		dpy = XOpenDisplay(0x0);
		if (!dpy){
				return 1;
		}
		root = DefaultRootWindow(dpy);
		spawn(wall,dpy);

		/*
		 * Grabs keyboard input to here we use Mod4Mask ,i.e., super key
		 */

		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("u")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("t")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("c")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("r")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);

		/*
		 *Uses mouse input + Super key to perform certain tasks
		 * 1: Left click
		 * 3: Right click
		 */

		XGrabButton(dpy, 1, Mod4Mask, root, True, ButtonPressMask, GrabModeAsync,GrabModeAsync, None, None);
		XGrabButton(dpy, 3, Mod4Mask, root, True, ButtonPressMask, GrabModeAsync,GrabModeAsync, None, None);

		/*
		 * main event loop
		 */

		while (running){
				XNextEvent(dpy,&event);

				switch(event.type){
						case CreateNotify:
								break;
						case KeyPress:
								kevent = event.xkey;
								ks = XkbKeycodeToKeysym(dpy,kevent.keycode,0,0);	
										if (event.xkey.subwindow)
											keypressfunc(ks,dpy,event.xkey.subwindow);
								break;
						case ButtonPress:
								XGrabPointer(dpy, event.xbutton.subwindow, True,
									PointerMotionMask|ButtonReleaseMask, GrabModeAsync,
									GrabModeAsync, None, None, CurrentTime);
								XGetWindowAttributes(dpy, event.xbutton.subwindow, &attr);
								bevent = event.xbutton;
								break;
						case MotionNotify: 
								while(XCheckTypedEvent(dpy, MotionNotify, &event));
								xdiff = event.xbutton.x_root - bevent.x_root;
								ydiff = event.xbutton.y_root - bevent.y_root;
								XMoveResizeWindow(dpy, event.xmotion.window,
								   	attr.x + (bevent.button==1 ? xdiff : 0),
									attr.y + (bevent.button==1 ? ydiff : 0),
									MAX(1, attr.width + (bevent.button==3 ? xdiff : 0)),
									MAX(1, attr.height + (bevent.button==3 ? ydiff : 0)));
								break;
						case ButtonRelease:
								XUngrabPointer(dpy,CurrentTime);
								break;
						case KeyRelease:
								break;
						default: printf("Error!");
				}
		}
}

void spawn(char* arg[],Display *dpy){
		if(fork()==0){
				if(dpy)
					close(ConnectionNumber(dpy));
				setsid();
				int status_code = execvp(arg[0],arg);
				if (status_code==-1){
						printf("Error Occured!");
				}
		}
}

void keypressfunc(KeySym Sym,Display *dpy,Window sw){
		switch(Sym){
				case XK_r:   /*Super + r*/
					spawn(menu,dpy);
					break;
				case XK_t:  /*Super + t*/
					spawn(term,dpy);
					break;
				case XK_u: /*Super + u*/
					XRaiseWindow(dpy,sw);
					break;
				case XK_c: /*Super + c*/
					XDestroyWindow(dpy,sw);
					break;
				default: break;

		}
} 
