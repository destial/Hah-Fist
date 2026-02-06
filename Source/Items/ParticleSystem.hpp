#pragma once
#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM
#include "AEEngine.h"
#include "../Managers/AssetManager.hpp"
#include <vector>
#include <array>
#include <memory>

class ParticleSystem;

enum ParticleType {
	PARTICLE_SPARKLE = 0,
	PARTICLE_STAR,
	PARTICLE_LAST;
};

class ParticleData {

};

class Particle {
	friend class ParticleSystem;
private:
	AEVec2 pos, vel;
	f32 elapsed_dur, dur, scale, scale_og;
	u8 curr_row, curr_col;
	ParticleData& data;

	Particle(AEVec2 pos = {0.f, 0.f});
};

class ParticleSystem {
private:
	ParticleData datatypes[PARTICLE_LAST];
	std::vector<std::unique_ptr<Particle>> particles;

public:
	ParticleSystem();
	~ParticleSystem();

	void SpawnParticles(ParticleType type, AEVec2 center, int amt);

	void Update(const f32& dt);
	void Render();
};

#endif