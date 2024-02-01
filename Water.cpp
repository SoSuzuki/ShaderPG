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
	hModel_ = Model::Load("Assets/WaterSurface.fbx");
	assert(hModel_ >= 0);

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
