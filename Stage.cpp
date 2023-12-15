#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

namespace {
	const XMFLOAT4 DEF_LIGHT_POSITION{ 1,2,-1,0 };
}

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Player"), hModel_(-1),	pCBStage_(nullptr), lightSourcePosition_(DEF_LIGHT_POSITION)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Assets/Balloon.fbx");
	assert(hModel_ >= 0);
	transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
	XMFLOAT4 lightPos = GetLightPos();
	transform_.position_ = XMFLOAT3(lightPos.x, lightPos.y, lightPos.z);
	InitConstantBuffer();
}

void Stage::Update()
{
	// �����̈ʒu�𓮂����A���f���ňʒu������
	if (Input::IsKey(DIK_RIGHT)) {
		XMFLOAT4 p = GetLightPos();
		XMFLOAT4 margin{ p.x + 0.1f,p.y + 0.0f,p.z + 0.0f,p.w + 0.0f };
		SetLightPos(margin);
		transform_.position_ = XMFLOAT3(margin.x, margin.y, margin.z);
	}
	if (Input::IsKey(DIK_LEFT)) {
		XMFLOAT4 p = GetLightPos();
		XMFLOAT4 margin{ p.x - 0.1f,p.y - 0.0f,p.z - 0.0f,p.w - 0.0f };
		SetLightPos(margin);
		transform_.position_ = XMFLOAT3(margin.x, margin.y, margin.z);
	}
	if (Input::IsKey(DIK_UP)) {
		XMFLOAT4 p = GetLightPos();
		XMFLOAT4 margin{ p.x + 0.0f,p.y + 0.0f,p.z + 0.1f,p.w + 0.0f };
		SetLightPos(margin);
		transform_.position_ = XMFLOAT3(margin.x, margin.y, margin.z);
	}
	if (Input::IsKey(DIK_DOWN)) {
		XMFLOAT4 p = GetLightPos();
		XMFLOAT4 margin{ p.x - 0.0f,p.y - 0.0f,p.z - 0.1f,p.w - 0.0f };
		SetLightPos(margin);
		transform_.position_ = XMFLOAT3(margin.x, margin.y, margin.z);
	}

	// ���~
	if (Input::IsKey(DIK_SPACE)) {
		XMFLOAT4 p = GetLightPos();
		XMFLOAT4 margin{ p.x + 0.0f,p.y + 0.1f,p.z + 0.0f,p.w + 0.0f };
		SetLightPos(margin);
		if (margin.y > 10) {
			margin.y = 10.0f;
		}
		transform_.position_ = XMFLOAT3(margin.x, margin.y, margin.z);
	}
	if (Input::IsKey(DIK_LCONTROL)) {
		XMFLOAT4 p = GetLightPos();
		XMFLOAT4 margin{ p.x - 0.0f,p.y - 0.1f,p.z - 0.0f,p.w - 0.0f };
		SetLightPos(margin);
		if (margin.y < -10) {
			margin.y = -10.0f;
		}
		transform_.position_ = XMFLOAT3(margin.x, margin.y, margin.z);
	}

	CBUFF_STAGE cb;
	cb.lightPosition = lightSourcePosition_;
	XMStoreFloat4(&cb.eyePosition, Camera::GetEyePos());

	//D3D11_MAPPED_SUBRESOURCE pdata;
	//Direct3D::pContext_->Map(pCBTestScene_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	//memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

	//Direct3D::pContext_->Unmap(pCBTestScene_, 0);	//�ĊJ

	Direct3D::pContext_->UpdateSubresource(pCBStage_, 0, NULL, &cb, 0, 0);

	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStage_);	// ���_�V�F�[�_�[�p
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStage_);	// �s�N�Z���V�F�[�_�[�p
}

void Stage::Draw()
{
	Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Stage::Release()
{
}

void Stage::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CBUFF_STAGE);
	cb.Usage = D3D11_USAGE_DEFAULT;				// FBX��draw�ɂ������L�q�������
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;						// ��ɓ�����
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStage_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
	}
}