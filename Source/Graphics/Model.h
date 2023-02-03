#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "ModelResource.h"

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

	// アニメーションが動いているか
	bool IsPlayAnimation();

	// ノードを探す
	Node* FindNode(const char* name);

	// ルートモーションセットアップ
	void SetupRootMotion(const char* rootMotionNodeName);

	// ノードの番号を探す
	int FindNodeIndex(const char* name);

	// ルートモーション
	DirectX::XMFLOAT3 RootMotion(const char* nodeName);

	// ゲッター　セッター
	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }

	const ModelResource* GetResource() const { return resource.get(); }

	float GetAnimationSeconds() { return this->currentAnimationSeconds; }

private:
	// ルートモーション計算
	void ComputeRootMotion();

private:
	std::shared_ptr<ModelResource> resource;
	std::vector<Node> nodes;

	// 現在のアニメーション番号
	int currentAnimationIndex = -1;
	// 現在のアニメーション秒数
	float currentAnimationSeconds = 0.0f;

	// アニメーション関連
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
