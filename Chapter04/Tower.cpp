// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Tower.h"
#include "AIComponent.h"
#include "AIState.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"

Tower::Tower(class Game* game)
:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));
	
	mMove = new MoveComponent(this);

    // AI components
    mAi = new AIComponent(this);
    TowerSearchForEnemy* search = new TowerSearchForEnemy(mAi, AttackRange);
    TowerAttackEnemy* attack = new TowerAttackEnemy(mAi, AttackRange, AttackTime);
    mAi->RegisterState(search);
    mAi->RegisterState(attack);
    mAi->ChangeState("TowerSearchForEnemy");
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

