/*!
* @file Utils.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Declaration file for utility functions
*/

#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"
#include <iostream>
#include <vector>
#include <utility>

// Forward declaration without having to include the actual header files
class BaseEntity;
class GameObjectEntity;

/*!
* @brief Structure to represent a color
* @brief Alpha, Red, Green, Blue (in this order)
* @brief Each value is 0-255 (unsigned char)
*/
struct Color {
	Color(u32 packed = 0); // Ctor
	Color(int a, int r, int g, int b); // Ctor
	Color(f32 a, f32 r, f32 g, f32 b); // Ctor
	Color(Color const& copy); // Ctor

	/*!
	* @brief Pack the colors into a 32-bit integer
	* @brief 8-bits per channel stored as [ARGB]
	* @return The packed integer
	*/
	u32 Pack() const;

	union {
		unsigned char a;
		unsigned char alpha;
	};
	union {
		unsigned char r;
		unsigned char red;
	};
	union {
		unsigned char g;
		unsigned char green;
	};
	union {
		unsigned char b;
		unsigned char blue;
	};
};

std::istream& operator>> (std::istream& is, Color& color);

std::ostream& operator<< (std::ostream& os, Color const& color);

Color operator- (Color const& lhs, Color const& rhs);

Color operator+ (Color const& lhs, Color const& rhs);

Color operator* (Color const& lhs, f32 scalar);

Color operator* (f32 scalar, Color const& lhs);

namespace Game {
	/*!
	* @brief Set the current application running state
	* @param b - The state
	*/
	void SetGameRunning(bool b);

	/*!
	* @brief Set the background color
	* @param c - The color
	*/
	void SetBackgroundColor(Color c);

	/*!
	* @brief Get the audio group for the music
	* @return The music audio group
	*/
	AEAudioGroup const& GetMusicGroup();

	/*!
	* @brief Get the volume for the music audio group
	* @return The music volume between 0 - 1.f
	*/
	float& MusicVol();

	/*!
	* @brief Get the audio group for the sound effects
	* @return The effects audio group
	*/
	AEAudioGroup const& GetSfxGroup();

	/*!
	* @brief Get the volume for the sfx audio group
	* @return The sfx volume between 0 - 1.f
	*/
	float& SfxVol();
}

namespace Utils {
	/*!
	* @brief Convert world coordinates to screen coordinates
	* @param x - World x
	* @param y - World y
	* @return Vec2 of screen coordinates
	*/
	AEVec2 WorldToScreen(f32 x, f32 y);

	/*!
	* @brief Convert screen coordinates to world coordinates
	* @param x - Screen x
	* @param y - Screen y
	* @return Vec2 of world coordinates
	*/
	AEVec2 ScreenToWorld(f32 x, f32 y);

	/*!
	* @brief Convert game coordinates to screen coordinates
	* @param x - Game x
	* @param y - Game y
	* @return Vec2 of screen coordinates
	*/
	AEVec2 GameToScreen(f32 x, f32 y);

	/*!
	* @brief Convert scales to screen size
	* @param x - Scale x
	* @param y - Scale y
	* @return Vec2 of screen size
	*/
	AEVec2 ScaleToScreen(f32 x, f32 y);

	/*!
	* @brief Convert screen size to scale
	* @param x - Screen x
	* @param y - Screen y
	* @return Vec2 of scale size
	*/
	AEVec2 ScreenToScale(f32 x, f32 y);

	/*!
	* @brief Convert game coordinates to text coordinates
	* @param x - Game x
	* @param y - Game y
	* @return Vec2 of text coordinates
	*/
	AEVec2 GameToTextScreen(f32 x, f32 y, bool cam = false);

	/*!
	* @brief Get the screen size multiplier (e.g. fullscreen or different from 1600x900)
	* @return Vec2 of size multiplier (x,y)
	*/
	AEVec2 ScreenSizeMultiplier();

	/*!
	* @brief Get a random number between two ranges
	* @param mn - Minimum range
	* @param mx - Maximum range
	* @return A float between a range
	*/
	f32 RandRange(f32 mn, f32 mx);

	/*!
	* @brief Get the world width
	* @return The world width
	*/
	const f32 GetWorldWidth(void);

	/*!
	* @brief Get the world height
	* @return The world height
	*/
	const f32 GetWorldHeight(void);

	/*!
	* @brief Get the current game delta time
	*/
	const f32 GetDeltaTime(void);

	/*!
	* @brief Set the current game delta time
	* @param dt - The delta time
	*/
	void SetDeltaTime(f32 dt);

	/*!
	* @brief Get the screen resolution
	* @return The screen resolution (first = x, second = y)
	*/
	std::pair<int, int> GetScreenResolution(void);

	/*!
	* @brief Set the current screen resolution
	* @param width - Screen width
	* @param height - Screen height
	*/
	void SetScreenResolution(unsigned int width, unsigned int height);

