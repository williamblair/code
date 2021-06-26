// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PhysWorld.h"
#include <algorithm>
#include <map>
#include "BoxComponent.h"
#include <SDL2/SDL.h>

PhysWorld::PhysWorld(Game* game)
	:mGame(game)
{
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
	bool collided = false;
	// Initialize closestT to infinity, so first
	// intersection will always update closestT
	float closestT = Math::Infinity;
	Vector3 norm;
	// Test against all boxes
	for (auto box : mXBoxes)
	{
		float t;
		// Does the segment intersect with the box?
		if (Intersect(l, box->GetWorldBox(), t, norm))
		{
			// Is this closer than previous intersection?
			if (t < closestT)
			{
				closestT = t;
				outColl.mPoint = l.PointOnSegment(t);
				outColl.mNormal = norm;
				outColl.mBox = box;
				outColl.mActor = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

void PhysWorld::TestPairwise(std::function<void(Actor*, Actor*)> f)
{
	// Naive implementation O(n^2)
	for (size_t i = 0; i < mXBoxes.size(); i++)
	{
		// Don't need to test vs itself and any previous i values
		for (size_t j = i + 1; j < mXBoxes.size(); j++)
		{
			BoxComponent* a = mXBoxes[i];
			BoxComponent* b = mXBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				// Call supplied function to handle intersection
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
	// Sort by min.x, min.y, min.z
	std::sort(mXBoxes.begin(), mXBoxes.end(),
		[](BoxComponent* a, BoxComponent* b) {
			return a->GetWorldBox().mMin.x <
				b->GetWorldBox().mMin.x;
	});
	std::sort(mYBoxes.begin(), mYBoxes.end(),
		[](BoxComponent* a, BoxComponent* b) {
			return a->GetWorldBox().mMin.y <
				b->GetWorldBox().mMin.y;
	});
	std::sort(mZBoxes.begin(), mZBoxes.end(),
		[](BoxComponent* a, BoxComponent* b) {
			return a->GetWorldBox().mMin.z <
				b->GetWorldBox().mMin.z;
	});

    // Prune X axis
    std::map<BoxComponent*,BoxComponent*> xOverlaps;
	for (size_t i = 0; i < mXBoxes.size(); i++)
	{
		// Get max.x for current box
		BoxComponent* a = mXBoxes[i];
		float max = a->GetWorldBox().mMax.x;
		for (size_t j = i + 1; j < mXBoxes.size(); j++)
		{
			BoxComponent* b = mXBoxes[j];
			// If AABB[j] min is past the max bounds of AABB[i],
			// then there aren't any other possible intersections
			// against AABB[i]
			if (b->GetWorldBox().mMin.x > max)
			{
				break;
			}
			//else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			//{
			//	f(a->GetOwner(), b->GetOwner());
			//}
            xOverlaps[a] = b;
		}
	}

    // Prune Y axis
    std::map<BoxComponent*,BoxComponent*> yOverlaps;
	for (size_t i = 0; i < mYBoxes.size(); i++)
	{
		// Get max.x for current box
		BoxComponent* a = mYBoxes[i];
		float max = a->GetWorldBox().mMax.y;
		for (size_t j = i + 1; j < mYBoxes.size(); j++)
		{
			BoxComponent* b = mYBoxes[j];
			// If AABB[j] min is past the max bounds of AABB[i],
			// then there aren't any other possible intersections
			// against AABB[i]
			if (b->GetWorldBox().mMin.y > max)
			{
				break;
			}
			//else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			//{
			//	f(a->GetOwner(), b->GetOwner());
			//}
            yOverlaps[a] = b;
		}
	}

    // Prune Z axis
    std::map<BoxComponent*,BoxComponent*> zOverlaps;
	for (size_t i = 0; i < mZBoxes.size(); i++)
	{
		// Get max.x for current box
		BoxComponent* a = mZBoxes[i];
		float max = a->GetWorldBox().mMax.z;
		for (size_t j = i + 1; j < mZBoxes.size(); j++)
		{
			BoxComponent* b = mZBoxes[j];
			// If AABB[j] min is past the max bounds of AABB[i],
			// then there aren't any other possible intersections
			// against AABB[i]
			if (b->GetWorldBox().mMin.z > max)
			{
				break;
			}
			//else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
			//{
			//	f(a->GetOwner(), b->GetOwner());
			//}
            zOverlaps[a] = b;
		}
	}

    for (auto& xIter : xOverlaps)
    {
        BoxComponent* a = xIter.first;
        BoxComponent* b = xIter.second;
        auto yIter = yOverlaps.find(a);
        auto zIter = zOverlaps.find(a);
        if (yIter != yOverlaps.end() &&
            zIter != zOverlaps.end() &&
            yIter->second == b &&
            zIter->second == b)
        {
			f(a->GetOwner(), b->GetOwner());
        }
    }
}

void PhysWorld::AddBox(BoxComponent* box)
{
	mXBoxes.emplace_back(box);
	mYBoxes.emplace_back(box);
	mZBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
    std::vector<BoxComponent*>* boxArrs[3] = {
        &mXBoxes, &mYBoxes, &mZBoxes
    };
    for (int i=0; i<3; ++i)
    {
        auto boxes = boxArrs[i];
        auto iter = std::find(boxes->begin(), boxes->end(), box);
        if (iter != boxes->end())
        {
            // Swap to end of vector and pop off (avoid erase copies)
            std::iter_swap(iter, boxes->end() - 1);
            boxes->pop_back();
        }
    }
}
