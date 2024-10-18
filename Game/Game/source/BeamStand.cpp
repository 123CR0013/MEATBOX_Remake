#include "BeamStand.h"
#include "BeamBody.h"

BeamStand::BeamStand(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::BEAM_STAND;
	_direction = DIRECTION::UP;
	_vDir = Vector3(0, 0, 0);

	_vBBEndPos = Vector3(0, 0, 0);
	_pBBEnd = nullptr;
	_bWallExist = false;
}

BeamStand::~BeamStand()
{

}

void BeamStand::Process()
{
	if (!_bUse) return;

	// ��, �M�~�b�N, �~�[�g�{�b�N�X, �G�̂����ꂩ�ɓ�����܂�BeamBody�𐶐�
	Vector3 vStartPos = _vPos + _vDir;
	_vBBEndPos = vStartPos;
	_bWallExist = false;
	while (true)
	{
		// �ǂƂ̓����蔻��
		MapChip* mapChip = _mapData->GetMapChip(_vBBEndPos);
		if(mapChip == nullptr || mapChip->GetType() == MapChip::TYPE::WALL)
		{
			_bWallExist = true;
			break;
		}

		// �~�[�g�{�b�N�X, �G�Ƃ̓����蔻��
		GameObject* obj = _mapData->GetGameObject(_vBBEndPos);
		if(obj != nullptr)
		{
			if(obj->GetType() == TYPE::MEATBOX || obj->GetType() == TYPE::ENEMY)
			{
				break;
			}
		}

		_vBBEndPos += _vDir;
	}

	UpdateBeamBodies(vStartPos, _vBBEndPos);

	_vBBEndPos -= _vDir;
}

void BeamStand::AnimProcess()
{
	if (!_bUse) return;

	SyncBeamBodyAnim();

	GameObject::AnimProcess();
}

void BeamStand::CreateBeamBody(Vector3 vPos)
{
	BeamBody* beamBody = new BeamBody(_mode);
	beamBody->SetPos(vPos);

	Animation* anim = beamBody->AddAnimationClass();
	anim->SetDrawOrder(12);
	_mode->LoadAnimData(anim, "BeamBody_Pink");
	Animation* subAnim = beamBody->AddAnimationClass();
	subAnim->SetDrawOrder(13);
	_mode->LoadAnimData(subAnim, "BeamBody_White");

	beamBody->SetDirection(_direction);

	_childObjects.push_back(beamBody);

	_pBBEnd = beamBody;
}

void BeamStand::SetUnuseAllBeamBody()
{
	for (auto& object : _childObjects)
	{
		if (object->GetType() == TYPE::BEAM_BODY)
		{
			object->SetUse(false);
		}
	}
}

