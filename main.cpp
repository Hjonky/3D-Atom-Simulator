#include <iostream>
#include <algorithm>
#include "../3DEngine/window.h"
#include "../3DEngine/renderer.h"
#include "particle.h"
#include "Simulator.h"


std::vector<uint32_t> framebuffer(WIDTH * HEIGHT, 0xFF000000);

int main() 
{
    Simulator* activeSim = nullptr;
    AtomSimulator* atomSim = new AtomSimulator();
    activeSim = atomSim;

    RenderWindow window(WIDTH, HEIGHT, framebuffer.data());

    bool prevLeft = false;
    bool prevRight = false;
    bool isPaused = false;
    while (window.ProcessEvents()) 
    {
        char key = window.GetLastKey();
        if (key == ' ') 
        {
            isPaused = !isPaused;
        }
        if (key == 'c') 
        {
            activeSim->GetParticles().clear();
        }

        bool currentLeft = window.IsLeftMouseDown();
        if (currentLeft && !prevLeft) 
        {
            float randomZ = (rand() % 100 / 10.0f) + 3.0f;
            float worldX = (window.GetMouseX() - (WIDTH / 2.0f)) * (5.0f / FOV);
            float worldY = (window.GetMouseY() - (HEIGHT / 2.0f)) * (5.0f / FOV);

            activeSim->GetParticles().push_back(Proton({worldX, worldY, randomZ}));
        }
        prevLeft = currentLeft;
        
        bool currentRight = window.IsRightMouseDown();
        if (currentRight && !prevRight) 
        {
            float randomZ = (rand() % 100 / 10.0f) + 3.0f;
            float worldX = (window.GetMouseX() - (WIDTH / 2.0f)) * (5.0f / FOV);
            float worldY = (window.GetMouseY() - (HEIGHT / 2.0f)) * (5.0f / FOV);

            activeSim->GetParticles().push_back(Electron({worldX, worldY, randomZ}));
        }
        prevRight = currentRight;

        std::fill(framebuffer.begin(), framebuffer.end(), 0xFF000000);

        if (!isPaused)
        {
            activeSim->Update(0.008f);
        }


        // sort particles by Z order to add Z-depth
        auto& particles = activeSim->GetParticles();
        std::sort(particles.begin(), particles.end(), [](const auto& a, const auto& b) {
            return a.position.z > b.position.z;
        });

        // Grab the active particles and draw them
        for (const auto& p : activeSim->GetParticles()) 
        {
            Vec2 proj = Project(p.position);
            
            // Fade out as Z gets higher (assuming your camera is at Z=0)
            float fade = std::max(0.1f, 1.0f - (p.position.z / 20.0f)); 
            
            // Bitshift the faded values into a 32-bit pixel (0xAARRGGBB)
            uint8_t r = (p.charge > 0.0f) ? (255 * fade) : 0;
            uint8_t b = (p.charge < 0.0f) ? (255 * fade) : 0;
            uint32_t color = 0xFF000000 | (r << 16) | b;

            int screenRadius = (p.radius / p.position.z) * FOV; 
            DrawCircle(proj, screenRadius, color);
        }

        window.Present();
    }
    
    delete activeSim;
    return 0;
}
