#include "GameObject.h"
#include "SphereCollider.h"

GameObject::GameObject():pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(parent), objectName_(name), pCollider_(nullptr), isDead_(false)
{
	if (parent/* != nullptr*/)
		this->transform_.pParent_ = &(parent->transform_);
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	Update();

	RoundRobin(GetRootJob());
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		(*itr)->UpdateSub();	// �q�ǂ��̎q�ǂ����q�ǂ����Ăяo���Ă��[�����[���c(��₱����)

	for (auto itr = childList_.begin(); itr != childList_.end();) {
		if ((*itr)->isDead_) {
			(*itr)->ReleaseSub();
			itr = childList_.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		(*itr)->DrawSub();
}

void GameObject::ReleaseSub()
{	//�����ŃI�u�W�F�N�g��������Ă��܂��̂�delete���Ȃ��čς�
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++) {
		(*itr)->ReleaseSub();
		SAFE_DELETE(*itr);
	}
	Release();
}

void GameObject::Killme()
{
	isDead_ = true;
}

XMMATRIX GameObject::GetWolrdMatrix(void)
{
	return transform_.GetWorldMatrix();
}

GameObject* GameObject::FindChildObject(std::string& _objName)
{
	if (_objName == this->objectName_) {
		return(this);	// ������_objName�̃I�u�W�F�N�g��������Athis��NULL��
	}
	else {
		for (auto &itr:childList_) {	// auto &e: childlist�ł�����
			GameObject* obj = itr->FindChildObject(_objName);
			if (obj != nullptr)
				return obj;
		}
	}
	return nullptr;
}

GameObject* GameObject::GetRootJob()
{	//�ċN����
	if (pParent_ == nullptr)
		return this;
	return pParent_->GetRootJob();
}

GameObject* GameObject::FindObject(std::string _objName)
{	// �������猩��RootJob��T���A��������S�Ă̎q��H���āA_objName��T��
	return GetRootJob()->FindChildObject(_objName);
}

//�I�u�W�F�N�g�̖��O���擾
const std::string& GameObject::GetObjectName(void) const
{
	return objectName_;
}

void GameObject::AddCollider(SphereCollider* _pCollider)
{
	pCollider_ = _pCollider;
}

void GameObject::Collision(GameObject* _pTarget)
{
	if (_pTarget == this || _pTarget->pCollider_ == nullptr)
		return;	// �������g or �^�[�Q�b�g�ɃR���C�_�[���A�^�b�`����ĂȂ�
	
	//XMVECTOR v{ transform_.position_.x - _pTarget->transform_.position_.x,
	//			transform_.position_.y - _pTarget->transform_.position_.y,
	//			transform_.position_.z - _pTarget->transform_.position_.z,
	//			0 };
	//XMVECTOR dist = XMVector3Dot(v, v);

	// pow�֐���������2�悵����������
	float dist = (transform_.position_.x - _pTarget->transform_.position_.x) * (transform_.position_.x - _pTarget->transform_.position_.x)
		+ (transform_.position_.y - _pTarget->transform_.position_.y) * (transform_.position_.y - _pTarget->transform_.position_.y)
		+ (transform_.position_.z - _pTarget->transform_.position_.z) * (transform_.position_.z - _pTarget->transform_.position_.z);

	float rDist = (this->pCollider_->GetRadius() + _pTarget->pCollider_->GetRadius()) * (this->pCollider_->GetRadius() + _pTarget->pCollider_->GetRadius());

	// �����ƃ^�[�Q�b�g�̋��� <= R1 + R2�Ȃ� (�����A�����̃R���C�_�[�ƃ^�[�Q�b�g���Ԃ����Ă�����
	// onCollision�֐����Ăяo��
	if (dist <= rDist) {
		//onCollision(_pTarget);
		double p = 0;	// �f�o�b�O�p
	}

}

void GameObject::OnCollision(GameObject* _pTarget)
{
	_pTarget->Killme();
}

void GameObject::RoundRobin(GameObject* _pTarget)
{
	if (pCollider_ == nullptr)
		return;
	if(_pTarget->pCollider_!=nullptr)
		Collision(_pTarget);

	// �^�[�Q�b�g�̎q���X�g���瑍������
	for (auto& itr : _pTarget->childList_)
	{
		RoundRobin(itr);
	}

}









