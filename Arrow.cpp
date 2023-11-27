#include "Arrow.h"
#include "Engine/Model.h"

Arrow::Arrow(GameObject* parent):hModel_(-1)
{
}

Arrow::~Arrow()
{
}

void Arrow::Initialize()
{
	hModel_ = Model::Load("Assets/ArrowTest.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = XMFLOAT3(7.0f, 1.0f, 7.0f);
}

void Arrow::Update()
{
}

void Arrow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Arrow::Release()
{
}
