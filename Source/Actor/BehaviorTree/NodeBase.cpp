#include "JudgmentBase.h"
#include "NodeBase.h"

#include "BehaviorData.h"
#include "ActionBase.h"



// デストラクタ
NodeBase::~NodeBase()
{
	delete judgment;
	delete action;
}
// 子ノードゲッター
NodeBase* NodeBase::GetChild(int index)
{
	if (children.size() > index)
	{
		return nullptr;
	}
	return children.at(index);
}

// 子ノードゲッター(末尾)
NodeBase* NodeBase::GetLastChild()
{
	if (children.size() == 0)
	{
		return nullptr;
	}

	return children.at(children.size() - 1);
}

// 子ノードゲッター(先頭)
NodeBase* NodeBase::GetTopChild()
{
	if (children.size() == 0)
	{
		return nullptr;
	}

	return children.at(0);
}


// ノード検索
NodeBase* NodeBase::SearchNode(std::string searchName)
{
	// 名前が一致
	if (name == searchName)
	{
		return this;
	}
	else {
		// 子ノードで検索
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

// 実行可否判定
bool NodeBase::Judgment()
{
	// judgmentがあるか判断。あればメンバ関数Judgment()実行した結果をリターン。
	if (judgment != nullptr)
	{
		return judgment->Judgment();
	}
	return true;
}

// 優先順位でノード選択
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
	NodeBase* selectNode = nullptr;
	int priority = INT_MAX;

	// 一番優先順位が高いノードを探してselectNodeに格納
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

// ランダムでノード選択
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	int selectNo = 0;
	// listのサイズで乱数を取得してselectNoに格納
	selectNo = rand() % list->size();

	// listのselectNo番目の実態をリターン
	return (*list).at(selectNo);
}

// シーケンス・シーケンシャルルーピングでノード選択
NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
	int step = 0;

	// 指定されている中間ノードのがシーケンスがどこまで実行されたか取得する
	step = data->GetSequenceStep(name);

	// 中間ノードに登録されているノード数以上の場合、
	if (step >= children.size())
	{
		// ルールによって処理を切り替える
		// ルールがBehaviorTree::SelectRule::SequentialLoopingのときは最初から実行するため、stepに0を代入
		// ルールがBehaviorTree::SelectRule::Sequenceのときは次に実行できるノードがないため、nullptrをリターン
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
	// 実行可能リストに登録されているデータの数だけループを行う
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		// 子ノードが実行可能リストに含まれているか
		if (children.at(step)->GetName() == (*itr)->GetName())
		{
			// TODO 04_06 現在の実行ノードの保存、次に実行するステップの保存を行った後、
			// 現在のステップ番号のノードをリターンしなさい
			// ①スタックにはdata->PushSequenceNode関数を使用する。保存するデータは実行中の中間ノード。
			// ②また、次に実行する中間ノードとステップ数を保存する
			// 　保存にはdata->SetSequenceStep関数を使用。
			// 　保存データは中間ノードの名前と次のステップ数です(step + 1)
			// ③ステップ番号目の子ノードを実行ノードとしてリターンする
			data->PushSequenceNode(this);
			data->SetSequenceStep(this->GetName(), step + 1);

			return children.at(step);
		}
	}
	// 指定された中間ノードに実行可能ノードがないのでnullptrをリターンする
	return nullptr;
}

ActionBase::State NodeBase::Run(float elapsedTime)
{
	// actionがあるか判断。あればメンバ関数Run()実行した結果をリターン。
	if (action != nullptr)
	{
		return action->Run(elapsedTime);
	}

	return ActionBase::State::Failed;
}