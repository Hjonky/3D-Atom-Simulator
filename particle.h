#pragma once
#include "../3DEngine/renderer.h" 

typedef struct Particle
{
    Vec3 position;
    Vec3 old_position = position;
    Vec3 velocity = {0, 0, 0};
    Vec3 force = {0, 0, 0};
    float mass = 1.0f;
    float charge = 0.0f;
    float radius = 0.5f;

    void ApplyForce(Vec3 f) { force += f;}

    void Update(float dt) 
    {
        // Figure out how much we moved based on the last frame
        Vec3 movement = position - old_position;
        
        old_position = position;

        Vec3 acceleration = force / mass;
        velocity += (acceleration * dt);

        // Verlet position update
        position = position + (movement * 0.99f) + (acceleration * (dt * dt));

        // Keep the velocity field updated for the rest of your code
        velocity = movement / dt;

        force = {0, 0, 0};
    }
} Particle;


// define the proton
typedef struct Proton : public Particle 
{
    Proton(Vec3 pos) 
    {
        position = pos;
        old_position = pos;
        charge = 1.602e-19f;
        mass = 1.672e-27f;
        radius = 0.5f;
    }
} Proton;

// define the electron
typedef struct Electron : public Particle 
{
    Electron(Vec3 pos) 
    {
        position = pos;
        old_position = pos;
        charge = -1.602e-19f;
        mass = 9.109e-31f;
        radius = 0.5f;
    }
} Electron;