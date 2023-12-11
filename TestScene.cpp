#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Controller.h"
#include "Stage.h"
#include "Arrow.h"
#include "Ball.h"


namespace {
	const XMFLOAT4 DEF_LIGHT_POSITION{ 1,2,1,0 };
}


TestScene::TestScene(GameObject* parent)
	:GameObject(parent,"TestScene"),pCBTestScene_(nullptr), lightSourcePosition_(DEF_LIGHT_POSITION)
{
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	//Instantiate<Stage>(this);
	Instantiate<Arrow>(this);
	Instantiate<Ball>(this);
	Instantiate<Controller>(this);

	InitConstantBuffer();
}

void TestScene::Update()
{

	CBUFF_TESTSCENE cb;
	cb.lightPosition = lightSourcePosition_;
	XMStoreFloat4(&cb.eyePosition, Camera::GetEyePos());

	//D3D11_MAPPED_SUBRESOURCE pdata;
	//Direct3D::pContext_->Map(pCBTestScene_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	//memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

	//Direct3D::pContext_->Unmap(pCBTestScene_, 0);	//�ĊJ

	Direct3D::pContext_->UpdateSubresource(pCBTestScene_, 0, NULL, &cb, 0, 0);

	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBTestScene_);	// ���_�V�F�[�_�[�p
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBTestScene_);	// �s�N�Z���V�F�[�_�[�p

}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}

void TestScene::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CBUFF_TESTSCENE);
	cb.Usage = D3D11_USAGE_DEFAULT;				// FBX��draw�ɂ������L�q�������
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;						// ��ɓ�����
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBTestScene_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
	}
}