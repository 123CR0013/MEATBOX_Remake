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

void BeamStand::CreateBeamBody(Vector3 vPos)
{
	BeamBody* beamBody = new BeamBody(_mode);
	beamBody->SetPos(vPos);

	_mode->LoadAnimData(beamBody, "BeamBody");

	_childObjects.push_back(beamBody);
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


