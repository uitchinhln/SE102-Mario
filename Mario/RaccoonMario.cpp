#include "RaccoonMario.h"

RaccoonMario::RaccoonMario(shared_ptr<Mario> mario) : MarioPowerUp(mario)
{
}

ObjectType RaccoonMario::GetMarioType()
{
    return MEntityType::RaccoonMario;
}
