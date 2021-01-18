#include "GameEvent.h"

GameEvent* GameEvent::__instance = nullptr;

GameEvent* GameEvent::GetInstance()
{
    if (__instance == nullptr) {
        __instance = new GameEvent();
    }
    return __instance;
}

GameEvent::GameEvent()
{
}