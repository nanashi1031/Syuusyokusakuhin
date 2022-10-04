#pragma once
#include "JudgmentBase.h"
#include "EnemyBlueSlime.h"


// BattleNode�ɑJ�ڂł��邩����
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// AttackNode�ɑJ�ڂł��邩����
class AttackJudgment : public JudgmentBase
{
public:
	AttackJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// SkillNode�ɑJ�ڂł��邩����
class SkillShotJudgment : public JudgmentBase
{
public:
	SkillShotJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// WanderNode�ɑJ�ڂł��邩����
class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};


// Escape�ɑJ�ڂł��邩����
class EscapeJudgment : public JudgmentBase
{
public:
	EscapeJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};