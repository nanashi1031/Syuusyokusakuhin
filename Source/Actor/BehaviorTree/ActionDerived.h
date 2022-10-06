#pragma once
#include "ActionBase.h"
#include "EnemyBlueSlime.h"

// 通常攻撃
class AttackAction : public ActionBase
{
public:
	template<typename T>
	AttackAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// スキル攻撃行動
class SkillAction : public ActionBase
{
public:
	template<typename T>
	SkillAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 追跡行動
class PursuitAction : public ActionBase
{
public:
	template<typename T>
	PursuitAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 徘徊行動
class WanderAction : public ActionBase
{
public:
	template<typename T>
	WanderAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 待機行動
class IdleAction : public ActionBase
{
public:
	template<typename T>
	IdleAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 逃走行動
class LeaveAction : public ActionBase
{
public:
	template<typename T>
	LeaveAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// 回復行動
class RecoverAction : public ActionBase
{
public:
	template<typename T>
	RecoverAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};