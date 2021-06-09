// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <vector>

// Specifies an animation subset of a set of textures
struct AnimRange
{
    int first;
    int last;
    bool looping;
};

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation every frame (overriden from component)
	void Update(float deltaTime) override;
	// Set the textures used for animation
    // each entry in animRanges represents the first and last index in textures
    //  for that animation. Send an empty vector if there is just a single animation.
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures,
                         const std::vector<AnimRange>& animRanges);
    // The entry in animRanges to use
    void SetAnim(int anim) {
        mCurrAnim = anim; 
        mCurrFrame = mAnims[mCurrAnim].first;
    }
	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
    // Indices of each first and last mAnimTextures entry for that animation
    std::vector<AnimRange> mAnims;
    // which entry in mAnims to use
    int mCurrAnim;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
};
