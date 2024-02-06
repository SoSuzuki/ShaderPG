#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>
#include <vector>

//解放処理のマクロ
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}	 //引数：ポインタ変数 自分で作ったクラスはこっち
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}//引数：ポインタ変数 DirectX固有の型はこっち

//リンカ	←このプロジェクトのどこに書いても良い。使うところorリンカ専用のファイルに書くと分かりやすい
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;


namespace Direct3D
{
	enum SHADER_TYPE {
		SHADER_2D = 0,
		SHADER_3D,
		SHADER_POINT,
		SHADER_TOON,
		SHADER_TOONOUT,
		SHADER_NORMALMAP,
		SHADER_MAX,
	};

	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	HRESULT InitShader();
	HRESULT InitShader2D();
	HRESULT InitShader3D();
	HRESULT InitToonShade();
	HRESULT InitToonOutline();
	HRESULT InitNormalMapRenderer();
	HRESULT InistShaderPointLight();

	//シェーダーのファイル切り替え
	void SetShader(SHADER_TYPE type);

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

	/// <summary>
	/// Zバッファへの書き込みON/OFF
	/// </summary>
	/// <param name="_isWrite">true = 書き込みON/false = 書き込みOFF</param>
	void SetDepthBafferWriteEnable(bool _isWrite);

	//extern	CPP側で定義忘れないこと！
	extern ID3D11Device* pDevice_;
	extern ID3D11DeviceContext* pContext_;	//デバイスコンテキスト
	extern SIZE screenSize;

};