#include "ParticleSystem.hpp"

Particle::Particle(AEVec2 pos) 
: pos{ pos }, vel( {0.f,0.f} ), curr_col{ 0 }, curr_row{ 0 }, elapsed_dur{ 0.f }, dur{ 0.f }, scale{ 1.f }, scale_og{ 1.f } {
}


ParticleSystem::ParticleSystem() {
	particles.push_back(std::make_unique<Particle>());
}

ParticleSystem::~ParticleSystem() {
	particles.clear();
}

void ParticleSystem::Update(const f32& dt) {

}

void ParticleSystem::Render() {

}
