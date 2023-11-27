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
}

void Stage::Update()
{
	//transform_.rotate_.y += 0.5f;
}

void Stage::Draw()
{
    for (int x = 0; x < xSize_; x++) {
        for (int z = 0; z < zSize_; z++) {
                Transform bTrans;
                bTrans.position_.x = x;
                bTrans.position_.z = z;
                Model::SetTransform(hModel_, bTrans);
                Model::Draw(hModel_);
        }
    }
}

void Stage::Release()
{
}
