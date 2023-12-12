#pragma once
#include "Engine/GameObject.h"


class TestScene :
    public GameObject
{
private:
public:
    TestScene(GameObject* parent);
    ~TestScene();

    void Initialize()   override;
    void Update()   override;
    void Draw()     override;
    void Release()  override;
};

