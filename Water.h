#pragma once
#include "Engine/GameObject.h"
class Water :
    public GameObject
{
private:
    int hModel_;
public:
    Water(GameObject* parent);
    ~Water();

    void Initialize()   override;
    void Update()   override;
    void Draw()     override;
    void Release()  override;

};

