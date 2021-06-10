// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	~Asteroid();
	
	class CircleComponent* GetCircle() { return mCircle; }

    virtual void UpdateActor(float deltaTime) override;
    
private:
	class CircleComponent* mCircle;
    class MoveComponent* mMove;
    float mSpeed;
};
