/*!
* @file LevelEditor.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 23 February 2026
* @course CSD1451
* @brief Definition file for a level editor that the user
* @brief can actively modify and save the entities inside a level
*/

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
: scene{ b_scene }, toggled{ false }, currentSelections(0) {} // Ctor

LevelEditor::~LevelEditor() {} // Empty dtor

/*!
* @brief Toggle the level editor state
*/
void LevelEditor::Toggle() {
	toggled = !toggled;
	DebugUtils::ToggleRender(toggled);
}

/*!
* @brief Get the current state of the level editor
*/
bool LevelEditor::IsToggled() const {
	return toggled;
}

/*!
* @brief Set the current scene to edit
* @param scene - The scene to edit, called when transitioning
*/
void LevelEditor::SetScene(BaseScene* b_scene) {
	scene = b_scene;
	currentSelections.clear();
	toggled = false;
}

/*!
* @brief Set the current entity selection
* @param entity - The entity to select
*/
void LevelEditor::SelectEntity(BaseEntity* entity) {
	if (entity == nullptr) // Non-null
		return;
	if (Weapon* w = dynamic_cast<Weapon*>(entity)) // Skip weapon entity
		return;
	std::pair<BaseEntity*, AEVec2> pair = { entity, Utils::GetMouseWorld(true) - entity->position };
	currentSelections.push_back(pair);
}

/*!
* @brief Remove the current entity selection from the scene
* @brief This is safe from concurrent modification
*/
void LevelEditor::RemoveSelectedEntity() {
	if (currentSelections.empty())
		return; // Removing nothing

	for (std::pair<BaseEntity*, AEVec2> const& pair : currentSelections) {
		if (Weapon* w = dynamic_cast<Weapon*>(pair.first)) 
			continue; // Skip weapon entity

		scene->RemoveEntityFromScene(pair.first);
	}
	
	// Clear selections
	currentSelections.clear();
}

