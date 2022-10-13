#pragma once
#include "ActionBase.h"
#include"EnemyPurpleDragon.h"

// �ʏ�U��
class AttackAction : public ActionBase
{
public:
	AttackAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// �X�L���U���s��
class SkillAction : public ActionBase
{
public:
	SkillAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// �ǐՍs��
class PursuitAction : public ActionBase
{
public:
	PursuitAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// �p�j�s��
class WanderAction : public ActionBase
{
public:
	WanderAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// �ҋ@�s��
class IdleAction : public ActionBase
{
public:
	IdleAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// �����s��
class LeaveAction : public ActionBase
{
public:
	LeaveAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};

// �񕜍s��
class RecoverAction : public ActionBase
{
public:
	RecoverAction(EnemyPurpleDragon* actor) :ActionBase(actor) {}
	ActionBase::State Run(float elapsedTime);
};