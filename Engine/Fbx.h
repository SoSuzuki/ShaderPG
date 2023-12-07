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
		XMFLOAT4	diffuse;	// �g�U���ˌ��ւ̔��ˋ��x
		XMFLOAT4	ambient;	// �����ւ̔��ˋ��x
		XMFLOAT4	specular;	// ���ʔ��ˌ��ւ̔��ˋ��x
		float		shininess;	// �n�C���C�g�̋���(�T�C�Y)
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;			// ���[���h�A�r���[�A�v���W�F�N�V����
		XMMATRIX	matW;			// ���[���h�ϊ��̂�
		XMMATRIX	matNormal;		// �X�P�[���~���s�ړ��̋t�s��
		XMFLOAT4	diffuseColor;	// �ʂ̐F
		XMFLOAT4	specular;		// ���ʔ��ˌ��̏��iLambert�̏ꍇ�� 0�j
		FLOAT		shininess;		// �n�C���C�g�̋���
		XMFLOAT4	lightPosition;
		XMFLOAT4	eyePos;
		BOOL		isTextured;
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
	void InitTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, const DWORD& i);

	//void SetFlatColor(XMFLOAT4 col);

private:

	//---------Draw�֐�����Ă΂��֐�---------
	void PassDataToCB(DirectX::XMMATRIX worldMatrix);	//�R���X�^���g�o�b�t�@�ɏ���n��
	void SetBufferToPipeline();

};

