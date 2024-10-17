#include "BeamStand.h"
#include "BeamBody.h"

BeamStand::BeamStand(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::BEAM_STAND;
	_vDir = Vector3(0, 0, 0);
}

BeamStand::~BeamStand()
{

}

void BeamStand::Process()
{
	if (!_bUse) return;

	// ��, �M�~�b�N, �~�[�g�{�b�N�X, �G�̂����ꂩ�ɓ�����܂�BeamBody�𐶐�
	Vector3 vStartPos = _vPos + _vDir;
	Vector3 vEndPos = vStartPos;
	while (true)
	{
		// �ǂƂ̓����蔻��
		MapChip* mapChip = _mapData->GetMapChip(vEndPos);
		if(mapChip == nullptr || mapChip->GetType() == MapChip::TYPE::WALL)
		{
			break;
		}

		// �~�[�g�{�b�N�X, �G�Ƃ̓����蔻��
		GameObject* obj = _mapData->GetGameObject(vEndPos);
		if(obj != nullptr)
		{
			if(obj->GetType() == TYPE::MEATBOX || obj->GetType() == TYPE::ENEMY)
			{
				break;
			}
		}

		vEndPos += _vDir;
	}

	UpdateBeamBodies(vStartPos, vEndPos);
}

void BeamStand::AnimProcess()
{
	if (!_bUse) return;

	SyncBeamBodyAnim();

	GameObject::AnimProcess();
}

void BeamStand::CreateBeamBody(Vector3 vPos)
{
	BeamBody::DIRECTION direction = BeamBody::DIRECTION::UP;
	if (_vDir.x > 0) {
		direction = BeamBody::DIRECTION::RIGHT;
	}
	else if (_vDir.x < 0) {
		direction = BeamBody::DIRECTION::LEFT;
	}
	else if (_vDir.y < 0) {
		direction = BeamBody::DIRECTION::UP;
	}
	else if (_vDir.y > 0) {
		direction = BeamBody::DIRECTION::DOWN;
	}

	for (int i = 0; i < 2; i++) {
		BeamBody* beamBody = new BeamBody(_mode);
		beamBody->SetPos(vPos);

		Animation* anim = beamBody->GetAnimation();
		_mode->LoadAnimData(anim, "BeamBody_Pink");
		Animation* subAnim = beamBody->GetSubAnimation();
		_mode->LoadAnimData(subAnim, "BeamBody_White");

		beamBody->SetDirection(direction);

		_childObjects.push_back(beamBody);
	}
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
	// 1�ڂ�BeamBody�̃A�j���[�V�����ƈȍ~��BeamBody�̃A�j���[�V�����𓯊�����

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
					Animation* anim = object->GetAnimation();
					animCnt = anim->GetAnimCnt();

					// �A�j���[�V�������I��������
					if (anim->IsEnd()) {
						// �A�j���[�V�����̊J�n�摜�������_���ɐݒ肷��
						randIndex = rand() % 3;
						anim->SetAnimCntForNumOfAnim(randIndex);
					}
				}

				{
					Animation* subAnim = object->GetSubAnimation();
					animCntSub = subAnim->GetAnimCnt();
					// �A�j���[�V�������I��������
					if (subAnim->IsEnd()) {
						// �A�j���[�V�����̊J�n�摜�������_���ɐݒ肷��
						randIndexSub = rand() % 3;
						subAnim->SetAnimCntForNumOfAnim(randIndexSub);
					}
				}

				num++;
			}
			else {

				{
					Animation* anim = object->GetAnimation();
					anim->SetAnimCnt(animCnt);
					if (randIndex != -1)
					{
						anim->SetAnimCntForNumOfAnim(randIndex);
					}
				}

				{
					Animation* subAnim = object->GetSubAnimation();
					subAnim->SetAnimCnt(animCntSub);
					if (randIndexSub != -1)
					{
						subAnim->SetAnimCntForNumOfAnim(randIndexSub);
					}
				}
			}
		}
	}
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


