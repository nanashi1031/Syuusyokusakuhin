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
	void SetState(int setstate);
	// �X�e�[�g�ύX
	void ChangeState(int newState);
	// TODO 03_04 ��P�K�w�̃X�e�[�g�o�^�ɂ���������̌^��ύX
	// �X�e�[�g�o�^
	void RegisterState(HierarchicalState* state);
	// ���݂̃X�e�[�g�ԍ��擾
	int GetStateIndex();
	// TODO 03_04 �V�K�ő�Q�K�w�̃X�e�[�g�֘A��o�^
	// �Q�w�ڃX�e�[�g�ύX
	void ChangeSubState(int newState);
	// �Q�w�ڃX�e�[�g�o�^
	void RegisterSubState(int index, State* subState);
	// �X�e�[�g�擾
	HierarchicalState* GetState() { return currentState; }
	// TODO 03_04 ��P�K�w�̃X�e�[�g�o�^�ɂ�����^��ύX

private:
	// TODO 03_04 ��P�K�w�̃X�e�[�g�o�^�ɂ�����^��ύX
	// ���݂̃X�e�[�g
	HierarchicalState* currentState = nullptr;
	// TODO 03_04 �X�e�[�g��ێ�����vector�̕ۑ��^��ύX
	// �e�X�e�[�g��ێ�����z��
	std::vector<HierarchicalState*> statePool;
};