#pragma once
#ifndef __BARUI_H
#define __BARUI_H
#include "BaseUI.hpp"

class BarUI : public BaseUI {
protected:
	f32 value;
	AEMtx33 overlay_transform;

public:
	BarUI(AEVec2 pos = { 0.f, 0.f });
	virtual ~BarUI();

	virtual void PostUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void Render();

	f32 GetValue() const;
	virtual void SetValue(f32 v);
};

#endif