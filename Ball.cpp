#include "Ball.h"
#include "Engine/Model.h"

Ball::Ball(GameObject* parent):hModel_(-1)
{
}

Ball::~Ball()
{
}

void Ball::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Assets/Balloon.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = XMFLOAT3(7.0f, 1.0f, 7.0f);
}

void Ball::Update()
{
}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::Release()
{
}