// BeamBody�̃A�j���[�V������F���Ƃɓ���
void BeamStand::SyncBeamBodyAnim()
{
	//--------------------
	// BeamBody
	
	// BeamBody���������̏ꍇ
	if (_direction == DIRECTION::DOWN)
	{
		// �ǂɓ��������ꍇ
		if(_bWallExist)
		{
			_pBBEnd->GetAnimationClass(0)->SetAnimIndex(2);
			_pBBEnd->GetAnimationClass(1)->SetAnimIndex(2);

			// Orb�͔�\��
			GetAnimationClass(3)->SetUse(false);
			GetAnimationClass(4)->SetUse(false);
		}
		else
		{
			_pBBEnd->GetAnimationClass(0)->SetAnimIndex(1);
			_pBBEnd->GetAnimationClass(1)->SetAnimIndex(1);

			// Orb�͕\��
			GetAnimationClass(3)->SetUse(true);
			GetAnimationClass(4)->SetUse(true);
		}
	}


	// 1�ڂ�BeamBody�̃A�j���[�V�����ƈȍ~��BeamBody�̃A�j���[�V�����𓯊�����
	{
		int num = 0;
		float animCnt = 0;
		float animCntSub = 0;

		int randIndex = -1;
		int randIndexSub = -1;

		for (auto& object : _childObjects)
		{
			if (object->GetType() == TYPE::BEAM_BODY)
			{
				if (num == 0) {

					{
						Animation* anim = object->GetAnimationClass(0);
						animCnt = anim->GetAnimCnt();

						// �A�j���[�V�������I��������
						if (anim->IsEnd()) {
							// �A�j���[�V�����̊J�n�摜�������_���ɐݒ肷��
							randIndex = rand() % 3;
							anim->SetAnimCntForNumOfAnim(randIndex);
						}
					}

					{
						Animation* subAnim = object->GetAnimationClass(1);
						animCntSub = subAnim->GetAnimCnt();
						// �A�j���[�V�������I��������
						if (subAnim->IsEnd()) {
							// �A�j���[�V�����̊J�n�摜�������_���ɐݒ肷��
							randIndexSub = rand() % 3;
							subAnim->SetAnimCntForNumOfAnim(randIndexSub);
						}
					}
				}
				else {

					{
						Animation* anim = object->GetAnimationClass(0);
						anim->SetAnimCnt(animCnt);
						if (randIndex != -1)
						{
							anim->SetAnimCntForNumOfAnim(randIndex);
						}
					}

					{
						Animation* subAnim = object->GetAnimationClass(1);
						subAnim->SetAnimCnt(animCntSub);
						if (randIndexSub != -1)
						{
							subAnim->SetAnimCntForNumOfAnim(randIndexSub);
						}
					}
				}

				num++;
			}
		}
	}
	// END BeamBody
	//--------------------

	//--------------------
	// BeamOrb

	int randIndex = -1;
	if (GetAnimationClass(1)->IsEnd())
	{
		randIndex = rand() % 6;
		GetAnimationClass(1)->SetAnimCntForNumOfAnim(randIndex);
		GetAnimationClass(3)->SetAnimCntForNumOfAnim(randIndex);
	}
	if (GetAnimationClass(2)->IsEnd())
	{
		randIndex = rand() % 6;
		GetAnimationClass(2)->SetAnimCntForNumOfAnim(randIndex);
		GetAnimationClass(4)->SetAnimCntForNumOfAnim(randIndex);
	}

	std::array<Vector3, 4> drawOffsetTbl = {
	Vector3(0, -0.5f, 0),
	Vector3(0,  0.5f, 0),
	Vector3(-0.5f, -0.15f, 0),
	Vector3(0.5f, -0.15f, 0),
	};

	Vector3 drawOffset = (_vBBEndPos - _vPos) + drawOffsetTbl.at(static_cast<int>(_direction));
	GetAnimationClass(3)->SetDrawOffset(drawOffset);
	GetAnimationClass(4)->SetDrawOffset(drawOffset);

	// END BeamOrb
	//--------------------
}

void BeamStand::UpdateBeamBodies(Vector3 vStartPos, Vector3 vEndPos)
{
	Vector3 vSelectPos = vStartPos;

	GameObject* beamBody = nullptr;
	for (auto& object : _childObjects)
	{
		if (object->GetType() == TYPE::BEAM_BODY)
		{
			if (vSelectPos != vEndPos) {

				// �v���C���[�Ƃ̓����蔻��
				GameObject* obj = _mapData->GetGameObject(vSelectPos);
				if (obj != nullptr && obj->GetType() == TYPE::PLAYER)
				{
					_mode->SetGameOver();
				}

				object->SetUse(true);
				object->SetPos(vSelectPos);
				vSelectPos += _vDir;
			}
			else {
				object->SetUse(false);
			}
		}
	}

	while (true)
	{
		if (vSelectPos == vEndPos) break;

		// �v���C���[�Ƃ̓����蔻��
		GameObject* obj = _mapData->GetGameObject(vSelectPos);
		if (obj != nullptr && obj->GetType() == TYPE::PLAYER)
		{
			_mode->SetGameOver();
		}

		CreateBeamBody(vSelectPos);
		vSelectPos += _vDir;
	}
}


