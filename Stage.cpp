#include "Stage.h"
#include "Engine/Model.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Player"), hModel_(-1),xSize_(15),zSize_(15)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Assets/BoxDefault.fbx");
	assert(hModel_ >= 0);
	transform_.scale_ = XMFLOAT3(10.0f, 0.1f, 10.0f);
}

void Stage::Update()
{
	//transform_.rotate_.y += 0.5f;
}

void Stage::Draw()
{
	Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Stage::Release()
{
}
