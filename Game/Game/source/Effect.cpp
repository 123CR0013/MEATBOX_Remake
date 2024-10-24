#include "Effect.h"
#include "ModeGame.h"
#include "Map.h"
#include "ApplicationMain.h"

Effect::Effect(ModeBase* mode) : GameObject(mode)
{
	_objectType = GameObject::TYPE::EFFECT;
	_bSetToMap = false;
	_bLoop = false;
	_effectType = TYPE::NONE;
}

Effect::~Effect()
{

}

void Effect::Process()
{

}

void Effect::AnimProcess()
{
	if (!_bUse) return;
	GameObject::AnimProcess();

	// 再生終了処理
	if (!_bLoop)
	{
		bool bEnd = true;
		for (auto& anim : _anims)
		{
			if (!anim->IsEnd())
			{
				bEnd = false;
				break;
			}
		}

		if (bEnd)
		{
			_mode->RemoveGameObject(this);
		}
	}
}

void Effect::Draw()
{
	if (!_bUse) return;

	if(!_bLoop)
	{
		for (auto& _anim : _anims)
		{
			if (_anim->IsEnd() == false)
			{
				_anim->Draw();
			}
		}
	}
}

void CreateEffect(Effect::TYPE type, Vector3 vPos, ModeBase* mode)
{
	switch (type)
	{
	case Effect::TYPE::NONE:
		break;
	case Effect::TYPE::MOVE:
		CreateEffect_Move(vPos, mode);
		break;
	case Effect::TYPE::IMPACT:
		CreateEffect_Impact(vPos, mode);
		break;
	case Effect::TYPE::QUESTION:
		CreateEffect_Question(vPos, mode);
		break;
	case Effect::TYPE::EXPLOSION:
		CreateEffect_Explosion(vPos, mode);
		break;
	default:
		break;
	}
}

void CreateEffect_Move(Vector3 vPos, ModeBase* mode)
{
	Effect* effect = new Effect(mode);
	effect->SetEffectType(Effect::TYPE::MOVE);
	effect->SetPos(vPos);

	AnimationInfo* animInfo = new AnimationInfo();
	ResourceServer::LoadDivGraph("res/Effect/Move/ef_smoke_sheet.png", 4, 4, 1, 100, 100, animInfo->_graphHandle);
	animInfo->_framePerSheet = 4;

	Animation* anim = effect->AddAnimationClass();
	anim->SetDrawOrder(1);
	anim->AddAnimInfo(animInfo, 0);

	mode->AddGameObject(effect);
}

void CreateEffect_Impact(Vector3 vPos, ModeBase* mode)
{
	Effect* effect = new Effect(mode);
	effect->SetEffectType(Effect::TYPE::IMPACT);
	effect->SetPos(vPos);

	Animation* anim = effect->AddAnimationClass();
	anim->SetDrawOffset(Vector3(0.0f, -0.2f, 0.0f));
	anim->SetSize(220, 220);
	anim->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT);

	AnimationInfo* animInfo = new AnimationInfo();
	ResourceServer::LoadDivGraph("res/Effect/Impact/ef_kick_sheet.png", 7, 7, 1, 160, 160, animInfo->_graphHandle);
	animInfo->_framePerSheet = 2;

	anim->AddAnimInfo(animInfo, 0);

	mode->AddGameObject(effect);
}

void CreateEffect_Question(Vector3 vPos, ModeBase* mode)
{
	Effect* effect = new Effect(mode);
	effect->SetEffectType(Effect::TYPE::QUESTION);
	effect->SetPos(vPos);

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Question/hatena.png"));
	animInfo->_framePerSheet = 24;

	Animation* anim = effect->AddAnimationClass();
	anim->SetDrawOffset(Vector3(-0.5f, -0.5f, 0));
	anim->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT);
	anim->AddAnimInfo(animInfo, 0);

	anim->SetZoom(0.5f);

	mode->AddGameObject(effect);
}

