#include "Events.h"

Events* Events::__instance = nullptr;

Events* Events::GetInstance()
{
    if (__instance == nullptr) {
        __instance = new Events();
    }
    return __instance;
}

Events::Events()
{
}

Events::~Events()
{
}
