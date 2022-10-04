#pragma once
#include "JudgmentBase.h"
#include "EnemyBlueSlime.h"


// BattleNodeに遷移できるか判定
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// AttackNodeに遷移できるか判定
class AttackJudgment : public JudgmentBase
{
public:
	AttackJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// SkillNodeに遷移できるか判定
class SkillShotJudgment : public JudgmentBase
{
public:
	SkillShotJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// WanderNodeに遷移できるか判定
class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};


// Escapeに遷移できるか判定
class EscapeJudgment : public JudgmentBase
{
public:
	EscapeJudgment(EnemyBlueSlime* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};