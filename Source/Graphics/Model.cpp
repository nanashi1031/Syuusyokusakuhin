#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include "Mathf.h"

Model::Model(const char* filename)
{
	// リソース読み込み
	resource = std::make_shared<ModelResource>();
	resource->Load(Graphics::Instance().GetDevice(), filename);

	// ノード
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

void Model::UpdateAnimation(float elapsedTime)
{
	if (!IsPlayAnimation()) return;

	// ブレンド率の計算
	float blendRate = 1.0f;
	blendRate = currentAnimationSeconds / animationBlendSeconds;

	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			// 再生時間とキーフレームの時間から補間率を算出する
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe0.seconds - keyframe1.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// ２つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				// ブレンド補完処理
				if (blendRate < 1.0f)
				{
					// 現在の姿勢と次のキーフレームとの姿勢の補完
					DirectX::XMStoreFloat3(&node.translate, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&node.translate), DirectX::XMLoadFloat3(&key1.translate), blendRate));
					DirectX::XMStoreFloat3(&node.scale, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&node.scale), DirectX::XMLoadFloat3(&key1.scale), blendRate));
					DirectX::XMStoreFloat4(&node.rotate, DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&node.rotate), DirectX::XMLoadFloat4(&key1.rotate), blendRate));
				}
				// 通常の計算
				else
				{
					// 前のキーフレームと次のキーフレームの姿勢を補完
					DirectX::XMStoreFloat3(&node.translate, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&key0.translate), DirectX::XMLoadFloat3(&key1.translate), rate));
					DirectX::XMStoreFloat3(&node.scale, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&key0.scale), DirectX::XMLoadFloat3(&key1.scale), rate));
					DirectX::XMStoreFloat4(&node.rotate, DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&key0.rotate), DirectX::XMLoadFloat4(&key1.rotate), rate));
				}
			}
			break;
		}
	}

	// 最終フレーム処理
	if (animationEndFlag)
	{
		animationEndFlag = false;
		// アニメーションデータをの削除
		currentAnimationIndex = -1;
		return;
	}

	// 時間経過
	if (animationSeconds <= 0)
		currentAnimationSeconds += (elapsedTime * animationPlaySpeed);
	else
	{
		if (currentAnimationSeconds < animationSeconds)
			currentAnimationSeconds += (elapsedTime * animationPlaySpeed);
		else if (currentAnimationSeconds >= animationSeconds)
			currentAnimationSeconds += (elapsedTime * animationSpeed);
	}

	// 再生時間が終端時間を超えてループフラグがtrueなら
	if (currentAnimationSeconds >= animation.secondsLength && animationLoopFlag)
	{
		// 再生時間を巻き戻す
		currentAnimationSeconds -= animation.secondsLength;
	}
	else if (currentAnimationSeconds >= animation.secondsLength)
	{
		animationEndFlag = true;
	}
}

bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0) return false;
	if (currentAnimationIndex >= resource->GetAnimations().size()) return false;
	return true;
}

Model::Node* Model::FindNode(const char* name)
{
	for (Node& node : nodes)
	{
		// 比べた名前が同じなら
		if (::strcmp(node.name, name) == 0)
		{
			return &node;
		}
	}

	return nullptr;
}

int Model::FindNodeIndex(const char* name)
{
	int index = 0;
	for (Node& node : nodes)
	{
		// 比べた名前が同じなら
		if (::strcmp(node.name, name) == 0)
		{
			break;
		}
		index++;
	}

	return index;
}

DirectX::XMFLOAT3 Model::RootMotion(const char* nodeName)
{
	if (!IsPlayAnimation()) return {0, 0, 0};

	Model::Node* node = FindNode(nodeName);
	if (!node) return { 0, 0, 0 };
	int nodeIndex = FindNodeIndex(nodeName);

	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());

	//for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	//{
	//	// 現在のキーフレーム探索
	//	const ModelResource::Keyframe& keyframe = keyframes.at(keyIndex);
	//	const ModelResource::Keyframe& keyframeNext = keyframes.at(keyIndex + 1);
	//	if (currentAnimationSeconds >= keyframe.seconds &&
	//		currentAnimationSeconds < keyframeNext.seconds)
	//	{
	//		const ModelResource::NodeKeyData& nodekeyData = keyframe.nodeKeys.at(keyIndex);
	//		const ModelResource::NodeKeyData& nodekeyDataNext = keyframe.nodeKeys.at(keyIndex + 1);

	//		DirectX::XMFLOAT3 movement = Mathf::SubtractFloat3(
	//			nodekeyDataNext.translate, nodekeyData.translate);
	//		return movement;
	//	}
	//}
	node->translate = {0, node->translate.y, 0};

	return { 0, 0, 0 };
}