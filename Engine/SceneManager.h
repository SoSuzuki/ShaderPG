#pragma once
#include "GameObject.h"

// �Q�[���̃V�[�����
enum SCENE_ID
{
    SCENE_ID_TEST = 0,
};

class SceneManager :
    public GameObject
{
private:
    SCENE_ID currentSceneID_;
    SCENE_ID nextSceneID_;

public:
    SceneManager(GameObject* parent);
    ~SceneManager();

    void Initialize()   override;
    void Update()   override;
    void Draw()     override;
    void Release()  override;

    /// <summary>
    /// ���̃V�[����_next�ɐ؂�ւ���
    /// </summary>
    /// <param name="_next">NextSceneID</param>
    void ChangeScene(SCENE_ID _next);

};

