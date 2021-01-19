#pragma once
#include "Utils.h"
#include "Font.h"
#include "UIComponents.h"

enum class TextAlignment
{
	Left, Right
};


class Text : 
	public UIComponents
{
public:
	Text();

	virtual void Update() override;
	virtual void Render() override;

	virtual void SetContent(string text);
	virtual string GetContent();

	virtual void SetSpacing(int spacing);
	virtual void SetAlignment(TextAlignment alignment);

	virtual void SetFont(Font* font);

protected:
	int renderOrder;

	string content;

	int spacing;

	Font* font;

	TextAlignment alignment;

	Transform trans;
};

