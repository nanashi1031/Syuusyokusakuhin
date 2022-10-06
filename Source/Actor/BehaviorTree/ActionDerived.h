#pragma once
#include "ActionBase.h"
#include "EnemyBlueSlime.h"

// �ʏ�U��
class AttackAction : public ActionBase
{
public:
	template<typename T>
	AttackAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �X�L���U���s��
class SkillAction : public ActionBase
{
public:
	template<typename T>
	SkillAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �ǐՍs��
class PursuitAction : public ActionBase
{
public:
	template<typename T>
	PursuitAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �p�j�s��
class WanderAction : public ActionBase
{
public:
	template<typename T>
	WanderAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �ҋ@�s��
class IdleAction : public ActionBase
{
public:
	template<typename T>
	IdleAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �����s��
class LeaveAction : public ActionBase
{
public:
	template<typename T>
	LeaveAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};

// �񕜍s��
class RecoverAction : public ActionBase
{
public:
	template<typename T>
	RecoverAction(T* enemy) :ActionBase(enemy) {}
	ActionBase::State Run(float elapsedTime);
};