/*!
* @brief Add an entity to the scene based on the GameObject type
*/
BaseEntity* LevelEditor::AddEntity(Editor::GameObjectType type) {
	switch (type) {
	case Editor::GameObjectType::STATIC_PLATFORM: {
		StaticEntity* go = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_PLATFORM,Utils::GetMouseWorld(true));
		go->mesh = MeshRenderer::GetCenterRectMesh();
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

/*!
* @brief Update the level editor per frame, to manipulate
* @brief the currently selected entity
*/
void LevelEditor::Update(const f32& dt) {
	static bool scaleX{ true };
	f32 cam_x, cam_y;
	AEGfxGetCamPosition(&cam_x, &cam_y);
	
	// Get mouse position based on screen & camera position
	AEVec2 mwp = Utils::GetMouseWorld(true);
	s32 scroll;
	AEInputMouseWheelDelta(&scroll);

	// Select entities by clicking on them
	bool selected = false;
	for (BaseEntity* go : scene->Entities()) {
		if (AEInputCheckTriggered(AEVK_LBUTTON) && Utils::OBBPoint(go, mwp)) {
			bool s = true;
			for (std::pair<BaseEntity*, AEVec2> pair : currentSelections) {
				if (go == pair.first) {
					s = false;
					pair.second = Utils::GetMouseWorld(true) - go->position;
				}
			}
			if (s) {
				currentSelections.clear();
				SelectEntity(go);
				selected = true;
			}
		}
	}

	// Select entities using selection box
	static AEVec2 selectionBoxStart{};
	static AEVec2 selectionBoxEnd{};

	if (!selected && AEInputCheckTriggered(AEVK_MBUTTON)) {
		selectionBoxStart = mwp;
		selectionBoxEnd = mwp;
		currentSelections.clear();
	}

	if (!selected && AEInputCheckPrev(AEVK_MBUTTON) && !AEInputCheckReleased(AEVK_MBUTTON)) {
		selectionBoxEnd = mwp;
	}

	if (!selected && AEInputCheckPrev(AEVK_MBUTTON) && AEInputCheckReleased(AEVK_MBUTTON)) {
		BaseEntity* temp = new GameObjectEntity{ AEVec2{ selectionBoxStart.x + selectionBoxEnd.x, selectionBoxStart.y + selectionBoxEnd.y } * 0.5f };
		temp->scale.x = selectionBoxEnd.x - selectionBoxStart.x;
		temp->scale.y = selectionBoxEnd.y - selectionBoxStart.y;
		for (BaseEntity* en : scene->Entities()) {
			if (Utils::OBB(en, temp)) {
				SelectEntity(en);
			}
		}
		delete temp;

		selectionBoxStart = {};
		selectionBoxEnd = {};
	}

	// Render selection box
	DebugUtils::RenderLine(selectionBoxStart, { selectionBoxStart.x, selectionBoxEnd.y }, {255, 255, 255, 0});
	DebugUtils::RenderLine(selectionBoxEnd, { selectionBoxStart.x, selectionBoxEnd.y }, { 255, 255, 255, 0 });
	DebugUtils::RenderLine(selectionBoxStart, { selectionBoxEnd.x, selectionBoxStart.y }, { 255, 255, 255, 0 });
	DebugUtils::RenderLine(selectionBoxEnd, { selectionBoxEnd.x, selectionBoxStart.y }, { 255, 255, 255, 0 });

	// Hotkey to add entities
	if (AEInputCheckTriggered(AEVK_1)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::STATIC_PLATFORM));
	}
	if (AEInputCheckTriggered(AEVK_2)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::STATIC_WALL));
	}
	if (AEInputCheckTriggered(AEVK_3)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::ENEMY_1));
	}
	if (AEInputCheckTriggered(AEVK_4)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::ENEMY_2));
	}
	if (AEInputCheckTriggered(AEVK_5)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::SPIDER));
	}
	if (AEInputCheckTriggered(AEVK_6)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::PROJECTILE_ENEMY));
	}
	if (AEInputCheckTriggered(AEVK_7)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::COIN));
	}
	if (AEInputCheckTriggered(AEVK_8)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::MOVING_PLATFORM));
	}
	if (AEInputCheckTriggered(AEVK_9)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::CRATE));
	}
	if (AEInputCheckTriggered(AEVK_Q)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::BOSS_SPAWN_WALL));
	}

	// Hotkey to add bossess
	if (AEInputCheckTriggered(AEVK_B)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::TITAN));
	}
	if (AEInputCheckTriggered(AEVK_N)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::PAYLOAD));
	}
	if (AEInputCheckTriggered(AEVK_M)) {
		currentSelections.clear();
		SelectEntity(AddEntity(Editor::GameObjectType::IRONSIDE));
	}

	// Toggling to switch from scaling the X axis to Y axis
	if (AEInputCheckTriggered(AEVK_X)) {
		scaleX = !scaleX;
	}

	// Translate & scale selections based on mouse input
	if (!currentSelections.empty()) {
		bool drag = false;
		if (AEInputCheckCurr(AEVK_LBUTTON) && !AEInputCheckTriggered(AEVK_LBUTTON)) {
			
			// Translate & drag selections
			for (std::pair<BaseEntity*, AEVec2> const& pair : currentSelections) {
				if (Utils::OBBPoint(pair.first, mwp)) {
					drag = true;
				}
			}

			if (drag) {
				for (std::pair<BaseEntity*, AEVec2> const& pair : currentSelections) {
					pair.first->position = mwp - pair.second;
					if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(pair.first)) {
						go->prev_position = mwp;
						if (MovingPlatformEntity* mpe = dynamic_cast<MovingPlatformEntity*>(pair.first)) {
							mpe->SetStartPoint(go->position);
						}
					}
					
					// Scale selections
					if (scroll != 0) {
						(scaleX ? pair.first->scale.x : pair.first->scale.y) += scroll;
						(scaleX ? pair.first->scale.x : pair.first->scale.y) = max(scaleX ? pair.first->scale.x : pair.first->scale.y, 1);
					}

					if (AEInputCheckCurr(AEVK_RBUTTON)) {
						if (scroll != 0) {
							pair.first->scale.y += scroll;
							pair.first->scale.y = max(pair.first->scale.y, 1);
						}
					}
				}
			}
		}

		// Remove selection
		if (AEInputCheckTriggered(AEVK_DELETE)) {
			RemoveSelectedEntity();
		}
	}

	// Move camera position
	if (AEInputCheckCurr(AEVK_RBUTTON)) {
		s32 del_x, del_y;
		AEInputGetCursorPositionDelta(&del_x, &del_y);

		cam_x -= del_x;
		cam_y += del_y;

		AEGfxSetCamPosition(cam_x, cam_y);
	}

	// Save level
	static float saved = 0.f;
	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_S)) {
		char filename[50];
		sprintf_s(filename, 50, "Assets/level_%d.dat", LevelManager::GetLevel());
		Serialization::WriteToFile(filename, Serialization::SerializeAll(scene->Entities()));
		saved = 1.f;
	}

	// Render save text
	if (saved > 0.f) {
		saved -= dt;
		DebugUtils::RenderText({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }, "Saved!");
	}
}

/*!
* @brief Render data about the level, as well as if the level
* @brief has been saved or not
*/
void LevelEditor::Render() {
	if (currentSelections.empty()) {
		return;
	}

	// Render current selection highlight
	for (std::pair<BaseEntity*, AEVec2> const& pair : currentSelections) {
		AEVec2 normal = { pair.first->position.x, pair.first->position.y + pair.first->scale.y * 0.5f };
		AEVec2 right = { pair.first->position.x + pair.first->scale.x * 0.5f , pair.first->position.y};
		DebugUtils::RenderLine(pair.first->position, normal, { 255, 255, 0, 0 });
		DebugUtils::RenderLine(pair.first->position, right, { 255, 0, 0, 255 });
	}
}

