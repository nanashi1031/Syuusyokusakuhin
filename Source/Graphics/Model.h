#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// モデル
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

	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	void UpdateAnimation(float elapsedTime);

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

	bool IsPlayAnimation() const;

	Node* FindNode(const char* name);

	// ゲッター　セッター
	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }

	const ModelResource* GetResource() const { return resource.get(); }

	void RootNode(const char* nodeName, float nodeRadius);

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	int currentAnimationIndex = -1;
	float currentAnimationSeconds = 0.0f;

	// アニメーション関連
	bool animationLoopFlag = false;
	bool animationEndFlag = false;
	float animationBlendTime = 0.0f;
	float animationBlendSeconds = 0.0f;
	float	animationPlaySpeed = 1.0f;
	float	animationSeconds = 0.0f;
	float	animationSpeed = 0.0f;
};
