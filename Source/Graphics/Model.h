#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "ModelResource.h"

// ���f��
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	void UpdateAnimation(float elapsedTime);

	void UpdateRootMotion(DirectX::XMFLOAT3& transform);

	template<typename T>
	void PlayAnimation(T tIndex, bool loop, float blendSeconds = 0.2f, float playSpeed = 1.0f, float seconds = 0.0f, float speed = 1.0f)
	{
		int index = static_cast<int>(tIndex);
		currentAnimationIndex = index;
		currentAnimationSeconds = 0.0f;
		animationLoopFlag = loop;
		animationEndFlag = false;
		animationBlendTime = 0.0f;
		animationBlendSeconds = blendSeconds;
		animationPlaySpeed = playSpeed;
		animationSeconds = seconds;
		animationSpeed = speed;
	}

	// �A�j���[�V�����������Ă��邩
	bool IsPlayAnimation();

	// �m�[�h��T��
	Node* FindNode(const char* name);

	// ���[�g���[�V�����Z�b�g�A�b�v
	void SetupRootMotion(const char* rootMotionNodeName);

	// �m�[�h�̔ԍ���T��
	int FindNodeIndex(const char* name);

	// ���[�g���[�V����
	DirectX::XMFLOAT3 RootMotion(const char* nodeName);

	// �Q�b�^�[�@�Z�b�^�[
	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }

	const ModelResource* GetResource() const { return resource.get(); }

	float GetAnimationSeconds() { return this->currentAnimationSeconds; }

private:
	// ���[�g���[�V�����v�Z
	void ComputeRootMotion();

private:
	std::shared_ptr<ModelResource> resource;
	std::vector<Node> nodes;

	// ���݂̃A�j���[�V�����ԍ�
	int currentAnimationIndex = -1;
	// ���݂̃A�j���[�V�����b��
	float currentAnimationSeconds = 0.0f;

	// �A�j���[�V�����֘A
	bool animationLoopFlag = false;
	bool animationEndFlag = false;
	float animationBlendTime = 0.0f;
	float animationBlendSeconds = 0.0f;
	float animationPlaySpeed = 1.0f;
	float animationSeconds = 0.0f;
	float animationSpeed = 0.0f;

	int rootMotionNodeIndex = -1;
	DirectX::XMFLOAT3 cacheRootMotionTranslation = { 0, 0, 0 };
	DirectX::XMFLOAT3 rootMotionTranslation = { 0, 0, 0 };
	bool rootMotionFlag = false;
};
