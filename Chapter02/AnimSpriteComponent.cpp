// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"
#include <cstdio>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
    , mCurrAnim(0)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;

        // limit frame if finished and not looping
        if (mCurrFrame >= mAnims[mCurrAnim].last + 1 &&
            !mAnims[mCurrAnim].looping)
        {
            mCurrFrame -= mAnimFPS * deltaTime;
        }

		// Wrap current frame if needed
		while (mAnims[mCurrAnim].looping &&
               mCurrFrame >= mAnims[mCurrAnim].last + 1)
		{
            mCurrFrame -= mAnims[mCurrAnim].last -
                          mAnims[mCurrAnim].first + 1;
		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(
    const std::vector<SDL_Texture*>& textures,
    const std::vector<AnimRange>& animRanges)
{
	mAnimTextures = textures;
    mAnims = animRanges;
	if (mAnimTextures.size() > 0)
	{
        // a single animation
        if (mAnims.size() == 0)
        {
    		// Set the active texture to first frame
    		mCurrFrame = 0.0f;
    		SetTexture(mAnimTextures[0]);

            // set animation to be entire set of textures and looping
            mAnims.push_back({
                0,
                (int)mAnimTextures.size()-1,
                true
            });
        }
        else
        {
            // default to the first animation
            mCurrFrame = animRanges[0].first;
            SetTexture(mAnimTextures[animRanges[0].first]);
        }
	}
}
