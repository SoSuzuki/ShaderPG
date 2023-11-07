#pragma once

#include <dInput.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")	//Input�Ɋւ��Ă͑��X�ς�邱�Ƃ��Ȃ�����W

//����1�s�̂��߂ɑ��̃w�b�_�[���C���N���[�h����͖̂��ʂ�����������
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);
	XMVECTOR GetMousePosition();
	void SetMousePosition(int x, int y);
	void Release();
};