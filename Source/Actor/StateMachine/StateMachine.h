#pragma once
#include <vector>
#include "StateBase.h"

class StateMachine
{
public:
	// �R���X�g���N�^
	StateMachine() {}
	// �f�X�g���N�^
	~StateMachine();
	// �X�V����
	void Update(float elapsedTime);

	// �X�e�[�g�Z�b�g
	template<typename T>
	void SetState(T tSetstate)
	{
		int setState = static_cast<int>(tSetstate);
		currentState = statePool.at(setState);
		currentState->Enter();
	}

	// �X�e�[�g�ύX
	template<typename T>
	void ChangeState(T tNewState)
	{
		if (currentState != nullptr)
		{
			//���݂̃X�e�[�g��Exit�֐������s
			currentState->Exit();

			//�V�����X�e�[�g���Z�b�g
			SetState(tNewState);

			//�V�����X�e�[�g��Enter�֐����Ăяo���B
			currentState->Enter();
		}
	}

	// �X�e�[�g�o�^
	void RegisterState(State* state);

	// ���݂̃X�e�[�g�ԍ��擾
	int GetStateIndex();

private:
	// ���݂̃X�e�[�g
	State* currentState = nullptr;
	// �e�X�e�[�g��ێ�����z��
	std::vector<State*> statePool;
};