#include "BigMario.h"
#include "MEntityType.h"

BigMario::BigMario(shared_ptr<Mario> mario) : MarioPowerUp(mario)
{
}

ObjectType BigMario::GetMarioType()
{
	return MEntityType::BigMario;
}
