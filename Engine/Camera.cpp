#include "Camera.h"

namespace Camera 
{
	//変数
	XMVECTOR position_;		//カメラの位置（視点）
	XMVECTOR target_;		//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
}
//初期化
void Camera::Initialize()
{
	//position_ = { 0, 3, -10, 0 };	//カメラの位置
	//target_ = { 0, 0, 0, 0 };	//カメラの焦点
	position_ = { 0.0f, 4.0f, -12.0f, 0.0f };	//カメラの位置
	target_ = { 0.0f, 0.0f, -0.5f, 0.0f };	//カメラの焦点
	//プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800 / (FLOAT)600, 0.1f, 100.0f);
}

//更新
void Camera::Update()
{
	//ビュー行列
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
}

//位置を設定
void Camera::SetPosition(XMVECTOR position)
{
	position_ = position;
}

void Camera::SetPosition(XMFLOAT3 position)
{
	//オーバーロードの大元を呼び出すことで、仕様変更されても対応可に
	SetPosition(XMLoadFloat3(&position));
}

//焦点を設定
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}

void Camera::SetTarget(XMFLOAT3 target)
{
	SetTarget(XMLoadFloat3(&target));
}

XMFLOAT4 Camera::GetEyePos()
{
	XMFLOAT4 eyePos;
	XMStoreFloat4(&eyePos, position_);
	return eyePos;
}

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}