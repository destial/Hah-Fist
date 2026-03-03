/*!
* @file ParticleSystem.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 8 February 2026
* @course CSD1451
* @brief Declaration file for a particle system within a scene
*/

#pragma once
#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM
#include "AEEngine.h"
#include "../Managers/AssetManager.hpp"
#include <vector>
#include <array>
#include <memory>

namespace ParticleType {
	/*!
	* @brief Declare all available particles
	*/
	enum Type {
		SPARKLE = 0,
		STAR,
		LAST
	};

}

// Forward declaration
class ParticleSystem;
class Particle;

/*!
* @brief Store render data about a particle type
*/
class ParticleData {
private:
	u8 rows, cols;
	SpriteSheet* sprite;
	ParticleData(std::string const& filepath, u8 rows, u8 cols);
public:
	~ParticleData();
	friend class ParticleSystem; // To use private member variables
	friend class Particle; // To use private member variables
};

/*!
* @brief Store data about individual particles
*/
class Particle {
private:
	AEVec2 pos, vel;
	f32 elapsed_dur, dur, scale, min_scale;
	u8 curr_row, curr_col;
	bool fade;
	ParticleData* data; // The owning particle data type
public:
	Particle(ParticleData* data, AEVec2 pos = { 0.f, 0.f }); // Ctor
	~Particle(); // Dtor
	friend class ParticleSystem; // For the particle system to use private member variables
};

/*!
* @brief Handles the spawning, updating, rendering and despawning of particles
*/
class ParticleSystem {
private:
	ParticleData* datatypes; // All available particle data type
	std::vector<std::unique_ptr<Particle>> particles; // Store individual spawned particles

public:
	ParticleSystem(); // Ctor
	~ParticleSystem(); // Dtor

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
	void SpawnParticles(ParticleType::Type type, AEVec2 center, int amt, float radius = 1.f, float speed = 1.f, float duration = 1.f,
						float minSize = 1.f, float maxSize = 1.f, bool fade = false);

	/*!
	* @brief Clears all active particles
	*/
	void Clear();

	/*!
	* @brief Update all active particles
	* @param dt - The delta time
	*/
	void Update(const f32& dt);

	/*!
	* @brief Render all active particles
	*/
	void Render();
};

#endif