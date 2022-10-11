#pragma once
#include "JudgmentBase.h"
#include "Enemy.h"


// BattleNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class BattleJudgment : public JudgmentBase
{
public:
	template<typename T>
	BattleJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// AttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class AttackJudgment : public JudgmentBase
{
public:
	template<typename T>
	AttackJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// SkillNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class SkillShotJudgment : public JudgmentBase
{
public:
	template<typename T>
	SkillShotJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};

// WanderNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class WanderJudgment : public JudgmentBase
{
public:
	template<typename T>
	WanderJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};


// Escape‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class EscapeJudgment : public JudgmentBase
{
public:template<typename T>
	EscapeJudgment(T* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();
};