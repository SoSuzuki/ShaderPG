#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>
#include <vector>

//��������̃}�N��
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}	 //�����F�|�C���^�ϐ� �����ō�����N���X�͂�����
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}//�����F�|�C���^�ϐ� DirectX�ŗL�̌^�͂�����

//�����J	�����̃v���W�F�N�g�̂ǂ��ɏ����Ă��ǂ��B�g���Ƃ���or�����J��p�̃t�@�C���ɏ����ƕ�����₷��
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;


namespace Direct3D
{
	enum SHADER_TYPE {
		SHADER_2D = 0,
		SHADER_3D,
		SHADER_MAX,
	};

	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	HRESULT InitShader();
	HRESULT InitShader2D();
	HRESULT InitShader3D();

	//�V�F�[�_�[�̃t�@�C���؂�ւ�
	void SetShader(SHADER_TYPE type);

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

	//extern	CPP���Œ�`�Y��Ȃ����ƁI
	extern ID3D11Device* pDevice_;
	extern ID3D11DeviceContext* pContext_;	//�f�o�C�X�R���e�L�X�g

};