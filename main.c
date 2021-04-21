#include <X11/X.h>
#include<X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

void spawn(char* arg[],Display *dpy){
		if(fork()==0){
				/*if(dpy)*/
					/*close(ConnectionNumber(dpy));*/
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
		char *menu[] = {"rofi","-show","run"};
		char *term[] = {"alacritty"};


		/*
		 * initialize display and checking whether any error or not
		 */

		dpy = XOpenDisplay(0x0);
		if (!dpy){
				return 1;
		}
		root = DefaultRootWindow(dpy);
		/*
		 * Grabs keyboard input to here we use Mod4Mask ,i.e., super key
		 * as mod key and "a" as combination
		 */
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("u")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,65293,Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("c")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("r")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);
		/*
		 * main event loop
		 */

		while (True){
				XNextEvent(dpy,&event);
				switch(event.type){
						/*If super+a is pressed we can raise a given window*/
						case KeyPress:
								kevent = event.xkey;
								ks = XKeycodeToKeysym(dpy,kevent.keycode, 0);
										if (event.xkey.subwindow){
												if(ks==117)
												{printf("pressed");
													XRaiseWindow(dpy,event.xkey.subwindow);
												}
												if(ks==99)
												{	printf("pressed");
													XDestroyWindow(dpy, event.xkey.subwindow);
												}
												if(ks==65293)
												{	printf("pressed");
													spawn(term,dpy);
												}
												if(ks==114)
												{	spawn(menu,dpy);
													printf("pressed");
												}
										}
													
										
								
								break;
						case ButtonPress:
										break;
						case MotionNotify: break;
						default: printf("Error!");
				}
		}
}
