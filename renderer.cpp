#include "renderer.h"
#include <cmath>
#include <algorithm>

void SetPixel(Vec2 p, uint32_t color) 
{
    // Bounds checking so you don't segfault
    if (p.x >= 0 && p.x < WIDTH && p.y >= 0 && p.y < HEIGHT) 
    {
        framebuffer[(p.y * WIDTH) + p.x] = color;
    }
}


void DrawLine(Vec2 p0, Vec2 p1, uint32_t color) 
{
    int dx = std::abs(p1.x-p0.x);
    int dy = std::abs(p1.y-p0.y);
    int sx = p0.x < p1.x ? 1 : -1;
    int sy = p0.y < p1.y ? 1 : -1;
    int err = dx - dy;
    

    while (true) 
    {
        SetPixel(p0, color);
        if (p0.x == p1.x && p0.y == p1.y) break;
        int e2 = 2 * err;

        if (e2 > (-dy)) 
        {
            err -= dy;
            p0.x += sx;
        }
        if (e2 < dx) 
        {
            err += dx;
            p0.y += sy;
        }
    }
    
}

void DrawCircle(Vec2 center, int radius, uint32_t color) 
{
    int r2 = radius * radius;
    for (int y = -radius; y <= radius; y++) 
    {
        for (int x = -radius; x <= radius; x++) 
        {
            if (x*x + y*y <= r2) 
            {
                SetPixel({center.x + x, center.y + y}, color);
            }
        }
    }
}

void DrawFilledTriangle(Vec2 p0, Vec2 p1, Vec2 p2, uint32_t color)
{
    int minX = std::min(p0.x, std::min(p1.x,p2.x));
    int maxX = std::max(p0.x, std::max(p1.x,p2.x));
    int minY = std::min(p0.y, std::min(p1.y, p2.y));
    int maxY = std::max(p0.y, std::max(p1.y,p2.y));
    int w0 = 0;
    int w1 = 0;
    int w2 = 0;
    
    for (int y = minY; y < maxY; ++y) 
    {
        for (int x = minX; x < maxX; ++x) 
        {
            w0 = (p2.x-p1.x)*(y-p1.y) - (p2.y - p1.y)*(x-p1.x);
            w1 = (p0.x-p2.x)*(y-p2.y) - (p0.y - p2.y)*(x-p2.x);
            w2 = (p1.x-p0.x)*(y-p0.y) - (p1.y - p0.y)*(x-p0.x);
            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) 
            {
                SetPixel({ x,y }, color);
            }
        }
    }
}

void DrawWireframeTriangle(Vec2 p0, Vec2 p1, Vec2 p2, uint32_t color) 
{
    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p0, color);
}


Vec2 Project(Vec3 p) 
{
    int xProj = (((p.x / p.z) * FOV) + WIDTH / 2);
    int yProj = (((p.y / p.z) * FOV) + HEIGHT / 2);

    return { xProj, yProj };
}


Vec3 RotateY(Vec3 p, float theta)
{
    float rotX = p.x * std::cos(theta) + p.z * std::sin(theta);
    float rotZ = p.z * std::cos(theta) - p.x * std::sin(theta);

    return {rotX, p.y, rotZ};
}

Vec3 RotateX(Vec3 p, float theta) 
{
    float rotY = p.y * std::cos(theta) - p.z * std::sin(theta);
    float rotZ = p.y * std::sin(theta) + p.z * std::cos(theta);
    return {p.x, rotY, rotZ};
}

Vec3 RotateZ(Vec3 p, float theta) 
{
    float rotX = p.x * std::cos(theta) - p.y * std::sin(theta);
    float rotY = p.x * std::sin(theta) + p.y * std::cos(theta);
    return {rotX, rotY, p.z};
}
