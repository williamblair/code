// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;
	
	float GetAngularSpeed() const { return mAngularSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }

    // Impulse force; acceleration is cleared every update
    void AddForce(const Vector2& force) {
        mAccel += force;
    }

private:
	// Controls rotation (radians/second)
	float mAngularSpeed;

    // positional acceleration
    Vector2 mAccel;
    
    // positional velocity
    Vector2 mVelocity;

    // maximum velocity
    float mMaxSpeed;
};
