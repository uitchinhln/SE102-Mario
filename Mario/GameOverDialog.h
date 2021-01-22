#pragma once
#include "Dialog.h"
#include "Transform.h"

enum class GameOverDialogState {
	CONTINUE,
	END
};

class GameOverDialog :
	public Dialog
{
public:
	GameOverDialog();

	virtual void Update() override;
	virtual void Render(D3DCOLOR overlay) override;

	virtual void OnKeyUp(int key) override;
	virtual void OnKeyDown(int key) override;

protected:
	GameOverDialogState state = GameOverDialogState::CONTINUE;

	Vec2 posArrow1 = Vec2(150, 120);
	Vec2 posArrow2 = Vec2(150, 147);

	Transform trans;
};

