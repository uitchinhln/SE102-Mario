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
    if (!('0' <= c && c <= '9') && !('A' <= c && c <= 'Z')) return nullptr;
    return dictionary.at(c);
}

int Font::GetSpaceWidth()
{
    return this->SPACE_WIDTH;
}
