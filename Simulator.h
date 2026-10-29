#pragma once
#include "particle.h"
#include <vector>

// Base interface
class Simulator {
public:
    virtual void Update(float dt) = 0;
    virtual std::vector<Particle>& GetParticles() = 0;
    virtual ~Simulator() = default;
};

// Atom Simulator
class AtomSimulator : public Simulator 
{
private:
void Collision(Particle& p1, Particle& p2, Vec3 distance, float r2);

public:
    static constexpr float CELL_SIZE = 4.0f;
    static constexpr int GRID_SIZE = 30; // Creates a 30x30x30 grid
    std::vector<Particle*> grid[GRID_SIZE][GRID_SIZE][GRID_SIZE];
    
    std::vector<Particle> particles;
    static constexpr float k_e = 8.987e9f; // Coulomb's constant
    float softening = 0.0001f; // Prevents division by zero crashes


    std::vector<Particle>& GetParticles() override { return this->particles; };
    int GetCell(float coordinate);
    void Update(float dt) override;
    virtual ~AtomSimulator() = default;
};