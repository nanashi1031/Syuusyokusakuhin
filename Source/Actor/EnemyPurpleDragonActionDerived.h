#pragma once
#include "ActionBase.h"
#include"EnemyPurpleDragon.h"

// 通常攻撃
class AttackAction : public ActionBase
{
public:
	AttackAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// スキル攻撃行動
class SkillAction : public ActionBase
{
public:
	SkillAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// 追跡行動
class PursuitAction : public ActionBase
{
public:
	PursuitAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// 徘徊行動
class WanderAction : public ActionBase
{
public:
	WanderAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// 待機行動
class IdleAction : public ActionBase
{
public:
	IdleAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// 逃走行動
class LeaveAction : public ActionBase
{
public:
	LeaveAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// 回復行動
class RecoverAction : public ActionBase
{
public:
	RecoverAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};