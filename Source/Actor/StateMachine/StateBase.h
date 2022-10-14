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

class HierarchicalState : public State
{
public :
	template<class T>
	HierarchicalState(T* t) : State((t)) {}
	virtual ~HierarchicalState() {}

	//�X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	//�X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsedTime) = 0;
	//�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void Exit() = 0;
	//�T�u�X�e�[�g�o�^
	virtual void SetSubState(int newState);
	//�T�u�X�e�[�g�ύX
	virtual void ChangeSubState(int newState);

	virtual void RegisterSubState(State* state);

	virtual State* GetSubState() { return subState; }

	virtual int GetSubStateIndex();

protected:
	std::vector<State*> subStatePool;
	State* subState = nullptr;
};