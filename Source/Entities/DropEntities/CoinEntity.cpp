#include "CoinEntity.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../PlayerEntity.hpp"

CoinEntity::CoinEntity(AEVec2 pos) : DropEntity(pos)
{
	InitializeAnimatedSpriteData(ASSET_COIN_SPRITE, ASSET_COIN_SPRITE_ROWS, ASSET_COIN_SPRITE_COLUMNS, ASSET_COIN_SPRITE_SCALE);
}

CoinEntity::~CoinEntity()
{ 

}

void CoinEntity::OnPickup(GameObjectEntity* pickupper)
{
	PlayerEntity* p = static_cast<PlayerEntity*>(pickupper);
	p->AddCoin();
	AEAudioPlay(AssetManager::GetAudio(ASSET_COIN_PICKUP_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
	SceneManager::GetInstance()->GetCurrentScene()->GetParticleSystem()->SpawnParticles(ParticleType::STAR, this->position, 1, 0.5f, 0.4f, .75f, 0.1f, 2.f, false);
}