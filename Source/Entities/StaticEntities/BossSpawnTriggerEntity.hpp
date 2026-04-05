/*!
* @file BossSpawnTriggerEntity.hpp
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @date 12th March 2026
* @course CSD1451
* @brief This source file declares the BossSpawnTriggerEntity class and its member functions, which are
* Destructable objects in the game that drop coins.
*/
#ifndef __BOSSSPAWNTRIGGERENTITY_HPP
#define __BOSSSPAWNTRIGGERENTITY_HPP
#include "../StaticEntity.hpp"

class BossSpawnTriggerEntity : public StaticEntity {
protected:
	bool player_entered{ false };
	bool player_inside{false};
	bool player_left{ false };
public:
	BossSpawnTriggerEntity(AEVec2 pos);
	virtual ~BossSpawnTriggerEntity();

	virtual void PreUpdate(const f32& dt) override;
	virtual void PostUpdate(const f32& dt) override;

	virtual void OnCollide(GameObjectEntity* go) override;
};

#endif