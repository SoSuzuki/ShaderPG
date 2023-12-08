#pragma once
#include "Engine/GameObject.h"
#include <d3d11.h>

struct CBUFF_TESTSCENE 
{
    XMFLOAT4    lightPosition;
    XMFLOAT4    eyePosition;
};


class TestScene :
    public GameObject
{
private:

    ID3D11Buffer* pCBTestScene_;


    void InitConstantBuffer();
    XMFLOAT4 lightSourcePosition_;

public:
    TestScene(GameObject* parent);
    ~TestScene();

    void Initialize()   override;
    void Update()   override;
    void Draw()     override;
    void Release()  override;

    void SetLightPos(XMFLOAT4& _pos) { lightSourcePosition_ = _pos; }
    XMFLOAT4 GetLightPos() { return(lightSourcePosition_); }
};

