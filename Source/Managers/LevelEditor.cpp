#include "LevelEditor.hpp"
#include "../UI/Debug.hpp"
#include "../Managers/SerializationManager.hpp"
#include "../Managers/LevelManager.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Entities/StaticEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Entities/Enemies/SpiderEntity.hpp"
#include "../Entities/Enemies/ProjectileEntity.hpp"
#include "../Entities/Enemies/TitanEntity.hpp"
#include "../Entities/Enemies/PayloadEntity.hpp"
#include "../Entities/Enemies/IronsideEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Entities/DropEntities/CoinEntity.hpp"
#include "../Entities/StaticEntities/MovingPlatformEntity.hpp"
#include "../Entities/StaticEntities/BreakableCrateEntity.hpp"
#include "../Entities/StaticEntities/BossSpawnTriggerEntity.hpp"

LevelEditor::LevelEditor(BaseScene* b_scene)
: scene{ b_scene }, toggled{ false }, currentSelection{ nullptr } {
}

LevelEditor::~LevelEditor() {}

void LevelEditor::Toggle() {
	toggled = !toggled;
	DebugUtils::ToggleRender(toggled);
}

bool LevelEditor::IsToggled() const {
	return toggled;
}

void LevelEditor::SetScene(BaseScene* b_scene) {
	scene = b_scene;
	currentSelection = nullptr;
	toggled = false;
}

void LevelEditor::SelectEntity(BaseEntity* entity) {
	if (Weapon* w = dynamic_cast<Weapon*>(entity)) {
		return;
	}
	currentSelection = entity;
	currentOffset = Utils::GetMouseWorld(true) - entity->position;
}

void LevelEditor::RemoveSelectedEntity() {
	if (currentSelection == nullptr) {
		return;
	}

	if (Weapon* w = dynamic_cast<Weapon*>(currentSelection)) {
		return;
	}

	scene->RemoveEntityFromScene(currentSelection);
	currentSelection = nullptr;
}

