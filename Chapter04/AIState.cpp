// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include "AIState.h"
#include "AIComponent.h"
#include "Enemy.h"
#include "Bullet.h"
#include <SDL2/SDL_log.h>

void AIPatrol::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
	bool dead = true;
	if (dead)
	{
		mOwner->ChangeState("Death");
	}
}

void AIPatrol::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIPatrol::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void AIDeath::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void AIDeath::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIDeath::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void AIAttack::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void AIAttack::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIAttack::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void TowerSearchForEnemy::Update(float deltaTime)
{
    Actor* actor = mOwner->GetOwner();
    Enemy* e = actor->GetGame()->
                GetNearestEnemy(actor->GetPosition());
    if (e != nullptr)
    {
		// Vector from me to enemy
		Vector2 dir = e->GetPosition() - actor->GetPosition();
		float dist = dir.Length();
		if (dist < AttackRange)
        {
            mOwner->ChangeState("TowerAttackEnemy");
            return;
        }
    }
    actor->SetRotation(actor->GetRotation() + 0.1f);
}

void TowerSearchForEnemy::OnEnter()
{
    SDL_Log("Entering AI state %s\n", GetName());
}

void TowerSearchForEnemy::OnExit()
{
    SDL_Log("Exiting AI state %s\n", GetName());
}

void TowerAttackEnemy::Update(float deltaTime)
{
    Actor* actor = mOwner->GetOwner();

    mNextAttack -= deltaTime;
    if (mNextAttack <= 0.0f)
    {
        Enemy* e = actor->GetGame()->
                    GetNearestEnemy(actor->GetPosition());
        if (e == nullptr)
        {
            mOwner->ChangeState("TowerSearchForEnemy");
        }
        else
        {
			// Vector from me to enemy
			Vector2 dir = e->GetPosition() - actor->GetPosition();
			float dist = dir.Length();
			if (dist < AttackRange)
			{
				// Rotate to face enemy
				actor->SetRotation(Math::Atan2(-dir.y, dir.x));
				// Spawn bullet at tower position facing enemy
				Bullet* b = new Bullet(actor->GetGame());
				b->SetPosition(actor->GetPosition());
				b->SetRotation(actor->GetRotation());
			}
            else
            {
                mOwner->ChangeState("TowerSearchForEnemy");
            }
        }

        mNextAttack += AttackTime;
    }
}

void TowerAttackEnemy::OnEnter()
{
    SDL_Log("Entering AI state %s\n", GetName());
    mNextAttack = 0.0f; // be able to attack immediately upon enter
}

void TowerAttackEnemy::OnExit()
{
    SDL_Log("Exiting AI state %s\n", GetName());
}

