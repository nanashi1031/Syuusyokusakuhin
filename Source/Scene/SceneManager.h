#pragma once

#include "Scene.h"

enum class SceneGameState
{
    Game,
    Pause,
    GameOver,
};

//シーンマネージャー
class SceneManager
{
private:
    SceneManager() {}
    ~SceneManager() {}

public:
    //唯一のインスタンス取得
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render();

    //シーンクリア
    void Clear();

    //シーン切り替え
    void ChangeScene(Scene* scene);

    // 現在のシーンを取得
    Scene* GetScene() { return currentScene; }

    SceneGameState GetSceneGameState() { return sceneGameState; }
    template<typename T>
    void SetSceneGameState(T state)
    {
        this->sceneGameState = static_cast<SceneGameState>(state);
    }

private:
    Scene* currentScene = nullptr;
    SceneGameState sceneGameState = SceneGameState::Game;
};