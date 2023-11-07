#pragma once
#include <vector>
#include <string>
#include "Fbx.h"

namespace Model
{
	struct ModelData
	{
		Fbx* pFbx = nullptr;
		Transform transform;
		std::string fileName;
	};

	/// <summary>
	/// ���f���f�[�^�����[�h����
	/// </summary>
	/// <param name="_fileName">�t�@�C����</param>
	/// <returns>���̃��f���f�[�^�Ɋ��蓖�Ă�ꂽ�ԍ�</returns>
	int Load(std::string _fileName);

	/// <summary>
	/// �Ώۃ��f���̃g�����X�t�H�[�����Z�b�g����
	/// </summary>
	/// <param name="_hModel">���f���n���h��</param>
	/// <param name="_t">�g�����X�t�H�[��</param>
	void SetTransform(int _hModel, Transform _t);

	/// <summary>
	/// FBX��Draw���g���ĕ`��
	/// </summary>
	/// <param name="_hModel">���f���n���h��</param>
	void Draw(int _hModel);

	/// <summary>
	/// �C�ӂ̃��f�������
	/// </summary>
	/// <param name="_hModel">���f���n���h��</param>
	void Release();

	/// <summary>
	/// �S�Ẵ��f�������
	/// </summary>
	void AllRelease();
};
