#ifndef __BOSSSPAWNTRIGGERENTITY_HPP
#define __BOSSSPAWNTRIGGERENTITY_HPP
#include "../StaticEntity.hpp"

class BossSpawnTriggerEntity : public StaticEntity
{
protected:
	bool player_entered{ false };
	bool player_inside{false};
	bool player_left{ false };
public:
	BossSpawnTriggerEntity(AEVec2 pos);
	~BossSpawnTriggerEntity();


	void PreUpdate(const f32& dt) override;
	void PostUpdate(const f32& dt) override;

	void OnCollide(GameObjectEntity* go) override;
};

#endif