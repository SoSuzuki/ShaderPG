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
	//�}�e���A��
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
		XMMATRIX	matWVP;			// ���[���h�A�r���[�A�v���W�F�N�V����
		XMMATRIX	matW;			// ���[���h�ϊ��̂�
		XMMATRIX	matNormal;		// �X�P�[���~���s�ړ��̋t�s��
		XMFLOAT4	diffuseColor;	// �ʂ̐F
		XMFLOAT4	ambientColor;
		XMFLOAT4	specularColor;
		float		shininess;
		BOOL		hasTexture;
		BOOL		hasNormalMap;
		float		scroll;
	};
	
	struct VERTEX
	{
		XMVECTOR position;	//�ʒu
		XMVECTOR uv;		//UV���W
		XMVECTOR normal;	//�@���x�N�g��
		XMVECTOR tangent;	//�ڐ��x�N�g��
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

	//---------Draw�֐�����Ă΂��֐�---------
	//void PassDataToCB(DirectX::XMMATRIX worldMatrix);	//�R���X�^���g�o�b�t�@�ɏ���n��
	//void SetBufferToPipeline();

};

