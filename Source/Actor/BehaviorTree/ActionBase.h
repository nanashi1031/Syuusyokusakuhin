#pragma once

class Enemy;

// �s���������N���X
class ActionBase
{
public:
	template<typename T>
	ActionBase(T* actor):owner(actor){}
	// ���s���
	enum class State
	{
		Run,		// ���s��
		Failed,		// ���s���s
		Complete,	// ���s����
	};

	// ���s����(�������z�֐�)
	virtual ActionBase::State Run(float elapsedTime) = 0;
protected:
	Enemy* owner;
	int step = 0;
};
