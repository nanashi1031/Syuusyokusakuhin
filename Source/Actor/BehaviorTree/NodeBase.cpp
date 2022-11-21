#include "JudgmentBase.h"
#include "NodeBase.h"

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

// ���s�۔���
bool NodeBase::Judgment()
{
	// judgment�����邩���f�B����΃����o�֐�Judgment()���s�������ʂ����^�[���B
	if (judgment != nullptr)
	{
		return judgment->Judgment();
	}
	return true;
}

// �D�揇�ʂŃm�[�h�I��
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
	NodeBase* selectNode = nullptr;
	int priority = INT_MAX;

	// ��ԗD�揇�ʂ������m�[�h��T����selectNode�Ɋi�[
	for (int i = 0; i < list->size(); i++)
	{
		if (priority > list->at(i)->GetPriority())
		{
			priority = list->at(i)->GetPriority();
			selectNode = list->at(i);
		}
	}

	return selectNode;
}

// �����_���Ńm�[�h�I��
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	int selectNo = 0;
	// list�̃T�C�Y�ŗ������擾����selectNo�Ɋi�[
	selectNo = rand() % list->size();

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
		// ���[���ɂ���ď�����؂�ւ���
		// ���[����BehaviorTree::SelectRule::SequentialLooping�̂Ƃ��͍ŏ�������s���邽�߁Astep��0����
		// ���[����BehaviorTree::SelectRule::Sequence�̂Ƃ��͎��Ɏ��s�ł���m�[�h���Ȃ����߁Anullptr�����^�[��
		switch (selectRule)
		{
		case BehaviorTree::SelectRule::SequentialLooping:
			step = 0;
			break;
		case BehaviorTree::SelectRule::Sequence:
			return nullptr;
			break;
		}
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
			data->PushSequenceNode(this);
			data->SetSequenceStep(this->GetName(), step + 1);

			return children.at(step);
		}
	}
	// �w�肳�ꂽ���ԃm�[�h�Ɏ��s�\�m�[�h���Ȃ��̂�nullptr�����^�[������
	return nullptr;
}

ActionBase::State NodeBase::Run(float elapsedTime)
{
	// action�����邩���f�B����΃����o�֐�Run()���s�������ʂ����^�[���B
	if (action != nullptr)
	{
		return action->Run(elapsedTime);
	}

	return ActionBase::State::Failed;
}