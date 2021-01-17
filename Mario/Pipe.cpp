#include "Pipe.h"
#include "MEntityType.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "Game.h"

Pipe::Pipe()
{
    renderOrder = 1;
}

ObjectType Pipe::GetObjectType()
{
    return MEntityType::Pipe;
}

RectF Pipe::GetHitBox()
{
    return hitbox;
}

bool Pipe::IsGetThrough(GameObject& object, Direction direction)
{
    return false;
}

float Pipe::GetDamageFor(GameObject& object, Direction direction)
{
    return 0.0f;
}

void Pipe::Render()
{
    if (!head || !body) return;
    Vec2 cam = SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Position;
    Vec2 renderPos = Position - cam;

    switch (direction)
    {
    case Direction::Left:
        head->Draw(renderPos.x, renderPos.y, trans);
        for (int i = renderPos.x + lengthOfPiece; i < renderPos.x + size.x - 1; i += lengthOfPiece) {
            body->Draw(i, renderPos.y, trans);
        }
        break;
    case Direction::Top:
        head->Draw(renderPos.x, renderPos.y, trans);
        for (int i = renderPos.y + lengthOfPiece; i < renderPos.y + size.y - 1; i += lengthOfPiece) {
            body->Draw(renderPos.x, i, trans);
        }
        break;
    case Direction::Right:
        head->Draw(renderPos.x + size.x - lengthOfPiece, renderPos.y, trans);
        for (int i = renderPos.x; i < renderPos.x + size.x - lengthOfPiece - 1; i += lengthOfPiece) {
            body->Draw(i, renderPos.y, trans);
        }
        break;
    case Direction::Bottom:
        head->Draw(renderPos.x, renderPos.y + size.y - lengthOfPiece, trans);
        for (int i = renderPos.y; i < renderPos.y + size.y - lengthOfPiece - 1; i += lengthOfPiece) {
            body->Draw(renderPos.x, i, trans);
        }
        break;
    }
}

shared_ptr<Pipe> Pipe::CreatePipe(Vec2 pos, Vec2 size, MapProperties& props)
{
    shared_ptr<Pipe> pipe = make_shared<Pipe>();
    pipe->Position = pos;
    pipe->size = size;
    pipe->hitbox = RectF(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

    pipe->length = props.GetInt("Length");

    string drct = "vertical";

    switch (props.GetInt("Direction"))
    {
    case 1:
        pipe->direction = Direction::Top;
        pipe->lengthOfPiece = size.y / pipe->length;
        drct = "vertical";
        break;
    case 2:
        pipe->direction = Direction::Bottom;
        pipe->lengthOfPiece = size.y / pipe->length;
        drct = "vertical";
        break;
    case 3:
        pipe->direction = Direction::Left;
        pipe->lengthOfPiece = size.x / pipe->length;
        drct = "horizontal";
        break;
    case 4:
        pipe->direction = Direction::Right;
        pipe->lengthOfPiece = size.x / pipe->length;
        drct = "horizontal";
        break;
    }
    
    int type = props.GetInt("Type");

    pipe->head = SpriteManager::GetInstance()->Get("spr-pipe-" + to_string(type) + "-" + drct + "-head");
    pipe->body = SpriteManager::GetInstance()->Get("spr-pipe-" + to_string(type) + "-" + drct + "-body");

    return pipe;
}
