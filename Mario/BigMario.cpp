#include "BigMario.h"

BigMario::BigMario(shared_ptr<Mario> mario) : MarioPowerUp(mario)
{
}

int BigMario::GetEntityType()
{
	return 0;
}
