#include "ICallbackEffect.h"
#include "MarioGame.h"

ICallbackEffect::ICallbackEffect(Vec2 pos, long playTime, function<void(long)> callback) : IEffect(pos, playTime)
{
	this->callback = callback;
}

void ICallbackEffect::Update()
{
	IEffect::Update();

	playTime += MarioGame::Time().ElapsedGameTime;

	if (!Active) {
		callback(playTime);
	}
}
