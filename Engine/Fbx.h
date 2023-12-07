#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Direct3D.h"
#include "Transform.h"
#include "Camera.h"
//#include "Texture.h"

#pragma comment(lib, "LibFbxSDK-Md.lib")
#pragma comment(lib, "LibXml2-Md.lib")
#pragma comment(lib, "zlib-Md.lib")

using namespace Direct3D;

class Texture;

class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;	// 拡散反射光への反射強度
		XMFLOAT4	ambient;	// 環境光への反射強度
		XMFLOAT4	specular;	// 鏡面反射光への反射強度
		float		shininess;	// ハイライトの強さ(サイズ)
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;			// ワールド、ビュー、プロジェクション
		XMMATRIX	matW;			// ワールド変換のみ
		XMMATRIX	matNormal;		// スケール×平行移動の逆行列
		XMFLOAT4	diffuseColor;	// 面の色
		XMFLOAT4	specular;		// 鏡面反射光の情報（Lambertの場合は 0）
		FLOAT		shininess;		// ハイライトの強さ
		XMFLOAT4	lightPosition;
		XMFLOAT4	eyePos;
		BOOL		isTextured;
	};
	
	struct VERTEX
	{
		XMVECTOR position;	//位置
		XMVECTOR uv;		//UV座標
		XMVECTOR normal;	//法線
	};

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数
	//std::vector<int>indexCount_;
	int* indexCount_;

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;

	Texture* pTexture_;

public:

	Fbx();
	~Fbx();

	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void Release();

	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
	void InitTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, const DWORD& i);

	//void SetFlatColor(XMFLOAT4 col);

private:

	//---------Draw関数から呼ばれる関数---------
	void PassDataToCB(DirectX::XMMATRIX worldMatrix);	//コンスタントバッファに情報を渡す
	void SetBufferToPipeline();

};

