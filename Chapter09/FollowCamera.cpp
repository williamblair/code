// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowCamera.h"
#include "Actor.h"

FollowCamera::FollowCamera(Actor* owner)
	:CameraComponent(owner)
	,mHorzDist(350.0f)
	,mVertDist(150.0f)
	,mTargetDist(100.0f)
	,mSpringConstant(64.0f)
	,mUp(Vector3::UnitZ)
	,mPitchSpeed(0.0f)
	,mYawSpeed(0.0f)
{
}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// Compute dampening from spring constant
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);
	// Compute ideal position
	Vector3 idealPos = ComputeCameraPos();
	// Compute difference between actual and ideal
	Vector3 diff = mActualPos - idealPos;
	// Compute acceleration of spring
	Vector3 acel = -mSpringConstant * diff -
		dampening * mVelocity;
	// Update velocity
	mVelocity += acel * deltaTime;
	// Update actual camera position
	mActualPos += mVelocity * deltaTime;
	
	// Ideal Up (technically doesn't spring like above)
	Vector3 idealUp = Vector3::UnitZ;
	diff = mUp - idealUp;
	mUp -= diff * deltaTime;
	
	// Create a quaternion for yaw about world up
	Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
	// Transform actual pos by yaw
	mActualPos = Vector3::Transform(mActualPos, yaw);

	// Target is target dist in front of owning actor
	Vector3 target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	
	// Calculate camera forward (from current pos to target pos)
	// then right from that
	Vector3 forward = target - mActualPos;
	forward.Normalize();
	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();
	
	// Create quaternion for pitch about camera right
	Quaternion pitch(right, mPitchSpeed * deltaTime);
	// Transform camera offset and up by pitch
	mActualPos = Vector3::Transform(mActualPos, pitch);
	mUp = Vector3::Transform(mUp, pitch);

	// Use actual position here, not ideal
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, mUp);
	SetViewMatrix(view);
}

void FollowCamera::SnapToIdeal()
{
	// Set actual position to ideal
	mActualPos = ComputeCameraPos();
	// Zero velocity
	mVelocity = Vector3::Zero;
	// Compute target and view
	Vector3 target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target,
		Vector3::UnitZ);
	SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const
{
	// Set camera position behind and above owner
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3::UnitZ * mVertDist;
	return cameraPos;
}
