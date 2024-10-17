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

	// 壁, ギミック, ミートボックス, 敵のいずれかに当たるまでBeamBodyを生成
	Vector3 vStartPos = _vPos + _vDir;
	Vector3 vEndPos = vStartPos;
	while (true)
	{
		// 壁との当たり判定
		MapChip* mapChip = _mapData->GetMapChip(vEndPos);
		if(mapChip == nullptr || mapChip->GetType() == MapChip::TYPE::WALL)
		{
			break;
		}

		// ミートボックス, 敵との当たり判定
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

// BeamBodyのアニメーションを色ごとに同期
void BeamStand::SyncBeamBodyAnim()
{
	// 1つ目のBeamBodyのアニメーションと以降のBeamBodyのアニメーションを同期する

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

					// アニメーションが終了したら
					if (anim->IsEnd()) {
						// アニメーションの開始画像をランダムに設定する
						randIndex = rand() % 3;
						anim->SetAnimCntForNumOfAnim(randIndex);
					}
				}

				{
					Animation* subAnim = object->GetSubAnimation();
					animCntSub = subAnim->GetAnimCnt();
					// アニメーションが終了したら
					if (subAnim->IsEnd()) {
						// アニメーションの開始画像をランダムに設定する
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

				// プレイヤーとの当たり判定
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

		// プレイヤーとの当たり判定
		GameObject* obj = _mapData->GetGameObject(vSelectPos);
		if (obj != nullptr && obj->GetType() == TYPE::PLAYER)
		{
			_mode->SetGameOver();
		}

		CreateBeamBody(vSelectPos);
		vSelectPos += _vDir;
	}
}


