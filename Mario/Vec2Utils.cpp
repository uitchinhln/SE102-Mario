#include "Vec2Utils.h"

float Vec2Utils::DotProduct(Vec2 vec1, Vec2 v2)
{
    return vec1.x * v2.x + vec1.y * v2.y;
}

Vec2 Vec2Utils::Normalize(Vec2 source)
{
    float magnitude = sqrt(source.x * source.x + source.y * source.y);
    return Vec2(source.x / magnitude, source.y / magnitude);
}
