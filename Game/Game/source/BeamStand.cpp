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

	// 壁, ギミック, ミートボックス, 敵のいずれかに当たるまでBeamBodyを生成
	Vector3 vStartPos = _vPos + _vDir;
	_vBBEndPos = vStartPos;
	_bWallExist = false;
	while (true)
	{
		// 壁との当たり判定
		MapChip* mapChip = _mapData->GetMapChip(_vBBEndPos);
		if(mapChip == nullptr || mapChip->GetType() == MapChip::TYPE::WALL)
		{
			_bWallExist = true;
			break;
		}

		// ミートボックス, 敵との当たり判定
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

// BeamBodyのアニメーションを色ごとに同期
void BeamStand::SyncBeamBodyAnim()
{
	//--------------------
	// BeamBody
	
	// BeamBodyが下向きの場合
	if (_direction == DIRECTION::DOWN)
	{
		// 壁に当たった場合
		if(_bWallExist)
		{
			_pBBEnd->GetAnimationClass(0)->SetAnimIndex(2);
			_pBBEnd->GetAnimationClass(1)->SetAnimIndex(2);

			// Orbは非表示
			GetAnimationClass(3)->SetUse(false);
			GetAnimationClass(4)->SetUse(false);
		}
		else
		{
			_pBBEnd->GetAnimationClass(0)->SetAnimIndex(1);
			_pBBEnd->GetAnimationClass(1)->SetAnimIndex(1);

			// Orbは表示
			GetAnimationClass(3)->SetUse(true);
			GetAnimationClass(4)->SetUse(true);
		}
	}


	// 1つ目のBeamBodyのアニメーションと以降のBeamBodyのアニメーションを同期する
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

						// アニメーションが終了したら
						if (anim->IsEnd()) {
							// アニメーションの開始画像をランダムに設定する
							randIndex = rand() % 3;
							anim->SetAnimCntForNumOfAnim(randIndex);
						}
					}

					{
						Animation* subAnim = object->GetAnimationClass(1);
						animCntSub = subAnim->GetAnimCnt();
						// アニメーションが終了したら
						if (subAnim->IsEnd()) {
							// アニメーションの開始画像をランダムに設定する
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


