#include "StickyGroup.h"
#include "Sticky.h"
#include "MeatBox.h"
#include "Effect.h"

int tmpId = 0;

StickyGroup::StickyGroup(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::STICKYGROUP;
	_bSetToMap = false;

	id = tmpId;
	tmpId++;
}

StickyGroup::~StickyGroup()
{
	_pSticky.clear();
	_pMeatBox.clear();
	_pDeleteEnemyList.clear();
}

bool StickyGroup::CheckMove(Vector3 vMove)
{
	// ���̃O���[�v���̃~�[�g�{�b�N�X�����ׂĈړ��ł��邩�ǂ����𒲂ׂ�
	bool bCanMove = true;
	for(auto pMB : _pMeatBox)
	{
		if (pMB->_CheckMove(vMove) == false)
		{
			bCanMove = false;
			break;
		}
	}

	// ���ׂẴ~�[�g�{�b�N�X���ړ��ł���ꍇ
	if (bCanMove)
	{
		// �~�[�g�{�b�N�X���ړ�
		for(auto pMB : _pMeatBox)
		{
			pMB->Move(vMove);
		}

		// �����Ԃ���G���폜
		for (auto pEnemy : _pDeleteEnemyList)
		{
			CreateEffect(Effect::TYPE::EXPLOSION, pEnemy->GetPos(), _mode);
			pEnemy->Destroy();
		}
	}

	_pDeleteEnemyList.clear();

	return bCanMove;
}

// ����StickyGroup�ƌ�������
void StickyGroup::Merge(StickyGroup* other)
{
	if (other == nullptr || this == other) return;

	// ��������StickyGroup���X�V
	for (auto pSticky : other->_pSticky)
	{
		pSticky->SetStickyGroup(this);
	}
	// vector�̌���
	_pSticky.reserve(_pSticky.size() + other->_pSticky.size());
	_pSticky.insert(_pSticky.end(), other->_pSticky.begin(), other->_pSticky.end());

	// ��������StickyGroup���X�V
	for(auto pMB : other->_pMeatBox)
	{
		pMB->SetStickyGroup(this);
	}
	// vector�̌���
	_pMeatBox.reserve(_pMeatBox.size() + other->_pMeatBox.size());
	_pMeatBox.insert(_pMeatBox.end(), other->_pMeatBox.begin(), other->_pMeatBox.end());

	// ����StickyGroup���폜
	_mode->RemoveGameObject(other);
}

void StickyGroup::AddSticky(Sticky* pSticky)
{
	if (pSticky == nullptr) return;

	_pSticky.push_back(pSticky);
	pSticky->SetStickyGroup(this);
}

void StickyGroup::AddMeatBox(MeatBox* pMeatBox)
{
	if (pMeatBox == nullptr) return;

	StickyGroup* tmpSG = pMeatBox->GetStickyGroup();
	if (tmpSG != nullptr) {
		// �~�[�g�{�b�N�X���ʂ�StickyGroup�ɏ������Ă���ꍇ�A��������
		if (tmpSG != this) {
			Merge(tmpSG);
		}
	}
	// �~�[�g�{�b�N�X���܂�StickyGroup�ɏ������Ă��Ȃ��ꍇ
	else {
		_pMeatBox.push_back(pMeatBox);
		pMeatBox->SetStickyGroup(this);
	}
}
