#pragma once
#ifndef __LEVEL_EDITOR_H
#define __LEVEL_EDITOR_H
#include "AEEngine.h"
#include "../Scenes/BaseScene.hpp"

namespace Editor {
	enum GameObjectType {
		STATIC_PLATFORM,
		ENEMY_1,
		ENEMY_2,
		BOSS_1,
	};
};

class LevelEditor {
private:
	bool toggled;
	BaseScene* scene;
	BaseEntity* currentSelection;
	int level;
public:
	LevelEditor(BaseScene* scene);
	~LevelEditor();

	void Toggle();
	bool IsToggled() const;

	void SetScene(BaseScene* scene);

	void SelectEntity(BaseEntity* entity);
	void RemoveSelectedEntity();
	BaseEntity* AddEntity(Editor::GameObjectType type);

	void Update(const f32& dt);
	void Render();

	void SetLevel(int l);
};

#endif
