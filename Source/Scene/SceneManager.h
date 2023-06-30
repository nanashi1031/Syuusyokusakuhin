#pragma once

#include "Scene.h"

enum class SceneGameState
{
    Game,
    Pause,
    GameOver,
};

//�V�[���}�l�[�W���[
class SceneManager
{
private:
    SceneManager() {}
    ~SceneManager() {}

public:
    //�B��̃C���X�^���X�擾
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render();

    //�V�[���N���A
    void Clear();

    //�V�[���؂�ւ�
    void ChangeScene(Scene* scene);

    // ���݂̃V�[�����擾
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