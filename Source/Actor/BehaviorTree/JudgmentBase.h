#pragma once

class Enemy;

// é¿çsîªíË
class JudgmentBase
{
public:
	template<typename T>
	JudgmentBase(T* actor) :owner(actor) {}
	virtual bool Judgment() = 0;
protected:
	Enemy* owner;
};