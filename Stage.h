#pragma once
#include "Engine/GameObject.h"
#include <d3d11.h>

struct CBUFF_STAGE
{
    XMFLOAT4    lightPosition;
    XMFLOAT4    eyePosition;
};

// �����̈ʒu���Ǘ��A����(�����Ȃ�)����N���X
class Stage :
    public GameObject
{
private:
    int hModel_;    //���f���ԍ�
    ID3D11Buffer* pCBStage_;
    XMFLOAT4 lightSourcePosition_;

    void InitConstantBuffer();


public:
    //�R���X�g���N�^
    Stage(GameObject* parent);
    //�f�X�g���N�^
    ~Stage();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetLightPos(XMFLOAT4& _pos) { lightSourcePosition_ = _pos; }
    XMFLOAT4 GetLightPos() { return(lightSourcePosition_); }
};

