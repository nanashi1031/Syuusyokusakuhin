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
	void ChangeState(T newState)
	{
		if (currentState != nullptr)
		{
			//���݂̃X�e�[�g��Exit�֐������s
			currentState->Exit();

			// ���ݎ��s����Ă���T�u�X�e�[�g��Exit�֐������s
			currentState->GetSubState()->Exit();

			//�V�����X�e�[�g���Z�b�g
			SetState(newState);

			//�V�����X�e�[�g��Enter�֐����Ăяo���B
			currentState->Enter();
		}
	}

	// �X�e�[�g�o�^
	void RegisterState(HierarchicalState* state);

	// ���݂̃X�e�[�g�ԍ��擾
	int GetStateIndex();

	// �Q�w�ڃX�e�[�g�ύX
	template<typename T>
	void ChangeSubState(T newState)
	{
		int state = static_cast<int>(newState);
		currentState->ChangeSubState(state);
	}

	// �Q�w�ڃX�e�[�g�o�^
	template<typename T>
	void RegisterSubState(T tIndex, State* subState)
	{
		int index = static_cast<int>(tIndex);
		statePool.at(index)->RegisterSubState(subState);
	}

	// �X�e�[�g�擾
	HierarchicalState* GetState() { return currentState; }

private:
	// ���݂̃X�e�[�g
	HierarchicalState* currentState = nullptr;

	// �e�X�e�[�g��ێ�����z��
	std::vector<HierarchicalState*> statePool;
};