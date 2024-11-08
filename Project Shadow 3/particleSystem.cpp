#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(int maxParticles) : maxParticles(maxParticles) {}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::createParticles(){
    for(int i = particles.size(); i < maxParticles; ++i){
        float x = std::rand() % 256;
        float y = std::rand() % 192;
        float xVel = std::rand() % 100;
        float yVel = std::rand() % 100;
        Uint8 r = std::rand() % 256;
        Uint8 g = std::rand() % 256;
        Uint8 b = std::rand() % 256;
        Uint8 a = std::rand() % 256;
        float lifetime = 1000 + (std::rand() % 3000);
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

void ParticleSystem::render(SDL_Renderer* renderer) {
    for (const auto& particle : particles) {
        SDL_SetRenderDrawColor(renderer, particle.r, particle.g, particle.b, particle.a);
        SDL_RenderDrawPoint(renderer, static_cast<int>(particle.x), static_cast<int>(particle.y));
    }
}
