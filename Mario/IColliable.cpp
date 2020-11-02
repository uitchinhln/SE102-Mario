#include "IColliable.h"
#include "GameObject.h"

bool IColliable::IsActive()
{
    return active;
}

void IColliable::SetActive(bool value)
{
    active = value;
}
