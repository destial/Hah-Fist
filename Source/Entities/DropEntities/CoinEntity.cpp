#include "CoinEntity.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../PlayerEntity.hpp"

CoinEntity::CoinEntity(AEVec2 pos) : DropEntity(pos)
{
	columns = 20;
	rows = 1;

	sprite = AssetManager::GetSpriteSheet("Assets/coin.png", rows, columns);
	mesh = nullptr;
	animationTimer = 0.f;
	animationFrame = 1.f / static_cast<f32>(columns * rows);
	currentRow = currentCol = 0;
	scale = { 2.f * ((static_cast<f32>(sprite->image->width) / static_cast<f32>(columns)) / (sprite->image->height / static_cast<f32>(rows))) , 2.f };
}

CoinEntity::~CoinEntity()
{ 

}

void CoinEntity::OnPickup(GameObjectEntity* pickupper)
{
	Player* p = static_cast<Player*>(pickupper);
	p->AddCoin();
	AEAudioPlay(AssetManager::GetAudio(ASSET_TURBOFIST_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
}