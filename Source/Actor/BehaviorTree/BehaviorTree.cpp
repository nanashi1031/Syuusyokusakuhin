#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "EnemyBlueSlime.h"
#include "BehaviorData.h"


// �f�X�g���N�^
BehaviorTree::~BehaviorTree()
{
	NodeAllClear(root);
}

void BehaviorTree::AddNode(std::string parentName, std::string entryName, int priority, SelectRule selectRule, JudgmentBase* judgment, ActionBase* action)
{
	if (parentName != "")
	{
		NodeBase* parentNode = root->SearchNode(parentName);

		if (parentNode != nullptr)
		{
			NodeBase* sibling = parentNode->GetLastChild();
			NodeBase* addNode = new NodeBase(entryName, parentNode, sibling, priority, selectRule, judgment, action, parentNode->GetHirerchyNo() + 1);

			parentNode->AddChild(addNode);
		}
	}
	else {
		if (root == nullptr)
		{
			root = new NodeBase(entryName, nullptr, nullptr, priority, selectRule, judgment, action, 1);
		}
	}
}

// ���_
NodeBase* BehaviorTree::ActiveNodeInference(BehaviorData* data)
{
	// �f�[�^�����Z�b�g���ĊJ�n
	data->Init();
	return root->Inference(owner, data);
}

// �V�[�P���X�m�[�h����̐��_�J�n
NodeBase* BehaviorTree::SequenceBack(NodeBase* sequenceNode, BehaviorData* data)
{
	return sequenceNode->Inference(owner, data);
}

// �m�[�h���s
NodeBase* BehaviorTree::Run(NodeBase* actionNode, BehaviorData* data,float elapsedTime)
{
	// �m�[�h���s
	ActionBase::State state = actionNode->Run(owner,elapsedTime);

	// ����I��
	if (state == ActionBase::State::Complete)
	{
		// �V�[�P���X�̓r�����𔻒f
		NodeBase* sequenceNode = data->PopSequenceNode();

		// �r������Ȃ��Ȃ�I��
		if (sequenceNode == nullptr)
		{
			return nullptr;
		}
		else {
			// �r���Ȃ炻������n�߂�
			return SequenceBack(sequenceNode, data);
		}
		// ���s�͏I��
	}
	else if (state == ActionBase::State::Failed) {
		return nullptr;
	}

	// ����ێ�
	return actionNode;
}

// �o�^���ꂽ�m�[�h��S�č폜����
void BehaviorTree::NodeAllClear(NodeBase* delNode)
{
	size_t count = delNode->children.size();
	if (count > 0)
	{
		for (NodeBase* node :delNode->children)
		{
			NodeAllClear(node);
		}
		delete delNode;
	}
	else
	{
		delete delNode;
	}
}

