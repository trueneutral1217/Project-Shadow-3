#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <SDL.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
//#include <ctime>
#include <iostream>

struct Particle {
    float x, y;
    float xVel, yVel;
    Uint8 r, g, b, a;
    float lifetime;
};

class ParticleSystem {
public:
    ParticleSystem(int maxParticles);
    ~ParticleSystem();
    void addParticle(const Particle& particle);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void createParticles();

private:
    std::vector<Particle> particles;
    int maxParticles;
};

#endif // PARTICLESYSTEM_H
