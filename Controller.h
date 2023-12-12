#pragma once
#include "Engine/GameObject.h"
#include "Engine/Camera.h"

class Controller :
    public GameObject
{
private:
    bool isDegOver;
public:
    Controller(GameObject* parent);
    ~Controller();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
