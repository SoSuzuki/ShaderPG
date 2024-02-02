#include "Dice.h"
#include "Engine/Model.h"

Dice::Dice(GameObject* parent):hModel_(-1)
{
}

Dice::~Dice()
{
}

void Dice::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Assets/RedDice.fbx");
	assert(hModel_ >= 0);

	transform_.position_ = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//transform_.scale_ = XMFLOAT3(3.0f, 3.0f, 3.0f);
}

void Dice::Update()
{
	transform_.rotate_.y++;
}

void Dice::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Dice::Release()
{
}
