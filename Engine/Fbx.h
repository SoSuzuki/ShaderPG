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

enum RENDER_STATE
{
	RENDER_DIRLIGHT,
	RENDER_PNTLIGHT,
};

class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture*	pTexture;
		Texture*	pNormalMap;
		XMFLOAT4	diffuse;
		XMFLOAT4	ambient;
		XMFLOAT4	specular;
		float		shininess;
	};

	struct CBUFF_MODEL
	{
		XMMATRIX	matWVP;			// ワールド、ビュー、プロジェクション
		XMMATRIX	matW;			// ワールド変換のみ
		XMMATRIX	matNormal;		// スケール×平行移動の逆行列
		XMFLOAT4	diffuseColor;	// 面の色
		XMFLOAT4	ambientColor;
		XMFLOAT4	specularColor;
		float		shininess;
		BOOL		hasTexture;
		BOOL		hasNormalMap;
		float		scroll;
	};
	
	struct VERTEX
	{
		XMVECTOR position;	//位置
		XMVECTOR uv;		//UV座標
		XMVECTOR normal;	//法線ベクトル
		XMVECTOR tangent;	//接線ベクトル
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

	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
	RENDER_STATE state_;
	float scrollVal_;
public:

	Fbx();
	~Fbx();

	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void SetRenderingShader(RENDER_STATE _state) { state_ = _state; }
	void Release();

	//void SetFlatColor(XMFLOAT4 col);

private:

	//---------Draw関数から呼ばれる関数---------
	//void PassDataToCB(DirectX::XMMATRIX worldMatrix);	//コンスタントバッファに情報を渡す
	//void SetBufferToPipeline();

};

