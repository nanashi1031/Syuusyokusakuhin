#pragma once
#include "JudgmentBase.h"
#include "Enemy.h"


// BattleNodeに遷移できるか判定
class BattleJudgment : public JudgmentBase
{
public:
	template<typename T>
	BattleJudgment(T* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// AttackNodeに遷移できるか判定
class AttackJudgment : public JudgmentBase
{
public:
	template<typename T>
	AttackJudgment(T* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// SkillNodeに遷移できるか判定
class SkillShotJudgment : public JudgmentBase
{
public:
	template<typename T>
	SkillShotJudgment(T* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// WanderNodeに遷移できるか判定
class WanderJudgment : public JudgmentBase
{
public:
	template<typename T>
	WanderJudgment(T* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};


// Escapeに遷移できるか判定
class EscapeJudgment : public JudgmentBase
{
public:template<typename T>
	EscapeJudgment(T* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};