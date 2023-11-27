#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Controller.h"
#include "Stage.h"
#include "Arrow.h"
#include "Ball.h"


TestScene::TestScene(GameObject* parent)
	:GameObject(parent,"TestScene")
{
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Arrow>(this);
	Instantiate<Ball>(this);
	Instantiate<Controller>(this);
	//Camera::SetPosition(XMFLOAT3(7.0f, 5.0f, 7.0f));
	//Camera::SetTarget(XMFLOAT3(7.0f, 7.0f, 7.0f));
}

void TestScene::Update()
{
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
