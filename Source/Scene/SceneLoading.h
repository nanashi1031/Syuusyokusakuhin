#pragma once

#include "Sprite.h"
#include "Scene.h"

//���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
    SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
    ~SceneLoading() override {};

    //������
    void Initialize()override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render() override;

private:
    //���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);

private:
    std::unique_ptr<Sprite>	sprite;

    std::unique_ptr<Sprite> spr_loading = nullptr;

    float   angle = 0.0f;
    Scene* nextScene = nullptr;
};