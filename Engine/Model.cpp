#include "Model.h"
#include "Direct3D.h"

namespace Model
{
	std::vector<ModelData*>modelList;

	int Model::Load(std::string _fileName)
	{
		// �\���̂̃C���X�^���X���쐬
		ModelData* pData = new ModelData;

		// ���̃����o�̃t�@�C�����Ɉ�������
		pData->fileName = _fileName;

		// Fbx�I�u�W�F�N�g���쐬���A���[�h
		bool isUsed = false;	//�g�������Ƃ̂��郂�f����

		for (auto& e : modelList)
		{

			if (e != nullptr && e->fileName == _fileName) {
				pData->pFbx = e->pFbx;
				isUsed = true;
				break;
			}
		}

		//�g�������Ƃ̂Ȃ��t�@�C�����J��
		if (isUsed == false) {
			pData->pFbx = new Fbx;
			if (FAILED(pData->pFbx->Load(_fileName))) {
				//�J���Ȃ�����
				SAFE_DELETE(pData->pFbx);
				SAFE_DELETE(pData);
				return -1;
			}
			pData->fileName = _fileName;	//�J����
		}

		// �\���̂̒��g�����܂����̂ŁA���I�z��ɓ˂�����
		modelList.push_back(pData);

		// �ԍ��i�z��̗v�f��-1�j��Ԃ�
		return (int)modelList.size() - 1;
	}

	void Model::SetTransform(int _hModel, Transform _t)
	{	//���f���ԍ��́AmodelList�̃C���f�b�N�X
		modelList[_hModel]->transform = _t;
	}

	void Model::Draw(int _hModel)
	{	//���f���ԍ��́AmodelList�̃C���f�b�N�X
		modelList[_hModel]->pFbx->Draw(modelList[_hModel]->transform);
	}

	void Release()
	{
		bool isReffed = false;	//�Q�Ƃ��ꂽ��
		for (int i = 0; i < modelList.size(); i++)
		{
			for (int j = i + 1; j < modelList.size(); j++)
			{
				if (modelList[i]->pFbx == modelList[j]->pFbx) {	// �ǂ��������Е����Q�Ƃ��Ă�
					isReffed = true;
					break;
				}
			}
			if (isReffed == false) {	// �Q�Ƃ��ĂȂ��Ȃ�delete
				SAFE_DELETE(modelList[i]->pFbx);
			}
			SAFE_DELETE(modelList[i]);
		}
		modelList.clear();	// �������ɓ����Ă�A�h���X������(���������[�N��h��)
	}

	//void AllRelease()
	//{
	//	for (int i = 0; i < modelList.size(); i++)
	//	{
	//		if (modelList[i] != nullptr) {
	//			Release(i);
	//		}
	//	}
	//}
};