// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include <cstdlib>

#define BONE_DEBUG_VERT_SHADER "Shaders/BoneDebug.vert"
#define BONE_DEBUG_FRAG_SHADER "Shaders/BoneDebug.frag"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	:MeshComponent(owner, true)
	,mSkeleton(nullptr)
    ,mBoneVertBuff(0)
    ,mBoneVertArr(0)
{
    if (!mBoneDebugShader.Load(BONE_DEBUG_VERT_SHADER,
                               BONE_DEBUG_FRAG_SHADER)) {
        SDL_Log("Failed to load bone debug shader\n");
        exit(EXIT_FAILURE);
    }
}

void SkeletalMeshComponent::Draw(Shader* shader, Matrix4& viewProjMat)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		// Set the matrix palette
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], 
			MAX_SKELETON_BONES);
		// Set specular power
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);

        // debug draw skeleton joint positions
        DrawBonePositions(viewProjMat);
        
        // restore the given shader as active
        shader->SetActive();
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;
		// Wrap around anim time if past duration
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}

		// Recompute matrix palette
		ComputeMatrixPalette();

        // Get bone positions
        if (mCurrentBonePositions.size() != mCurrentPoses.size()) { 
            mCurrentBonePositions.resize(mCurrentPoses.size());
        }
        for (int i = 0; i < mCurrentPoses.size(); ++i) {
            mCurrentBonePositions[i] = GetBonePosition(i);
        }
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)
{
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;

	if (!mAnimation) { return 0.0f; }

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}

Vector3 SkeletalMeshComponent::GetBonePosition(int index)
{
    Matrix4& pose = mCurrentPoses[index];
    Vector3 position(0.0f, 0.0f, 0.0f);
    position = Vector3::Transform(position, pose);
    return position;
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();
	//std::vector<Matrix4> currentPoses;
	//mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);
	mAnimation->GetGlobalPoseAtTime(mCurrentPoses, mSkeleton, mAnimTime);

    // generate debug skeleton positions draw buffers if not already done
    if (mBoneVertBuff == 0 ||
        mBoneVertArr == 0)
    {
        glGenVertexArrays(1, &mBoneVertArr);
        glBindVertexArray(mBoneVertArr);

        // Get bone positions
        if (mCurrentBonePositions.size() != mCurrentPoses.size()) { 
            mCurrentBonePositions.resize(mCurrentPoses.size());
        }
        for (int i = 0; i < mCurrentPoses.size(); ++i) {
            mCurrentBonePositions[i] = GetBonePosition(i);
        }

        // send initial buffer data
        glGenBuffers(1, &mBoneVertBuff);
        glBindBuffer(GL_ARRAY_BUFFER, mBoneVertBuff);
        glBufferData(GL_ARRAY_BUFFER, mCurrentBonePositions.size() * sizeof(Vector3), 
            mCurrentBonePositions.data(), GL_DYNAMIC_DRAW);
    }

	// Setup the palette for each bone
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// Global inverse bind pose matrix times current pose matrix
		//mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
		mPalette.mEntry[i] = globalInvBindPoses[i] * mCurrentPoses[i];
	}
}

void SkeletalMeshComponent::DrawBonePositions(Matrix4& viewProjMat)
{
    // Use and update shader uniforms
    mBoneDebugShader.SetActive();
    mBoneDebugShader.SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
    mBoneDebugShader.SetMatrixUniform("uViewProj", viewProjMat);

    // Make points easier to see
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5.0f);

    // always want to see the bone points
    glDisable(GL_DEPTH_TEST);

    // Update bone positions buffer data
    glBindVertexArray(mBoneVertArr);
    glBindBuffer(GL_ARRAY_BUFFER, mBoneVertBuff);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mCurrentBonePositions.size() * sizeof(Vector3), 
        mCurrentBonePositions.data());
    glDrawArrays(GL_POINTS, 0, mCurrentBonePositions.size());

    // unbind vertex attrib array
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    // reenable depth testing and use default point size
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_PROGRAM_POINT_SIZE);

}