	/*!
	* @brief Get the mouse coordinates in world space
	* @param camera - Take into account camera position or not
	* @return Vec2 of mouse coordinates
	*/
	const AEVec2 GetMouseWorld(bool camera = false);

	/*!
	* @brief Get the min max AABB of a game object
	* @param go - The game object
	* @param min - Output the min
	* @param max - Output the max
	*/
	void GetMinMaxAABB(GameObjectEntity* const& go, AEVec2& min, AEVec2& max);

	/*!
	* @brief Get the coordinates of each corner of a game object
	* @param go - The game object
	* @param A vector of the corners of the game object (currently 4 corners)
	*/
	std::vector<AEVec2> GetCorners(const BaseEntity* go);

	/*!
	* @brief Test collision between two circle entities
	* @param en - The first entity
	* @param en2 - The other entity
	* @return true if overlap
	*/
	bool CircleCircleCollision(BaseEntity* const& en, BaseEntity* const& en2);

	/*!
	* @brief Test collision between circle and OBB entity
	* @param circle - The circle entity
	* @param obb - The obb entity
	* @return true if overlap
	*/
	bool CircleOBBCollision(BaseEntity* const& circle, BaseEntity* const& obb);

	/*!
	* @brief Test collision between two AABB entities
	* @param en - The first entity
	* @param en2 - The other entity
	* @return true if overlap
	*/
	bool AABB(BaseEntity* const& en, BaseEntity* const& en2);

	/*!
	* @brief Test collision between two dynamic AABB game entities
	* @param go - The first entity
	* @param go2 - The other entity
	* @param tCollision - Output the time of collision
	* @param dt - The delta time
	* @return true if collision
	*/
	bool DynamicAABB(GameObjectEntity* const& go, GameObjectEntity* const& go2, float& tCollision, const float& dt);

	/*!
	* @brief Test collision between a ray and a AABB
	* @param ray_origin - The origin of the ray
	* @param ray_dir - The direction of the ray
	* @param target - The AABB entity to check against
	* @param contact - Output the contact point
	* @param normal - Output the normal line
	* @param tCollision - Output the time of collision
	* @param true if hit
	*/
	bool RayAABB(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target, AEVec2& contact, AEVec2& normal, float& tCollision);
	
	/*!
	* @brief Test collision between ray and every entities
	* @brief This method relies on RayAABB to check if It collides with any of the provided std::vector of entities.
	* @brief If it collides with just one entity in a list of entities, it will return true
	* @param ray_origin - The origin of the ray
	* @param ray_dir - The direction of the ray
	* @param entities - The vector of entities provided
	* @param true if any of them were hit
	*/
	bool RayHitAny(const AEVec2& ray_origin, const AEVec2& ray_dir, std::vector<BaseEntity*> const& entities); 

	/*!
	* @brief Test collision between a ray and an entity
	* @param ray_origin - The origin of the ray
	* @param ray_dir - The direction of the ray
	* @param target - The AABB entity to check against
	* @param true if hit
	*/
	bool RayHit(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target);

	/*!
	* @brief Test collision between two OBB entities
	* @param en - The first entity
	* @param en2 - The other entity
	* @return true if overlap
	*/
	bool OBB(BaseEntity* const& en, BaseEntity* const& en2);

	/*!
	* @brief Test collision between a point and OBB entity
	* @param en - The first entity
	* @param pos - The point to check
	* @return true if point inside
	*/
	bool OBBPoint(BaseEntity* const& en, AEVec2 const& pos);
	
	/*!
	* @brief Snap a vector to a smooth axis
	* @param result - Output the resulting snapped vector
	* @param vec2 - The vector to snap
	*/
	void SnapVectorToAxis(AEVec2* result, AEVec2* vec2);

	/*!
	* @brief Get a transformation matrix based on vector positions, scale and radian rotation
	* @param pos - Position
	* @param sca - Scale
	* @param rot - Rotation (in radians)
	* @return The transformation matrix (T * R * S)
	*/
	AEMtx33 GetTransformMatrix(AEVec2 const& pos, AEVec2 const& sca, f32 rot);

	/*!
	* @brief Template linear interpolate between two values at a delta
	* @param start - The starting point
	* @param end - The ending point
	* @param delta - The delta to interpolate
	* @return The interpolated value
	*/
	template<typename N>
	N Lerp(N start, N end, f32 delta) {
		return start + (AEClamp(delta, 0.f, 1.f) * (end - start));
	}

	/*!
	* @brief Template circular interpolate between two values at a delta
	* @param start - The starting point
	* @param end - The ending point
	* @param delta - The delta to interpolate
	* @return The circular interpolated value
	*/
	template<typename N>
	N LerpCircle(N start, N end, f32 delta) {
		delta = AEClamp(delta, 0.f, 1.f);
		f32 p = 1.f - ((AECos(PI * delta) + 1.f) * 0.5f);
		return Lerp(start, end, p);
	}
}
#endif