#include "LevelEditor.hpp"
#include "../UI/Debug.hpp"
#include "../Managers/SerializationManager.hpp"
#include "../Utils/Utils.hpp"

#include "../Utils/MeshRenderer.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Entities/WeaponEntity.hpp"

LevelEditor::LevelEditor(BaseScene* b_scene)
	: scene{ b_scene }, toggled{ false }, currentSelection{ nullptr }, level{ 0 } {
}

LevelEditor::~LevelEditor() {}

void LevelEditor::Toggle() {
	toggled = !toggled;
	DebugUtils::ToggleRender(toggled);
}

bool LevelEditor::IsToggled() const {
	return toggled;
}

void LevelEditor::SetScene(BaseScene* b_scene)
{
	scene = b_scene;
	currentSelection = nullptr;
	toggled = false;
}

void LevelEditor::SelectEntity(BaseEntity* entity) {
	currentSelection = entity;
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
		GameObjectEntity* go = new GameObjectEntity(Utils::GetMouseWorld(true));
		go->mesh = MeshRenderer::GetCenterRectMesh();
		go->go_type = GameObjectEntity::PhysicsType::STATIC;
		scene->AddEntityToScene(go);
		return go;
	}
	case Editor::GameObjectType::ENEMY_1: {
		EnemyEntity* enemy = new EnemyEntity(Utils::GetMouseWorld(true));
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	case Editor::GameObjectType::ENEMY_2: {
		EnemyEntity* enemy = new TrooperEntity(Utils::GetMouseWorld(true));
		scene->AddEntityToScene(enemy);
		return enemy;
	}
	default: break;
	}

	return nullptr;
}

void LevelEditor::Update(const f32& dt) {
	static float fps_counter = 0.f;
	static float cfps = 1.f / Utils::GetDeltaTime();
	static float saved = 0.f;
	if ((fps_counter += Utils::GetDeltaTime()) > 0.1f) {
		cfps = 1.f / Utils::GetDeltaTime();
		fps_counter = 0.f;
	}
	char fps[50];
	sprintf_s(fps, "FPS:%.0f", cfps);
	DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 0.25f }, std::string{fps});

	f32 cam_x, cam_y;
	AEGfxGetCamPosition(&cam_x, &cam_y);
	char cam[128];
	sprintf_s(cam, "Cam:%.2f,%.2f", cam_x, cam_y);
	DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 0.75f }, std::string{ cam });
	
	AEVec2 mwp = Utils::GetMouseWorld(true);
	char mos[128];
	sprintf_s(mos, "Mouse:%.2f,%.2f", mwp.x, mwp.y);
	DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 1.25f }, std::string{ mos });

	s32 scroll;
	AEInputMouseWheelDelta(&scroll);
	char scr[32];
	sprintf_s(scr, "Scroll:%d", scroll);
	DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 1.75f }, std::string{ scr });

	for (BaseEntity* go : scene->Entities()) {
		if (AEInputCheckTriggered(AEVK_LBUTTON) && Utils::OBBPoint(go, mwp)) {
			SelectEntity(go);
		}
	}

	if (AEInputCheckTriggered(AEVK_1)) {
		SelectEntity(AddEntity(Editor::GameObjectType::STATIC_PLATFORM));
	}

	if (AEInputCheckTriggered(AEVK_2)) {
		SelectEntity(AddEntity(Editor::GameObjectType::ENEMY_1));
	}

	if (AEInputCheckTriggered(AEVK_3)) {
		SelectEntity(AddEntity(Editor::GameObjectType::ENEMY_2));
	}

	if (currentSelection) {
		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			if (Utils::OBBPoint(currentSelection, mwp)) {
				currentSelection->position = mwp;
			}

			if (scroll != 0) {
				currentSelection->scale.x += scroll * dt;
			}
		}

		if (AEInputCheckCurr(AEVK_RBUTTON)) {
			if (scroll != 0) {
				currentSelection->scale.y += scroll * dt;
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

	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_S)) {
		char filename[50];
		sprintf_s(filename, 50, "Assets/level_%d.dat", level);
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

void LevelEditor::SetLevel(int l) {
	level = l;
}

