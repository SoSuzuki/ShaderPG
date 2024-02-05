#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Controller.h"
#include "Stage.h"
#include "Arrow.h"
#include "Ball.h"
#include "Dice.h"
#include "Water.h"


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
	//Instantiate<Arrow>(this);
	Instantiate<Ball>(this);
	Instantiate<Controller>(this);
	//Instantiate<Water>(this);
	Instantiate<Dice>(this);
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
