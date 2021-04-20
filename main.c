#include<X11/Xlib.h>
#include <stdio.h>
#include <X11/Xutil.h>

int main(){
		Display *dpy;
		Window root;
		XWindowAttributes attr;
		XButtonEvent bevent;
		XEvent event; 


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
		XGrabKey(dpy,XKeysymToKeycode(dpy,XStringToKeysym("a")),Mod4Mask,root,True,GrabModeAsync,GrabModeAsync);

		/*
		 * main event loop
		 */

		while (True){
				XNextEvent(dpy,&event);
				switch(event.type){
						case ButtonPress:
										break;
						case MotionNotify: break;
						default: printf("Error!");
				}
		}
}
