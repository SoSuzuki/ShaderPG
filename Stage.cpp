#include "Stage.h"
#include "Engine/Model.h"

#include "Engine/Quad.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Player"), hModel_(-1)
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
}

void Stage::Update()
{
	transform_.rotate_.y += 0.5f;
}

void Stage::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
	Quad* q = new Quad;
	q->Draw(transform_);
}

void Stage::Release()
{
}