BaseEntity* LevelEditor::AddEntity(Editor::GameObjectType type) {
	switch (type) {
	case Editor::GameObjectType::STATIC_PLATFORM: {
		StaticEntity* go = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_PLATFORM,Utils::GetMouseWorld(true));
		go->mesh = MeshRenderer::GetCenterRectMesh();
		//go->go_type = GameObjectEntity::PhysicsType::STATIC;
		go->layer = BaseEntity::RenderLayer::WORLD;
		scene->AddEntityToScene(go);
		return go;
	}
	case Editor::GameObjectType::STATIC_WALL: {
		StaticEntity* go = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_WALL, Utils::GetMouseWorld(true));
		go->mesh = MeshRenderer::GetCenterRectMesh();
		go->layer = BaseEntity::RenderLayer::WORLD;
		scene->AddEntityToScene(go);
		return go;
	}
	case Editor::GameObjectType::ENEMY_1: {
		EnemyEntity* enemy = new EnemyEntity(Utils::GetMouseWorld(true));
		enemy->layer = BaseEntity::RenderLayer::ENTITY;
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	case Editor::GameObjectType::ENEMY_2: {
		EnemyEntity* enemy = new TrooperEntity(Utils::GetMouseWorld(true));
		enemy->layer = BaseEntity::RenderLayer::ENTITY;
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	case Editor::GameObjectType::PROJECTILE_ENEMY: {
		EnemyEntity* enemy = new ProjectileEntity(Utils::GetMouseWorld(true));
		enemy->layer = BaseEntity::RenderLayer::ENTITY;
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	case Editor::SPIDER: {
		EnemyEntity* enemy = new SpiderEntity(Utils::GetMouseWorld(true));
		enemy->layer = BaseEntity::RenderLayer::ENTITY;
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	case Editor::TITAN: {
		EnemyEntity* enemy = new TitanEntity(Utils::GetMouseWorld(true));
		enemy->layer = BaseEntity::RenderLayer::ENTITY;
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	case Editor::PAYLOAD: {
		EnemyEntity* enemy = new PayloadEntity(Utils::GetMouseWorld(true));
		enemy->layer = BaseEntity::RenderLayer::ENTITY;
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	case Editor::IRONSIDE: {
		EnemyEntity* enemy = new IronsideEntity(Utils::GetMouseWorld(true));
		enemy->layer = BaseEntity::RenderLayer::ENTITY;
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	case Editor::COIN: {
		CoinEntity* coin = new CoinEntity(Utils::GetMouseWorld(true));
		scene->AddEntityToScene(coin);
		return coin;
	}
	case Editor::CRATE: {
		BreakableCrateEntity* b = new BreakableCrateEntity(Utils::GetMouseWorld(true));
		scene->AddEntityToScene(b);
		return b;
	}
	case Editor::MOVING_PLATFORM: {
		MovingPlatformEntity* b = new MovingPlatformEntity(Utils::GetMouseWorld(true), AEVec2{1.f, 0.f});
		scene->AddEntityToScene(b);
		return b;
	}
	case Editor::BOSS_SPAWN_WALL: {
		BossSpawnTriggerEntity* b = new BossSpawnTriggerEntity(Utils::GetMouseWorld(true));
		scene->AddEntityToScene(b);
		return b;
	}
	default: break;
	}

	return nullptr;
}

void LevelEditor::Update(const f32& dt) {
	static bool scaleX{ true };
	f32 cam_x, cam_y;
	AEGfxGetCamPosition(&cam_x, &cam_y);
	
	AEVec2 mwp = Utils::GetMouseWorld(true);
	s32 scroll;
	AEInputMouseWheelDelta(&scroll);

	for (BaseEntity* go : scene->Entities()) {
		if (AEInputCheckTriggered(AEVK_LBUTTON) && Utils::OBBPoint(go, mwp)) {
			SelectEntity(go);
		}
	}

	if (AEInputCheckTriggered(AEVK_1)) {
		SelectEntity(AddEntity(Editor::GameObjectType::STATIC_PLATFORM));
	}

	if (AEInputCheckTriggered(AEVK_2)) {
		SelectEntity(AddEntity(Editor::GameObjectType::STATIC_WALL));
	}

	if (AEInputCheckTriggered(AEVK_3)) {
		SelectEntity(AddEntity(Editor::GameObjectType::ENEMY_1));
	}

	if (AEInputCheckTriggered(AEVK_4)) {
		SelectEntity(AddEntity(Editor::GameObjectType::ENEMY_2));
	}

	if (AEInputCheckTriggered(AEVK_5)) {
		SelectEntity(AddEntity(Editor::GameObjectType::SPIDER));
	}

	if (AEInputCheckTriggered(AEVK_6)) {
		SelectEntity(AddEntity(Editor::GameObjectType::PROJECTILE_ENEMY));
	}

	if (AEInputCheckTriggered(AEVK_7)) {
		SelectEntity(AddEntity(Editor::GameObjectType::COIN));
	}
	if (AEInputCheckTriggered(AEVK_8)) {
		SelectEntity(AddEntity(Editor::GameObjectType::MOVING_PLATFORM));
	}
	
	if (AEInputCheckTriggered(AEVK_9)) {
		SelectEntity(AddEntity(Editor::GameObjectType::CRATE));
	}

	if (AEInputCheckTriggered(AEVK_Q)) {
		SelectEntity(AddEntity(Editor::GameObjectType::BOSS_SPAWN_WALL));
	}

	//BOSSES
	if (AEInputCheckTriggered(AEVK_B)) {
		SelectEntity(AddEntity(Editor::GameObjectType::TITAN));
	}
	if (AEInputCheckTriggered(AEVK_N)) {
		SelectEntity(AddEntity(Editor::GameObjectType::PAYLOAD));
	}
	if (AEInputCheckTriggered(AEVK_M)) {
		SelectEntity(AddEntity(Editor::GameObjectType::IRONSIDE));
	}
	// Toggling to switch from scaling the X axis to Y axis
	if (AEInputCheckTriggered(AEVK_X)) {
		scaleX = !scaleX;
	}
	if (currentSelection) {
		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			if (Utils::OBBPoint(currentSelection, mwp)) {
				currentSelection->position = mwp - currentOffset;
				if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(currentSelection)) {
					go->prev_position = mwp;
					if (MovingPlatformEntity* mpe = dynamic_cast<MovingPlatformEntity*>(currentSelection))
					{
						mpe->SetStartPoint(go->position);
					}
				}
			}
			if (scroll != 0) {
				(scaleX ? currentSelection->scale.x : currentSelection->scale.y) += scroll;
				(scaleX ? currentSelection->scale.x : currentSelection->scale.y) = max(scaleX ? currentSelection->scale.x : currentSelection->scale.y,1);
			}
		}

		if (AEInputCheckCurr(AEVK_RBUTTON)) {
			if (scroll != 0) {
				currentSelection->scale.y += scroll;
				currentSelection->scale.y = max(currentSelection->scale.y, 1);
			}
		}

		if (AEInputCheckTriggered(AEVK_DELETE)) {
			RemoveSelectedEntity();
		}
	}

	if (AEInputCheckCurr(AEVK_RBUTTON)) {
		s32 del_x, del_y;
		AEInputGetCursorPositionDelta(&del_x, &del_y);

		cam_x -= del_x;
		cam_y += del_y;

		AEGfxSetCamPosition(cam_x, cam_y);
	}

	static float saved = 0.f;
	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_S)) {
		char filename[50];
		sprintf_s(filename, 50, "Assets/level_%d.dat", LevelManager::GetLevel());
		Serialization::WriteToFile(filename, Serialization::SerializeAll(scene->Entities()));
		saved = 1.f;
	}

	if (saved > 0.f) {
		saved -= dt;
		DebugUtils::RenderText({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }, "Saved!");
	}
}

void LevelEditor::Render() {
	if (currentSelection) {
		AEVec2 normal = { currentSelection->position.x, currentSelection->position.y + currentSelection->scale.y * 0.5f };
		AEVec2 right = { currentSelection->position.x + currentSelection->scale.x * 0.5f , currentSelection->position.y};
		DebugUtils::RenderLine(currentSelection->position, normal, { 255, 255, 0, 0 });
		DebugUtils::RenderLine(currentSelection->position, right, { 255, 0, 255, 0 });
	}
}

