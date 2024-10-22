#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int maxParticles) : maxParticles(maxParticles) {}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::addParticle(const Particle& particle) {
    if (particles.size() < maxParticles) {
        particles.push_back(particle);
    }
}
 //std::remove_if was not declared in this scope.
void ParticleSystem::update(float deltaTime) {
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
