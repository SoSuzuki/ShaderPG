#pragma once

#include <dInput.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")	//Inputに関しては早々変わることもないから８

//この1行のために他のヘッダーをインクルードするのは無駄だし書き直す
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