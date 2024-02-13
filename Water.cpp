#include "Water.h"
#include "Engine/Model.h"

Water::Water(GameObject* parent):hModel_(-1)
{
}

Water::~Water()
{
}

void Water::Initialize()
{
	hModel_ = Model::Load("Assets/Water.fbx");
	assert(hModel_ >= 0);
	transform_.scale_ = XMFLOAT3(3.0f, 1, 3.0f);
}

void Water::Update()
{
}

void Water::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Water::Release()
{
}
