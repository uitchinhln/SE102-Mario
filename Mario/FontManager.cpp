#include "FontManager.h"

FontManager* FontManager::__instance = NULL;

FontManager* FontManager::GetInstance()
{
	if (__instance == nullptr) {
		__instance = new FontManager();
	}
	return __instance;
}

FontManager::FontManager() {

}