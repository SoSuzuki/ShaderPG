#include "Controller.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"


Controller::Controller(GameObject* parent)
	:GameObject(parent, "Controller"), isDegOver(false)
{
	//transform_.position_ = XMFLOAT3(7.0f, 0.0f, 7.0f);
	transform_.rotate_.x = 45.0f;
}

Controller::~Controller()
{
}

void Controller::Initialize()
{

}

void Controller::Update()
{
	//if (Input::IsKey(DIK_LEFT)) {
	//	transform_.rotate_.y -= 1.0f;
	//}
	//if (Input::IsKey(DIK_RIGHT)) {
	//	transform_.rotate_.y += 1.0f;
	//}

	//if (Input::IsKey(DIK_UP)) {
	//	transform_.rotate_.x += 1.0f;
	//	if (transform_.rotate_.x > 89)
	//		transform_.rotate_.x = 89;
	//}
	//if (Input::IsKey(DIK_DOWN)) {
	//	transform_.rotate_.x -= 1.0f;
	//	if (transform_.rotate_.x <= 0)
	//		transform_.rotate_.x = 0;
	//}

	if (Input::IsKey(DIK_SPACE)) {
		transform_.position_.y += 0.05f;
		if (transform_.position_.y > 30)
			transform_.position_.y = 30.0f;
	}

	if (Input::IsKey(DIK_LCONTROL)) {
		transform_.position_.y -= 0.05f;
		if (transform_.position_.y <= 0)
			transform_.position_.y = 0;
	}

	if (Input::IsKeyDown(DIK_R)) {
		transform_.position_ = XMFLOAT3(7.0f, 0.0f, 7.0f);
		transform_.rotate_ = XMFLOAT3(45.0f, 0, 0);
	}

	XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	XMMATRIX mRotZ = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.z));
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	
	XMVECTOR vMoveZ = { 0.0f, 0.0f, 0.1f, 0.0f };
	vMoveZ = XMVector3TransformCoord(vMoveZ, mRotY);

	XMVECTOR vMoveX = { 0.1f, 0.0f, 0.0f, 0.0f };
	vMoveX = XMVector3TransformCoord(vMoveX, mRotY);

	if (Input::IsKey(DIK_W)) {
		vPos += vMoveZ;
	}
	if (Input::IsKey(DIK_S)) {
		vPos -= vMoveZ;
	}
	if (Input::IsKey(DIK_A)) {
		vPos -= vMoveX;
	}
	if (Input::IsKey(DIK_D)) {
		vPos += vMoveX;
	}
	XMStoreFloat3(&transform_.position_, vPos);

	XMVECTOR vCam = { 0, 0, -10, 0 };
	vCam = XMVector3TransformCoord(vCam, mRotX * mRotY * mRotZ);

	Camera::SetPosition(vPos + vCam);       
	Camera::SetTarget(transform_.position_);
}

void Controller::Draw()
{
}

void Controller::Release()
{
}