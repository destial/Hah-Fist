#include "LevelEditor.hpp"
#include "../Utils/Utils.hpp"

#include "../Utils/MeshRenderer.hpp"
#include "../UI/Debug.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"

LevelEditor::LevelEditor(BaseScene* b_scene) : scene{ b_scene }, toggled{ false }, currentSelection{ nullptr } {}

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

	scene->RemoveEntityFromScene(currentSelection);
}

BaseEntity* LevelEditor::AddEntity(Editor::GameObjectType type) {
	switch (type) {
	case Editor::GameObjectType::STATIC_PLATFORM: {
		GameObjectEntity* go = new GameObjectEntity(Utils::GetMouseWorld(true));
		go->mesh = MeshRenderer::GetCenterRectMesh();
		go->go_type = GameObjectEntity::KINEMATIC::STATIC;
		scene->AddEntityToScene(go);
		return go;
	}
	case Editor::GameObjectType::ENEMY_1: {
		EnemyEntity* enemy = new EnemyEntity(Utils::GetMouseWorld(true));
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
	if ((fps_counter += Utils::GetDeltaTime()) > 0.1f) {
		cfps = 1.f / Utils::GetDeltaTime();
		fps_counter = 0.f;
	}
	char fps[50];
	sprintf_s(fps, "FPS:%.0f", cfps);
	DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 0.25f }, std::string{fps});

	float x, y;
	AEGfxGetCamPosition(&x, &y);
	char cam[128];
	sprintf_s(cam, "Cam:%.2f,%.2f", x, y);
	DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 0.75f }, std::string{ cam });
	
	AEVec2 mwp = Utils::GetMouseWorld(true);
	char mos[128];
	sprintf_s(mos, "Mouse:%.2f,%.2f", mwp.x, mwp.y);
	DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 1.25f }, std::string{ mos });

	for (BaseEntity* go : scene->Entities()) {
		if (AEInputCheckTriggered(AEVK_LBUTTON) && Utils::OBBPoint(go, mwp)) {
			SelectEntity(go);
		}
	}

	if (AEInputCheckTriggered(AEVK_1)) {
		SelectEntity(AddEntity(Editor::GameObjectType::STATIC_PLATFORM));
	}

	if (currentSelection && AEInputCheckCurr(AEVK_LBUTTON) && Utils::OBBPoint(currentSelection, mwp)) {
		currentSelection->position = mwp;
	}

	if (AEInputCheckCurr(AEVK_LEFT)) {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		x -= dt * 100;
		AEGfxSetCamPosition(x, y);
	}
	if (AEInputCheckCurr(AEVK_RIGHT)) {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		x += dt * 100;
		AEGfxSetCamPosition(x, y);
	}
	if (AEInputCheckCurr(AEVK_UP)) {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		y += dt * 100;
		AEGfxSetCamPosition(x, y);
	}
	if (AEInputCheckCurr(AEVK_DOWN)) {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		y -= dt * 100;
		AEGfxSetCamPosition(x, y);
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

