#include "BreakableCrateEntity.hpp"
#include "../DropEntities/CoinEntity.hpp"
#include "../../Managers/SceneManager.hpp"

BreakableCrateEntity::BreakableCrateEntity(AEVec2 pos = { 0.f,0.f }) : BreakableStaticEntity(pos, "Assets/crate.png", 1, 1)
{

}

BreakableCrateEntity::~BreakableCrateEntity()
{

}

void BreakableCrateEntity::OnBroken()
{
	CoinEntity* coin = new CoinEntity(position);
	SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(coin);
}