#include "JudgmentBase.h"
#include "NodeBase.h"
#include "EnemyBlueSlime.h"
#include "BehaviorData.h"
#include "ActionBase.h"



// �f�X�g���N�^
NodeBase::~NodeBase()
{
	delete judgment;
	delete action;
}
// �q�m�[�h�Q�b�^�[
NodeBase* NodeBase::GetChild(int index)
{
	if (children.size() > index)
	{
		return nullptr;
	}
	return children.at(index);
}

// �q�m�[�h�Q�b�^�[(����)
NodeBase* NodeBase::GetLastChild()
{
	if (children.size() == 0)
	{
		return nullptr;
	}

	return children.at(children.size() - 1);
}

// �q�m�[�h�Q�b�^�[(�擪)
NodeBase* NodeBase::GetTopChild()
{
	if (children.size() == 0)
	{
		return nullptr;
	}

	return children.at(0);
}


// �m�[�h����
NodeBase* NodeBase::SearchNode(std::string searchName)
{
	// ���O����v
	if (name == searchName)
	{
		return this;
	}
	else {
		// �q�m�[�h�Ō���
		for (auto itr = children.begin(); itr != children.end(); itr++)
		{
			NodeBase* ret = (*itr)->SearchNode(searchName);

			if (ret != nullptr)
			{
				return ret;
			}
		}
	}

	return nullptr;
}

// �m�[�h���_
NodeBase* NodeBase::Inference(EnemyBlueSlime* enemy, BehaviorData* data)
{
	std::vector<NodeBase*> list;
	NodeBase* result = nullptr;

	// children�̐��������[�v���s���B
	for (int i = 0; i < children.size(); i++)
	{
		// children.at(i)->judgment��nullptr�łȂ����
		if (children.at(i)->judgment != nullptr)
		{
			// TODO 04_03 children.at(i)->judgment->Judgment()�֐������s���Ature�ł����
			// list��children.at(i)��ǉ����Ă���

		}
		else {
			// TODO 04_03 ����N���X���Ȃ���Ζ������ɒǉ�

		}
	}

	// �I�����[���Ńm�[�h����
	switch (selectRule)
	{
		// �D�揇��
	case BehaviorTree::SelectRule::Priority:
		result = SelectPriority(&list);
		break;
		// �����_��
	case BehaviorTree::SelectRule::Random:
		result = SelectRandom(&list);
		break;
		// �V�[�P���X
	case BehaviorTree::SelectRule::Sequence:
	case BehaviorTree::SelectRule::SequentialLooping:
		result = SelectSequence(&list, data);
		break;
	}

	if (result != nullptr)
	{
		// �s��������ΏI��
		if (result->HasAction() == true)
		{
			return result;
		}
		else {
			// ���܂����m�[�h�Ő��_�J�n
			result = result->Inference(enemy, data);
		}
	}

	return result;
}

// �D�揇�ʂŃm�[�h�I��
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
	NodeBase* selectNode = nullptr;
	int priority = INT_MAX;

	// TODO 04_04 ��ԗD�揇�ʂ������m�[�h��T����selectNode�Ɋi�[


	return selectNode;
}


// �����_���Ńm�[�h�I��
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	int selectNo = 0;
	// TODO 04_05 list�̃T�C�Y�ŗ������擾����selectNo�Ɋi�[

	
	// list��selectNo�Ԗڂ̎��Ԃ����^�[��
	return (*list).at(selectNo);
}

// �V�[�P���X�E�V�[�P���V�������[�s���O�Ńm�[�h�I��
NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
	int step = 0;

	// �w�肳��Ă��钆�ԃm�[�h�̂��V�[�P���X���ǂ��܂Ŏ��s���ꂽ���擾����
	step = data->GetSequenceStep(name);

	// ���ԃm�[�h�ɓo�^����Ă���m�[�h���ȏ�̏ꍇ�A
	if (step >= children.size())
	{
		// TODO 04_06 ���[���ɂ���ď�����؂�ւ���
		// ���[����BehaviorTree::SelectRule::SequentialLooping�̂Ƃ��͍ŏ�������s���邽�߁Astep��0����
		// ���[����BehaviorTree::SelectRule::Sequence�̂Ƃ��͎��Ɏ��s�ł���m�[�h���Ȃ����߁Anullptr�����^�[��





	}
	// ���s�\���X�g�ɓo�^����Ă���f�[�^�̐��������[�v���s��
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		// �q�m�[�h�����s�\���X�g�Ɋ܂܂�Ă��邩
		if (children.at(step)->GetName() == (*itr)->GetName())
		{
			// TODO 04_06 ���݂̎��s�m�[�h�̕ۑ��A���Ɏ��s����X�e�b�v�̕ۑ����s������A
			// ���݂̃X�e�b�v�ԍ��̃m�[�h�����^�[�����Ȃ���
			// �@�X�^�b�N�ɂ�data->PushSequenceNode�֐����g�p����B�ۑ�����f�[�^�͎��s���̒��ԃm�[�h�B
			// �A�܂��A���Ɏ��s���钆�ԃm�[�h�ƃX�e�b�v����ۑ�����
			// �@�ۑ��ɂ�data->SetSequenceStep�֐����g�p�B
			// �@�ۑ��f�[�^�͒��ԃm�[�h�̖��O�Ǝ��̃X�e�b�v���ł�(step + 1)
			// �B�X�e�b�v�ԍ��ڂ̎q�m�[�h�����s�m�[�h�Ƃ��ă��^�[������



		}
	}
	// �w�肳�ꂽ���ԃm�[�h�Ɏ��s�\�m�[�h���Ȃ��̂�nullptr�����^�[������
	return nullptr;
}

// ����
bool NodeBase::Judgment(EnemyBlueSlime* enemy)
{
	// TODO 04_07 judgment�����邩���f�B����΃����o�֐�Judgment()���s�������ʂ����^�[���B
	if (judgment != nullptr)
	{

	}
	return true;
}

// �m�[�h���s
ActionBase::State NodeBase::Run(EnemyBlueSlime* enemy,float elapsedTime)
{
	// TODO 04_08 action�����邩���f�B����΃����o�֐�Run()���s�������ʂ����^�[���B
	if (action != nullptr)
	{


	}

	return ActionBase::State::Failed;
}
