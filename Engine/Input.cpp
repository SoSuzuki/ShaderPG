#include "Input.h"


namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256];			//���݂̊e�L�[�̏��
	BYTE prevKeyState[256];		//�O�t���[���ł̊e�L�[�̏��
	XMVECTOR mousePosition;

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);					//�f�o�C�X�I�u�W�F�N�g�̍쐬
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);										//����ŃL�[�{�[�h�Ƃ��Ĉ���
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);	//���A�v���ɑ΂�����̗͂D��x
	}

	void Update()
	{
		memcpy(prevKeyState, keyState, sizeof(BYTE) * 256);

		pKeyDevice->Acquire();	//���ꂪ�����ƃL�[�{�[�h��������(?)�炵���c
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);	//������keystate���X�V�����
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)	//10:128, 2:1000 0000
		{
			return true;
		}

		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsKey(keyCode) && !(prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//���͉����ĂȂ��āA�O��͉����Ă�
		if (!IsKey(keyCode) && prevKeyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
		
	}

	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 0);
	}

	void Release()
	{
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pDInput);
	}
}