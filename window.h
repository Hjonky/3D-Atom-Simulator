#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>

class RenderWindow
{
private:
    Display* display;
    Window window;
    GC gc;
    XImage* image;
    int width;
    int height;

    int mouseX = 0, mouseY = 0;
    bool leftDown = false, rightDown = false;
    char lastKey = 0;

public:
    // Takes the dimensions and a pointer to whatever pixel array you want to draw
    RenderWindow(int w, int h, uint32_t* bufferData);
    ~RenderWindow();

    int GetMouseX() const { return mouseX; }
    int GetMouseY() const { return mouseY; }
    bool IsLeftMouseDown() const { return leftDown; }
    bool IsRightMouseDown() const { return rightDown; }
    char GetLastKey();
    
    bool ProcessEvents();
    void Present();
};