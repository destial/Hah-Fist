#include "CoinEntity.hpp"
#include "../../Managers/AssetManager.hpp"
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
	Player* p = static_cast<Player*>(pickupper);
	p->AddCoin();
	AEAudioPlay(AssetManager::GetAudio(ASSET_COIN_PICKUP_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
}