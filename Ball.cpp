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
	hModel_ = Model::Load("Assets/Ball.fbx");
	assert(hModel_ >= 0);
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
