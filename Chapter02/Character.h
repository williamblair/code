#pragma once
#include <cstdint>
#include "Actor.h"

class Character : public Actor
{
public:
	Character(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
private:
    int mCurrAnim;
    class AnimSpriteComponent* mSpr;
};

