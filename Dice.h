#pragma once
#include "Engine/GameObject.h"
class Dice :
    public GameObject
{
private:
    int hModel_;
public:
    Dice(GameObject* parent);
    ~Dice();

    void Initialize()   override;
    void Update()   override;
    void Draw()     override;
    void Release()  override;

};