#pragma once
#include <vector>
class Character;

class State
{
public:
	template<class T>
	State(T* t) :owner(t) {}
	virtual ~State() {}

	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsedTime) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void Exit() = 0;
protected:
	Character* owner;
};