#include "Sticky.h"
#include "StickyGroup.h"
#include "MeatBox.h"

Sticky::Sticky(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::STICKY;

	_pStickyGroup = nullptr;
	_pRootMB = nullptr;
	_bMBExist.fill(false);
}

Sticky::~Sticky()
{
	_pStickyGroup = nullptr;
	_pRootMB = nullptr;
	_bMBExist.fill(false);
}

void Sticky::Process()
{
	GameObject::Process();

	// �˂΂˂Ζ{�̂��~�[�g�{�b�N�X�ɂ������Ă���ꍇ�A�˂΂˂Ζ{�̂̍��W���~�[�g�{�b�N�X�̍��W�ɍ��킹��
	if(_bMBExist[0] == true)
	{
		_vPos = _pRootMB->GetPos();

		// ����4�}�X�Ƀ~�[�g�{�b�N�X�����邩�𒲂ׂ�
		CheckMeatBoxExistAround();
	}
	else {
		// �˂΂˂Ζ{�̂��~�[�g�{�b�N�X�ɂ������Ă��Ȃ��ꍇ�A�˂΂˂Ζ{�̂̍��W���}�b�v�ɓo�^����
		// �v���C���[����ɏ�����Ƃ��ɂ˂΂˂Ζ{�̂������Ȃ��悤�ɂ��邽��
		if (_mapData->GetGameObject(_vPos) == nullptr)
		{
			_mapData->SetGameObject(this, _vPos);
		}
	}
}

bool Sticky::AddMeatBox(MeatBox* pMeatBox, Sticky::POSITION pos)
{
	if(_bMBExist[(int)pos] == false && pMeatBox != nullptr)
	{
		_bMBExist[(int)pos] = true;

		if(pos == POSITION::ROOT)
		{
			_pRootMB = pMeatBox;
			_mapData->EraseGameObject(this);
		}

		// StickyGroup�Ƀ~�[�g�{�b�N�X��ǉ�
		_pStickyGroup->AddMeatBox(pMeatBox);

		return true;
	}
	return false;
}

// �˂΂˂Ζ{�̂̎���4�}�X�Ƀ~�[�g�{�b�N�X�����邩�𒲂ׂ�
void Sticky::CheckMeatBoxExistAround()
{
	// �˂΂˂Ζ{�̂��~�[�g�{�b�N�X�ɂ������Ă���ꍇ
	if (_bMBExist[0] == true)
	{
		// �˂΂˂Ζ{�̂̍��W���~�[�g�{�b�N�X�̍��W�ɍ��킹��
		_vPos = _pRootMB->GetPos();

		std::array<Vector3, 4> vPos = {
			_vPos + VGet(0, -1, 0),
			_vPos + VGet(0, 1, 0),
			_vPos + VGet(-1, 0, 0),
			_vPos + VGet(1, 0, 0)
		};

		// �㉺���E�Ƀ~�[�g�{�b�N�X�����邩�ǂ����𒲂ׂ�
		for (int i = 1; i < 5; i++)
		{
			if (_bMBExist[i] != false) continue;

			GameObject* obj = _mapData->GetGameObject(vPos[i - 1]);
			if (obj != nullptr && obj->GetType() == TYPE::MEATBOX)
			{
				AddMeatBox(static_cast<MeatBox*>(obj), static_cast<POSITION>(i));
			}
		}
	}
}