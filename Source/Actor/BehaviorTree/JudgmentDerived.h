#pragma once
#include "JudgmentBase.h"
#include "Enemy.h"


// BattleNode�ɑJ�ڂł��邩����
class BattleJudgment : public JudgmentBase
{
public:
	template<typename T>
	BattleJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// AttackNode�ɑJ�ڂł��邩����
class AttackJudgment : public JudgmentBase
{
public:
	template<typename T>
	AttackJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// SkillNode�ɑJ�ڂł��邩����
class SkillShotJudgment : public JudgmentBase
{
public:
	template<typename T>
	SkillShotJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// WanderNode�ɑJ�ڂł��邩����
class WanderJudgment : public JudgmentBase
{
public:
	template<typename T>
	WanderJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};


// Escape�ɑJ�ڂł��邩����
class EscapeJudgment : public JudgmentBase
{
public:template<typename T>
	EscapeJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};