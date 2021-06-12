// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

class AIState
{
public:
	AIState(class AIComponent* owner)
		:mOwner(owner)
	{ }
	// State-specific behavior
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	// Getter for string name of state
	virtual const char* GetName() const = 0;
protected:
	class AIComponent* mOwner;
};

class AIPatrol : public AIState
{
public:
	AIPatrol(class AIComponent* owner)
		:AIState(owner)
	{ }

	// Override with behaviors for this state
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{ return "Patrol"; }
};

class AIDeath : public AIState
{
public:
	AIDeath(class AIComponent* owner)
		:AIState(owner)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{ return "Death"; }
};

class AIAttack : public AIState
{
public:
	AIAttack(class AIComponent* owner)
		:AIState(owner)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{ return "Attack"; }
};

// Each update, checks if enemy is nearby and switches to it; else rotates
class TowerSearchForEnemy : public AIState
{
public:
	TowerSearchForEnemy(class AIComponent* owner, const float attackRange)
		:AIState(owner)
        ,AttackRange(attackRange)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{ return "TowerSearchForEnemy"; }
private:
	const float AttackRange;
};

// Each update, checks for nearby enemy and attacks if cooldown is valid;
// if no enemy found switch to searching state
class TowerAttackEnemy : public AIState
{
public:
	TowerAttackEnemy(class AIComponent* owner,
                     const float attackRange,
                     const float attackTime)
		:AIState(owner)
        ,mNextAttack(0.0f) // be able to fire immediately
        ,AttackRange(attackRange)
        ,AttackTime(attackTime)
	{ }

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{ return "TowerAttackEnemy"; }
private:
	float mNextAttack;
	const float AttackRange;
	const float AttackTime;
};

