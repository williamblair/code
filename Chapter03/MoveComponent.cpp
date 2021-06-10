// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
:Component(owner, updateOrder)
,mAngularSpeed(0.0f)
,mMaxSpeed(500.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	
    // apply acceleration
    mVelocity += mAccel * deltaTime;

    // limit speed
    if (mVelocity.x > mMaxSpeed || mVelocity.x < -mMaxSpeed)
    {
        mVelocity.x = mMaxSpeed * (mVelocity.x < 0 ? -1 : 1);
    }
    if (mVelocity.y > mMaxSpeed || mVelocity.y < -mMaxSpeed)
    {
        mVelocity.y = mMaxSpeed * (mVelocity.y < 0 ? -1 : 1);
    }
    if (!Math::NearZero(mVelocity.LengthSq()))
    {
        Vector2 pos = mOwner->GetPosition();
        pos += mVelocity * deltaTime;

		// (Screen wrapping code only for asteroids)
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

        mOwner->SetPosition(pos);

        // apply 'gravity' to auto slowdown
        mVelocity.x += (mVelocity.x > 0 ? -1 : 1) * 50.0f * deltaTime;
        mVelocity.y += (mVelocity.y > 0 ? -1 : 1) * 50.0f * deltaTime;
    }
    // reset acceleration
    mAccel = Vector2(0.0f,0.0f);
}

