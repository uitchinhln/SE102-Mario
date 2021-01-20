#include "Font.h"

Font::Font()
{
}

void Font::Import(vector<FontSprite> fontSprites)
{
    dictionary.clear();
    for (auto fontSprite : fontSprites)
        dictionary[fontSprite.character] = SpriteManager::GetInstance()->Get(fontSprite.spriteId);
}

Sprite Font::GetChar(char c)
{
    if (dictionary.find(c) == dictionary.end()) return dictionary.at('0');
    return dictionary.at(c);
}

int Font::GetSpaceWidth()
{
    return this->SPACE_WIDTH;
}
