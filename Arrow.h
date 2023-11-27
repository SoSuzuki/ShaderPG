#pragma once
#include "Engine/GameObject.h"
class Arrow :
    public GameObject
{
private:
    int hModel_;

public:
    Arrow(GameObject* parent);
    ~Arrow();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

