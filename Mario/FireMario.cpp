#include "FireMario.h"

FireMario::FireMario(shared_ptr<Mario> mario) : MarioPowerUp(mario)
{

}

ObjectType FireMario::GetMarioType()
{
    return MEntityType::FireMario;
}
