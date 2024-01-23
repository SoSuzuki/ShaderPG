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
	hModel_ = Model::Load("Assets/RedDice.fbx");
	//hModel_ = Model::Load("Assets/BrickDonutSpecular.fbx"); 
	assert(hModel_ >= 0);
	transform_.scale_ = XMFLOAT3(3.0f, 3.0f, 3.0f);
	//transform_.position_ = XMFLOAT3(3.0f, 1.0f, -1.0f);
}

void Ball::Update()
{
	transform_.rotate_.y ++;
}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::Release()
{
}
