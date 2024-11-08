#include "ParticleSystem.h"


//ParticleSystem::ParticleSystem(int maxParticles) : maxParticles(maxParticles) {}
ParticleSystem::ParticleSystem(int maxParticles) : maxParticles(maxParticles) {
    std::srand(static_cast<unsigned>(std::time(0))); // Seed the random number generator
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::createParticles(){
    while(particles.size() < maxParticles){
        float x = std::rand() % 256;
        float y = std::rand() % 192;
        float xVel = static_cast<float>(std::rand() % 30 - 10)/250.0f; // Random velocity
        float yVel = static_cast<float>(std::rand() % 30 - 10)/250.0f;
        //float xVel = std::rand() % 100;
        //float yVel = std::rand() % 100;
        Uint8 r = std::rand() % 256;
        Uint8 g = std::rand() % 256;
        Uint8 b = std::rand() % 256;
        //Uint8 a = std::rand() % 256;
        Uint8 a = 255;//full opacity
        float lifetime = 9000.0f + (std::rand() % 5000) / 1000.0f;
        Particle tempParticle{x,y,xVel,yVel,r,g,b,a,lifetime};
        addParticle(tempParticle);
    }
}

void ParticleSystem::addParticle(const Particle& particle) {
    if (particles.size() < maxParticles) {
        particles.push_back(particle);
    }
}
 //std::remove_if was not declared in this scope.
void ParticleSystem::update(float deltaTime) {
    if(particles.size() < maxParticles){
        createParticles();
    }

    for (auto& particle : particles) {

        particle.x += particle.xVel * deltaTime;
        particle.y += particle.yVel * deltaTime;
        particle.lifetime -= deltaTime;
    }

    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& p) {
        return p.lifetime <= 0;
    }), particles.end());
}
/*
void ParticleSystem::render(SDL_Renderer* renderer) {
    for (const auto& particle : particles) {
        SDL_SetRenderDrawColor(renderer, particle.r, particle.g, particle.b, particle.a);
        SDL_RenderDrawPoint(renderer, static_cast<int>(particle.x), static_cast<int>(particle.y));
    }
}*/

void ParticleSystem::render(SDL_Renderer* renderer) {
    int particleSize = 4; // Set the size of the particles
    for (const auto& particle : particles) {
        SDL_Rect particleRect = { static_cast<int>(particle.x), static_cast<int>(particle.y), particleSize, particleSize };
        SDL_SetRenderDrawColor(renderer, particle.r, particle.g, particle.b, particle.a);
        SDL_RenderFillRect(renderer, &particleRect); // Render filled rectangle for larger particles
    }
}
