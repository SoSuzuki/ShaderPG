#pragma once
#include "Engine/GameObject.h"
#include <d3d11.h>

struct CBUFF_STAGE
{
    XMFLOAT4    lightPosition;
    XMFLOAT4    eyePosition;
};

// 光源の位置を管理、操作(可視化など)するクラス
class Stage :
    public GameObject
{
private:
    int hModel_;    //モデル番号
    ID3D11Buffer* pCBStage_;
    XMFLOAT4 lightSourcePosition_;

    void InitConstantBuffer();


public:
    //コンストラクタ
    Stage(GameObject* parent);
    //デストラクタ
    ~Stage();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetLightPos(XMFLOAT4& _pos) { lightSourcePosition_ = _pos; }
    XMFLOAT4 GetLightPos() { return(lightSourcePosition_); }
};