void CreateEffect_Explosion(Vector3 vPos, ModeBase* mode, int num)
{
	float w = 960;
	float h = 540;


	Vector3 vNewPos = vPos;
#if 0
	{
		// マップチップ座標でXY軸それぞれで -0.5f ～ -0.1f, 0.1f ~ 0.5f の範囲でランダムにずらす
		// 肉片、血の位置をずらす

		float randMin = 0.1f;
		float randMax = 0.8f;

		float randOffsetX = rand() % (int)(randMax * 100) / 100.0f;
		float signX = rand() % 2 == 0 ? 1.0f : -1.0f;
		randOffsetX = MyMath::Clamp(randMin, randMax, randOffsetX) * signX;

		float randOffsetY = rand() % (int)(randMax * 100) / 100.0f;
		float signY = rand() % 2 == 0 ? 1.0f : -1.0f;
		randOffsetY = MyMath::Clamp(randMin, randMax, randOffsetY) * signY;

		vNewPos.x += randOffsetX;
		vNewPos.y += randOffsetY;
	}
#else
	{
		// マップチップ座標で中心からランダムにずらす

		float randMin = 0.1f;
		float randMax = 0.8f;

		float randOffset = rand() % (int)(randMax * 100) / 100.0f;
		randOffset = MyMath::Clamp(randMin, randMax, randOffset);
		float randRad = rand() % 360;

		Matrix4 m = Matrix4::CreateRotationZ(MyMath::DegToRad(randRad)) * Matrix4::CreateScale(randOffset);
		vNewPos += Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f),m);
	}
#endif


	// 肉片
	{
		Effect* effect = new Effect(mode);
		effect->SetEffectType(Effect::TYPE::EX_MEAT);
		effect->SetPos(vNewPos);

		Animation* anim = effect->AddAnimationClass();
		anim->SetSize(w, h);
		anim->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT + 100);

		AnimationInfo* animInfo = new AnimationInfo();
		ResourceServer::LoadDivGraph("res/Effect/Explosion/Meat/ef_meat_sheet.png", 6, 6, 1, w, h, animInfo->_graphHandle);
		animInfo->_framePerSheet = 8;

		anim->AddAnimInfo(animInfo, 0);

		mode->AddGameObject(effect);
	}

	// 血
	{
		Effect* effect = new Effect(mode);
		effect->SetEffectType(Effect::TYPE::EX_BLOOD);
		effect->SetPos(vNewPos);

		Animation* anim = effect->AddAnimationClass();
		anim->SetSize(w, h);
		anim->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT + 200);

		AnimationInfo* animInfo = new AnimationInfo();
		ResourceServer::LoadDivGraph("res/Effect/Explosion/Blood/ef_blood_sheet.png", 7, 7, 1, w, h, animInfo->_graphHandle);
		animInfo->_framePerSheet = 8;

		anim->AddAnimInfo(animInfo, 0);

		mode->AddGameObject(effect);
	}

	if (0 <= num && num <= 2) {
		std::array<Vector3, 3> vScreenPos = {
			Vector3(1920.0f / 2.0f, 1080.0f - (h / 2.0f), 0.0f),
			Vector3(445, 324, 0),
			Vector3(1505, 540, 0),
		};

		// 目1
		{
			Effect* effect = new Effect(mode);
			effect->SetEffectType(Effect::TYPE::EX_EYE01);
			effect->SetPos(vPos);

			Animation* anim = effect->AddAnimationClass();
			anim->SetSize(w, h);
			anim->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT + 300);

			AnimationInfo* animInfo = new AnimationInfo();
			ResourceServer::LoadDivGraph("res/Effect/Explosion/Eye/ef_eye_sheet_01.png", 2, 2, 1, w, h, animInfo->_graphHandle);
			animInfo->_framePerSheet = 8;

			anim->AddAnimInfo(animInfo, 0);

			mode->AddGameObject(effect);
		}
		// 目2
		{
			Effect* effect = new Effect(mode);
			effect->SetEffectType(Effect::TYPE::EX_EYE02);
			effect->SetPos(vScreenPos.at(num));

			Animation* anim = effect->AddAnimationClass();
			anim->SetSize(560, 1240);
			anim->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT + 400);
			anim->SetDrawWithScreenPos(true);

			AnimationInfo* animInfo = new AnimationInfo();
			std::vector<int> graphHandle;
			animInfo->_graphHandle.push_back(-1);
			animInfo->_graphHandle.push_back(-1);
			ResourceServer::LoadDivGraph("res/Effect/Explosion/Eye/ef_eye_sheet_02.png", 4, 4, 1, 560, 1240, graphHandle);
			animInfo->_graphHandle.insert(animInfo->_graphHandle.end(), graphHandle.begin(), graphHandle.end());

			animInfo->_framePerSheet = 8;
			
			anim->AddAnimInfo(animInfo, 0);

			mode->AddGameObject(effect);
		}
	}
}
