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
	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4	diffuseColor;
		int		isTextured;
	};
	
	struct VERTEX
	{
		XMVECTOR position;	//�ʒu
		XMVECTOR uv;		//UV���W
		XMVECTOR normal;	//�@��
	};

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�
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

private:

	//---------Draw�֐�����Ă΂��֐�---------
	void PassDataToCB(DirectX::XMMATRIX worldMatrix);	//�R���X�^���g�o�b�t�@�ɏ���n��
	void SetBufferToPipeline();

};

