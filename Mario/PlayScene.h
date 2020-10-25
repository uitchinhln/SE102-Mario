#pragma once
#include "Scene.h"
#include "Mario.h"

class PlayScene :
    public CScene
{
public:
	void LoadFromXml(TiXmlElement* data);

	void Load();

	void Update();

	void Render();

	virtual void OnKeyDown(int key) override;

	virtual void OnKeyUp(int key) override;

private:
	shared_ptr<Mario> mario;
};

