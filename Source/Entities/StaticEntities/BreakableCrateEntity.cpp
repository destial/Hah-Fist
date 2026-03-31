#include "BreakableCrateEntity.hpp"
#include "../DropEntities/CoinEntity.hpp"
#include "../../Managers/SceneManager.hpp"

BreakableCrateEntity::BreakableCrateEntity(AEVec2 pos = { 0.f,0.f }) 
	: BreakableStaticEntity{ pos, ASSET_BREAKABLE_CRATE_IMAGE, 1, 1 } {

}

BreakableCrateEntity::~BreakableCrateEntity() {} // Empty dtor

void BreakableCrateEntity::OnBroken() {
	CoinEntity* coin = new CoinEntity{ position };
	SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(coin);
}