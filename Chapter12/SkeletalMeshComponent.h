// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include "MeshComponent.h"
#include "MatrixPalette.h"
#include "Shader.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
    ~SkeletalMeshComponent() {
        if (mBoneVertBuff != 0) {
            glDeleteBuffers(1, &mBoneVertBuff);
        }
        if (mBoneVertArr != 0) {
            glDeleteVertexArrays(1, &mBoneVertArr);
        }
        mBoneDebugShader.Unload();
    }
	// Draw this mesh component
	void Draw(class Shader* shader, Matrix4& viewProjMat) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(const class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);

    // returns current object space position of the bone of the current pose
    Vector3 GetBonePosition(int index);
    
protected:
	void ComputeMatrixPalette();
    void DrawBonePositions(Matrix4& viewProjMat);

	MatrixPalette mPalette;
	const class Skeleton* mSkeleton;
	const class Animation* mAnimation;
	float mAnimPlayRate;
	float mAnimTime;
    std::vector<Matrix4> mCurrentPoses;
    std::vector<Vector3> mCurrentBonePositions;
    Shader mBoneDebugShader;
    unsigned int mBoneVertBuff;
    unsigned int mBoneVertArr;
};
