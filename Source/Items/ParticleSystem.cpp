#include "ParticleSystem.hpp"
#include "../Utils/AEOverload.hpp"
#include <iostream>

ParticleData::ParticleData(std::string const& filepath, u8 rows, u8 cols)
	: rows{ rows }, cols{ cols }, 
	sprite{ AssetManager::GetSpriteSheet(filepath, rows, cols) } {

}

ParticleData::~ParticleData() {

}

Particle::Particle(ParticleData* data, AEVec2 pos) 
	: pos{ pos }, vel({ 0.f,0.f }), curr_col{ 0 }, curr_row{ 0 }, elapsed_dur{ 0.f }, dur{ 0.f }, scale{ 1.f }, min_scale{ 1.f }, data{ data }, fade{ false } {
}

Particle::~Particle() {

}

ParticleSystem::ParticleSystem()
	: datatypes{ new ParticleData[PARTICLE_LAST]{
		{"Assets/sparkle_sheet.png", 3, 3}, // PARTICLE_SPARKLE
		{"Assets/star.png", 1, 1} // PARTICLE_STAR
	}} {
}

ParticleSystem::~ParticleSystem() {
	Clear();
	delete[] datatypes;
}

void ParticleSystem::SpawnParticles(ParticleType type, AEVec2 center, int amt, float radius, float speed, float duration, float minSize, float maxSize, bool fade) {
	if (amt <= 0)
		return;
	int index = 0;
	for (auto& ref : particles) {
		if (ref->elapsed_dur < 0.f) {
			break;
		}
		index++;
	}

	AEVec2 vel = { Utils::RandRange(-0.5f, 0.5f) * radius, Utils::RandRange(-0.5f, 0.5f) * radius };
	if (index == particles.size()) {
		particles.push_back(std::make_unique<Particle>(&datatypes[type], center));
	}
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

	SpawnParticles(type, center, amt - 1, radius, speed, duration, minSize, maxSize, fade);
}

void ParticleSystem::Clear() {
	particles.clear();
}

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

void ParticleSystem::Render() {
	for (auto& ref : particles) {
		if (ref->elapsed_dur < 0.f) {
			continue;
		}
		AEMtx33 t = Utils::GetTransformMatrix(ref->pos, { ref->scale, ref->scale }, 0.f);
		ref->data->sprite->Render(t, { static_cast<unsigned char>(ref->fade ? (ref->scale - ref->min_scale) * 255 : 255), 255, 255, 255 }, ref->curr_row, ref->curr_col);
	}
}
