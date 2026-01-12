#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"

Player::Player(AEVec2 pos) : BaseEntity(pos) {
	mesh = MeshRenderer::CreateLeftBottomCornerRect(0xFFFFFFFF);
}

Player::~Player() {

}