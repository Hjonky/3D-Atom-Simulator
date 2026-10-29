#include "Simulator.h"


int AtomSimulator::GetCell(float coordinate) 
{
    int index = static_cast<int>((coordinate / CELL_SIZE) + (GRID_SIZE / 2.0f));
    return std::max(0, std::min(index, GRID_SIZE - 1));
}

void AtomSimulator::Collision(Particle& p1, Particle& p2, Vec3 distance, float r2)
{
    // Calculate the total radius for intersection
    float r_total = p1.radius + p2.radius;
    if (r2 < (r_total * r_total)) 
    {
        // Calculate Euclidean distance and find the normalized unit vector (distance_hat)
        float r = std::sqrt(r2);
        Vec3 distance_hat = (r > 0.0001f) ? distance * (1.0f / r) : Vec3{1.0f, 0.0f, 0.0f};

        // Calculate inverse masses to weigh position and velocity adjustments
        float invMassI = 1.0f / p1.mass;
        float invMassJ = 1.0f / p2.mass;
        float invMassSum = invMassI + invMassJ;

        // Push overlapping particles apart based on their relative mass ratios
        float overlap = r_total - r;
        p1.position = p1.position - (distance_hat * (overlap * (invMassI / invMassSum)));
        p2.position = p2.position + (distance_hat * (overlap * (invMassJ / invMassSum)));

        // Calculate relative velocity and project it onto the collision normal vector
        Vec3 v_reletive = p2.velocity - p1.velocity;
        float v_normal = v_reletive.Dot(distance_hat);

        // Only apply a response impulse if the particles are actively moving toward each other
        if (v_normal < 0.0f) 
        {
            // Calculate scalar impulse magnitude for a perfectly elastic collision
            float impulse = -(2.0f * v_normal) / ((1.0f / p1.mass) + (1.0f / p2.mass)); 
            Vec3 J = distance_hat * impulse;

            // Apply the final impulse vector to update both particle velocities
            p1.velocity = p1.velocity - (J * invMassI);
            p2.velocity = p2.velocity + (J * invMassJ);
        }
    }
}


void AtomSimulator::Update(float dt) 
{
   
    // Clear the grid (fast, does not free memory allocations)
    for (int x = 0; x < GRID_SIZE; x++)
        for (int y = 0; y < GRID_SIZE; y++)
            for (int z = 0; z < GRID_SIZE; z++)
                grid[x][y][z].clear();

    // Map particles to their current cells
    for (auto& p : particles) {
        int cx = GetCell(p.position.x);
        int cy = GetCell(p.position.y);
        int cz = GetCell(p.position.z);
        grid[cx][cy][cz].push_back(&p);
    }

    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); i++) 
    {
        Particle& p = particles[i];

        int cx = GetCell(p.position.x);
        int cy = GetCell(p.position.y);
        int cz = GetCell(p.position.z);

        // Loop through the 3x3x3 neighbor block (clamped to prevent edge crashes)
        for (int nx = std::max(0, cx - 1); nx <= std::min(GRID_SIZE - 1, cx + 1); nx++) 
        {
            for (int ny = std::max(0, cy - 1); ny <= std::min(GRID_SIZE - 1, cy + 1); ny++) 
            {
                for (int nz = std::max(0, cz - 1); nz <= std::min(GRID_SIZE - 1, cz + 1); nz++) 
                {
                    
                    for (Particle* neighbor : grid[nx][ny][nz]) 
                    {
                        if (&p == neighbor) continue; // Skip self

                        // Calculates distance vector d and squared distance r^2.
                        Vec3 d = neighbor->position - p.position;
                        float r2 = (d.x*d.x) + (d.y*d.y) + (d.z*d.z);

                        // Pass *neighbor safely
                        Collision(p, *neighbor, d, r2);

                        // Calculates Coulomb electrostatic force using k_e, charges, and r^2.
                        float F_electrostatic = k_e*(neighbor->charge * p.charge) / (r2 + softening);

                        // Use The Lennard-Jones force model
                        if (p.charge > 0.0f && neighbor->charge > 0.0f) 
                        {
                            float r = std::sqrt(r2);
                            // Only apply at close range to save CPU
                            if (r > 0.0001f && r < 4.0f) 
                            {
                                float sigma = 0.8f; // Desired distance between protons
                                float epsilon = 5.0e-28f; // Force Strength multiplier

                                // Clamp the distance to prevent floating-point infinity explosions
                                float safe_r = std::max(r, 0.3f);

                                float sr = sigma / safe_r;
                                float sr6 = sr * sr * sr * sr * sr * sr; // (sigma / r)^6
                                float sr12 = sr6 * sr6;                  // (sigma / r)^12

                                // Calculate the force magnitude
                                float F_strong = 24.0f * epsilon * ((2.0f * sr12) - sr6) / safe_r;

                                // Subtract from the repulsive electrostatic force
                                F_electrostatic -= F_strong;
                            }
                        }
                        // Multiplys electrostatic force by the normalized d vector
                        Vec3 F = (d.Normalize()) * F_electrostatic;
                        
                        // Applys force to p
                        p.ApplyForce(F * -1.0f);
                    }
                }
            }
        }
    }
    for (auto& p : particles) 
    {
        p.Update(dt);
    }
}