#pragma once
#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM
#include "AEEngine.h"
#include "../Managers/AssetManager.hpp"
#include <vector>
#include <array>
#include <memory>

class ParticleSystem;
class Particle;

enum ParticleType {
	PARTICLE_SPARKLE = 0,
	PARTICLE_STAR,
	PARTICLE_LAST
};

class ParticleData {
private:
	u8 rows, cols;
	SpriteSheet* sprite;
	ParticleData(std::string const& filepath, u8 rows, u8 cols);
public:
	~ParticleData();
	friend class ParticleSystem;
	friend class Particle;
};

class Particle {
private:
	AEVec2 pos, vel;
	f32 elapsed_dur, dur, scale, min_scale;
	u8 curr_row, curr_col;
	bool fade;
	ParticleData* data;
public:
	Particle(ParticleData* data, AEVec2 pos = { 0.f, 0.f });
	~Particle();
	friend class ParticleSystem;
};

class ParticleSystem {
private:
	ParticleData* datatypes;
	std::vector<std::unique_ptr<Particle>> particles;

public:
	ParticleSystem();
	~ParticleSystem();

	void SpawnParticles(ParticleType type, AEVec2 center, int amt, float radius = 1.f, float speed = 1.f, float duration = 1.f, float minSize = 1.f, float maxSize = 1.f, bool fade = false);
	void Clear();
	void Update(const f32& dt);
	void Render();
};

#endif