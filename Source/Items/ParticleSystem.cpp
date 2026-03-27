/*!
* @file ParticleSystem.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 8 February 2026
* @course CSD1451
* @brief Definition file for a particle system within a scene
*/

#include "ParticleSystem.hpp"
#include "../Utils/AEOverload.hpp"
#include <iostream>

ParticleData::ParticleData(std::string const& filepath, u8 rows, u8 cols)
: rows{ rows }, cols{ cols }, 
  sprite{ AssetManager::GetSpriteSheet(filepath, rows, cols) } {} // Ctor

ParticleData::~ParticleData() {} // Empty Dtor

Particle::Particle(ParticleData* data, AEVec2 pos) 
: pos{ pos }, vel({ 0.f,0.f }), 
  curr_col{ 0 }, curr_row{ 0 }, 
  elapsed_dur{ 0.f }, dur{ 0.f }, 
  scale{ 1.f }, min_scale{ 1.f }, 
  data{ data }, fade{ false } {} // Ctor

Particle::~Particle() {} // Empty Dtor

ParticleSystem::ParticleSystem()
: datatypes{ new ParticleData[ParticleType::LAST] {
	{ASSET_SPARKLE_SPRITE, 3, 3}, // SPARKLE
	{ASSET_STAR_SPRITE, 1, 1} // STAR
}} {} // Ctor

ParticleSystem::~ParticleSystem() { // Dtor
	Clear();
	delete[] datatypes;
}

/*!
* @brief Spawn a particle in the scene
* @param type - The particle enum type to spawn
* @param center - The center location in world coordinates
* @param amt - The amount of particles to spawn
* @param radius - The spawning radius around the center
* @param speed - The speed of the particles spawning
* @param duration - The particles lifetime
* @param minSize - The minimum size of the particles
* @param maxSize - The maximum size of the particles
* @param fade - Whether the particle will fade overtime before death
*/
void ParticleSystem::SpawnParticles(ParticleType::Type type, AEVec2 center, int amt, float radius, float speed, float duration, float minSize, float maxSize, bool fade) {
	if (amt <= 0) // amount boundary check
		return;

	// get an inactive particle
	int index = 0;
	for (auto& ref : particles) {
		if (ref->elapsed_dur < 0.f) {
			break;
		}
		index++;
	}

	AEVec2 vel = { Utils::RandRange(-0.5f, 0.5f) * radius, Utils::RandRange(-0.5f, 0.5f) * radius };
	if (index == particles.size()) { // if no active particles, spawn a new particle
		particles.push_back(std::make_unique<Particle>(&datatypes[type], center));
	}

	// set particle data
	particles[index]->elapsed_dur = 0.f;
	particles[index]->data = &datatypes[type];
	particles[index]->curr_col = 0;
	particles[index]->curr_row = 0;
	particles[index]->min_scale = minSize;
	particles[index]->scale = maxSize;
	particles[index]->dur = duration;
	particles[index]->pos = (amt == 1 ? center : (center + vel));
	particles[index]->vel = vel;
	particles[index]->fade = fade;

	// recursion for more particles if amount > 1
	SpawnParticles(type, center, amt - 1, radius, speed, duration, minSize, maxSize, fade);
}

/*!
* @brief Clears all active particles
*/
void ParticleSystem::Clear() {
	particles.clear();
}

/*!
* @brief Update all active particles
* @param dt - The delta time
*/
void ParticleSystem::Update(const f32& dt) {
	for (auto& ref : particles) {
		if (ref->elapsed_dur < 0.f) {
			continue;
		}

		int frameCount = ref->data->rows * ref->data->cols;
		f32 edt = ref->dur / (f32)frameCount;

		// move the particles and fade them away after time
		ref->pos += (ref->vel * dt);
		ref->scale -= (ref->scale - ref->min_scale / (f32)frameCount) * dt;

		// update the current frame
		if ((ref->elapsed_dur += dt) > edt) {
			ref->elapsed_dur = 0.f;
			if (++ref->curr_col >= ref->data->cols) {
				ref->curr_col = 0;

				if (++ref->curr_row >= ref->data->rows) {
					ref->elapsed_dur = -1.f;
				}
			}
		}

	}
}

/*!
* @brief Render all active particles
*/
void ParticleSystem::Render() {
	for (auto& ref : particles) {
		if (ref->elapsed_dur < 0.f) {
			continue;
		}
		// get transformation matrix for each individual particle
		AEMtx33 t = Utils::GetTransformMatrix(ref->pos, { ref->scale, ref->scale }, 0.f);

		// render their sprite
		ref->data->sprite->Render(t, { static_cast<int>(ref->fade ? (ref->scale - ref->min_scale) * 255 : 255), 255, 255, 255 }, ref->curr_row, ref->curr_col);
	}
}
