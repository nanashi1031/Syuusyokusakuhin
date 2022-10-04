#pragma once
#include <string>

class ActionBase;
class JudgmentBase;
class NodeBase;
class BehaviorData;
class EnemyBlueSlime;

// �r�w�C�r�A�c���[
class BehaviorTree
{
public:
	// �I�����[��
	enum class SelectRule
	{
		Non,				// �������[�m�[�h�p
		Priority,			// �D�揇��
		Sequence,			// �V�[�P���X
		SequentialLooping,	// �V�[�P���V�������[�s���O
		Random,				// �����_��
	};

public:
	BehaviorTree() :root(nullptr) , owner(nullptr) {}
	BehaviorTree(EnemyBlueSlime* enemy) :root(nullptr) , owner(enemy){}
	~BehaviorTree();

	// ���s�m�[�h�𐄘_����
	NodeBase* ActiveNodeInference(BehaviorData* data);

	// �V�[�P���X�m�[�h���琄�_�J�n
	NodeBase* SequenceBack(NodeBase* sequenceNode, BehaviorData* data);

	// �m�[�h�ǉ�
	void AddNode(std::string parentName, std::string entryName, int priority, SelectRule selectRule, JudgmentBase* judgment, ActionBase* action);

	// ���s
	NodeBase* Run(NodeBase* actionNode, BehaviorData* data,float elapsedTime);
private:
	// �m�[�h�S�폜
	void NodeAllClear(NodeBase* delNode);
private:
	// ���[�g�m�[�h
	NodeBase* root;
	EnemyBlueSlime* owner;
};
