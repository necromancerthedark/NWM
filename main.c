#include <X11/X.h>
#include<X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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

int main(){
		Display *dpy;
		char *list;
		Window root;
		XWindowAttributes attr;
		XButtonEvent bevent;
		XEvent event; 
		XKeyEvent kevent;
		KeySym ks;
		char *menu[] = {"rofi","-show","run",NULL};
		char *term[] = {"alacritty",NULL};
		char *wall[] = {"nitrogen","--restore",NULL};
		int xdiff, ydiff;


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
		 * as mod key and "a" as combination
		 */
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("u")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("t")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("c")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("r")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);

    XGrabButton(dpy, 1, Mod4Mask, root, True, ButtonPressMask, GrabModeAsync,
            GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod4Mask, root, True, ButtonPressMask, GrabModeAsync,
            GrabModeAsync, None, None);
		/*
		 * main event loop
		 */

		while (True){
				XNextEvent(dpy,&event);
				switch(event.type){
						case CreateNotify:
								break;
						/*If super+a is pressed we can raise a given window*/
						case KeyPress:
								kevent = event.xkey;
								ks = XKeycodeToKeysym(dpy,kevent.keycode, 0);
										if (event.xkey.subwindow){
												if(ks==117)
												{printf("pressed u");
													XRaiseWindow(dpy,event.xkey.subwindow);
												}
												if(ks==99)
												{	printf("pressed c");
													XDestroyWindow(dpy, event.xkey.subwindow);
												}
												if(ks==116)
												{	printf("pressed enter");
													spawn(term,dpy);
												/*if(ks==65293)*/
												}
												if(ks==114)
												{	spawn(menu,dpy);
													printf("pressed r");
												}
										}
													
										
								
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
