#pragma once
#include "Dialog.h"
#include "Transform.h"

enum class MenuDialogState {
	FIRST,
	SECOND
};

class MenuDialog :
    public Dialog
{
public:
	MenuDialog();

	virtual void Update() override;
	virtual void Render(D3DXCOLOR overlay) override;

	virtual void OnKeyUp(int key) override;
	virtual void OnKeyDown(int key) override;

protected:
	MenuDialogState state = MenuDialogState::FIRST;

	Vec2 posArrow1 = Vec2(-24, 1);
	Vec2 posArrow2 = Vec2(-24, 49);

	Transform trans;
};

