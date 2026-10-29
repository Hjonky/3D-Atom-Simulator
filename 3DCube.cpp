#include "renderer.h"
#include "window.h"
#include <vector>
#include <algorithm>

// Initialize the shared framebuffer with solid black pixels
std::vector<uint32_t> framebuffer(WIDTH * HEIGHT, 0xFF000000);

std::vector<Vec3> vertices = 
{
    {-1, -1, -1}, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1}, // Front
    {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, {-1,  1,  1}  // Back
};

std::vector<std::vector<int>> triangles = 
{
    {0, 1, 2}, {0, 2, 3}, // Front
    {1, 5, 6}, {1, 6, 2}, // Right
    {5, 4, 7}, {5, 7, 6}, // Back
    {4, 0, 3}, {4, 3, 7}, // Left
    {3, 2, 6}, {3, 6, 7}, // Top
    {4, 5, 1}, {4, 1, 0}  // Bottom
};

int main() 
{
    // Instantiate your new encapsulated window wrapper
    RenderWindow window(WIDTH, HEIGHT, framebuffer.data());
    float angle = 0.0f;

    // The clean engine loop
    while (window.ProcessEvents()) 
    {
        std::fill(framebuffer.begin(), framebuffer.end(), 0xFF000000);

        angle += 0.02f; 

        for (int i = 0; i < triangles.size(); i++)
        {
            Vec3 v0 = vertices[triangles[i][0]];
            Vec3 v1 = vertices[triangles[i][1]];
            Vec3 v2 = vertices[triangles[i][2]];

            v0 = RotateX(v0, angle);
            v0 = RotateY(v0, angle);
            v0 = RotateZ(v0, angle);

            v1 = RotateX(v1, angle);
            v1 = RotateY(v1, angle);
            v1 = RotateZ(v1, angle);

            v2 = RotateX(v2, angle);
            v2 = RotateY(v2, angle);
            v2 = RotateZ(v2, angle);

            v0.z += 3.0f;
            v1.z += 3.0f;
            v2.z += 3.0f;

            Vec2 p0 = Project(v0);
            Vec2 p1 = Project(v1);
            Vec2 p2 = Project(v2);

            DrawWireframeTriangle(p0, p1, p2, 0xFF00FF00); 
        }

        // Push the drawn frame to the screen and sleep
        window.Present();
    }
    
    return 0;
}