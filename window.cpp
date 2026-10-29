#include "window.h"
#include <unistd.h>

RenderWindow::RenderWindow(int w, int h, uint32_t* bufferData) : width(w), height(h) 
{
    display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    
    // Create the OS window
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 
                                 10, 10, width, height, 1, 
                                 BlackPixel(display, screen), BlackPixel(display, screen));
    
    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XMapWindow(display, window);
    gc = DefaultGC(display, screen);

    // Bind your external pixel array to the X11 image
    image = XCreateImage(display, DefaultVisual(display, screen), DefaultDepth(display, screen),
                         ZPixmap, 0, (char*)bufferData, width, height, 32, 0);
}

RenderWindow::~RenderWindow() 
{
    XDestroyImage(image); // This automatically frees the image structure
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

char RenderWindow::GetLastKey()
{ 
    char k = lastKey; 
    lastKey = 0; // Reset it after reading so we don't trigger it twice
    return k; 
}

bool RenderWindow::ProcessEvents()
{
    XEvent event;
    while (XPending(display) > 0) 
    {
        XNextEvent(display, &event);
        
        switch (event.type) 
        {
            case KeyPress:
            {
                char buffer[10];
                XLookupString(&event.xkey, buffer, sizeof(buffer), nullptr, nullptr);
                if (buffer[0] == 27) return false; // 27 is the ASCII code for ESC (closes the app)
                
                lastKey = buffer[0];
                break;
            }
            case MotionNotify: mouseX = event.xmotion.x; mouseY = event.xmotion.y; break;
            case ButtonPress: 
            {
                if (event.xbutton.button == 1) leftDown = true;
                if (event.xbutton.button == 3) rightDown = true;
                mouseX = event.xmotion.x;
                mouseY = event.xmotion.y;
                break;
            }
            case ButtonRelease: 
            {
                if (event.xbutton.button == 1) leftDown = false;
                if (event.xbutton.button == 3) rightDown = false;
                break;
            }
        }
    }
    return true; // Keep running
}

void RenderWindow::Present()
{
    XPutImage(display, window, gc, image, 0, 0, 0, 0, width, height);
    XFlush(display);
    usleep(16000); // Caps framerate to ~60 FPS
}