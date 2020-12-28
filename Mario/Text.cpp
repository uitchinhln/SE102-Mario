#include "Text.h"

Text::Text()
{
	font = nullptr;
	spacing = -3;
	alignment = TextAlignment::Left;
}

void Text::Render()
{
	if (font == nullptr) return;

	int start = 0, end = 0, step = 0;
	switch (alignment)
	{
	case TextAlignment::Left:
		start = 0, end = text.length() - 1, step = 1;
		break;
	case TextAlignment::Right:
		start = text.length() - 1, end = 0, step = -1;
		break;
	}

	int x = 0;
	for (int i = start; (step > 0 ? i <= end : i >= end); i += step)
	{
		char ch = text.at(i);

		if (ch == ' ')
		{
			x += font->GetSpaceWidth() * (alignment == TextAlignment::Left ? 1 : -1);
			continue;
		}

		Sprite charSprite = font->GetChar(ch);

		if (charSprite == nullptr) continue;

		auto piv = (alignment == TextAlignment::Left ? 0 : charSprite->width);

		float oldpv = charSprite->pivot.x;
		charSprite->pivot.x = piv;
		charSprite->Draw(Position.x + x, Position.y, Transform());
		charSprite->pivot.x = oldpv;

		auto charStep = 0;
		if (alignment == TextAlignment::Left)
			charStep = (charSprite->width + spacing);
		else if (alignment == TextAlignment::Right && i > end)
			charStep = -(charSprite->width + spacing);

		x += charStep;
	}
}

void Text::SetContent(string text)
{
	this->text = text;
	for (auto c : text) c = toupper(c);
}

string Text::GetContent()
{
    return this->text;
}

void Text::SetSpacing(int spacing)
{
	this->spacing = spacing;
}

void Text::SetAlignment(TextAlignment alignment)
{
	this->alignment = alignment;
}

void Text::SetFont(Font* font)
{
	this->font = font;
}
