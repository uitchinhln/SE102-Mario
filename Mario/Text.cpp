#include "Text.h"

Text::Text()
{
	font = nullptr;
	spacing = 0;
	alignment = TextAlignment::Right;
}

void Text::Update()
{
}

void Text::Render()
{
	if (font == nullptr) return;

	float textWidth = 0;

	for each (char letter in content)
	{
		if (letter == ' ') {
			textWidth += font->GetSpaceWidth();
			continue;
		}
		textWidth += spacing;
		textWidth += font->GetChar(letter)->width;
	}

	float begin = Position.x;
	float end = min(Position.x + Size.x, Position.x + textWidth);

	if (alignment == TextAlignment::Right) {
		begin = max(Position.x, Position.x + Size.x - textWidth);
		end = Position.x + Size.x;
	}

	float currentX = begin;

	for each (char letter in content)
	{
		if (letter == ' ') {
			currentX += font->GetSpaceWidth();
			continue;
		}
		
		Sprite c = font->GetChar(letter);

		if (currentX + c->width > end) break;

		c->Draw(currentX + c->width / 2, Position.y + Size.y - c->height / 2, trans);

		currentX += c->width + spacing;
	}
}

void Text::SetContent(string text)
{
	this->content = text;
	for (auto c : text) c = toupper(c);
}

string Text::GetContent()
{
    return this->content;
}

void Text::SetSpacing(int spacing)
{
	this->spacing = spacing;
}

void Text::SetAlignment(TextAlignment alignment)
{
	if (this->alignment == alignment) return;

	this->alignment = alignment;
}

void Text::SetFont(Font* font)
{
	this->font = font;
}
