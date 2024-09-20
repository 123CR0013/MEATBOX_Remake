#include "Effect.h"
#include "EffectExplosion.h"
#include "ModeGame.h"
#include "Map.h"
#include "ApplicationMain.h"

Effect::Effect(ModeBase* mode) : GameObject(mode)
{
	_bSetToMap = false;
	_bLoop = false;
}

Effect::~Effect()
{

}

void Effect::Process()
{
	if(!_bLoop)
	{
		if (_anim->IsEnd())
		{
			_mode->RemoveGameObject(this);
		}
	}
}

void Effect::Draw()
{
	if(!_bLoop)
	{
		if (_anim->IsEnd())
		{
			return;
		}
	}

	GameObject::Draw();
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
	effect->SetPos(vPos);
	effect->SetDrawOrder(DRAW_ORDER_UNDERLAP_OBJECT);

	AnimationInfo* animInfo = new AnimationInfo();
	ResourceServer::LoadDivGraph("res/Effect/Move/ef_smoke_sheet.png", 4, 4, 1, 100, 100, animInfo->_graphHandle);
	animInfo->_framePerSheet = 4;
	effect->AddAnimInfo(animInfo);

	mode->AddGameObject(effect);
}

void CreateEffect_Impact(Vector3 vPos, ModeBase* mode)
{
	Effect* effect = new Effect(mode);
	effect->SetPos(vPos);
	effect->SetDrawOffset(Vector3(0.0f, -0.2f, 0.0f));
	effect->SetAnimSize(220, 220);
	effect->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT);

	AnimationInfo* animInfo = new AnimationInfo();
	ResourceServer::LoadDivGraph("res/Effect/Impact/ef_kick_sheet.png", 7, 7, 1, 160, 160, animInfo->_graphHandle);
	animInfo->_framePerSheet = 2;
	effect->AddAnimInfo(animInfo);

	mode->AddGameObject(effect);
}

void CreateEffect_Question(Vector3 vPos, ModeBase* mode)
{
	Effect* effect = new Effect(mode);
	effect->SetPos(vPos);
	effect->SetDrawOffset(Vector3(-0.5f, -0.5f, 0));
	effect->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT);

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Question/hatena.png"));
	animInfo->_framePerSheet = 24;
	effect->AddAnimInfo(animInfo);

	effect->SetAnimZoom(0.5f);

	mode->AddGameObject(effect);
}

void CreateEffect_Explosion(Vector3 vPos, ModeBase* mode)
{
	float w = 960;
	float h = 540;

	// “÷•Ð
	{
		Effect* effect = new Effect(mode);
		effect->SetPos(vPos);
		effect->SetAnimSize(w, h);
		effect->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT);

		AnimationInfo* animInfo = new AnimationInfo();
		ResourceServer::LoadDivGraph("res/Effect/Explosion/Meat/ef_meat_sheet.png", 6, 6, 1, w, h, animInfo->_graphHandle);
		animInfo->_framePerSheet = 8;
		effect->AddAnimInfo(animInfo);

		mode->AddGameObject(effect);
	}

	// –Ú1
	{
		Effect* effect = new Effect(mode);
		effect->SetPos(vPos);
		effect->SetAnimSize(w, h);
		effect->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT);

		AnimationInfo* animInfo = new AnimationInfo();
		ResourceServer::LoadDivGraph("res/Effect/Explosion/Eye/ef_eye_sheet_01.png", 2, 2, 1, w, h, animInfo->_graphHandle);
		animInfo->_framePerSheet = 8;
		effect->AddAnimInfo(animInfo);

		mode->AddGameObject(effect);
	}
	// –Ú2
	{
		Effect* effect = new Effect(mode);
		effect->SetPos(Vector3(g_oApplicationMain.DispSizeW() / 2.0f, g_oApplicationMain.DispSizeH() / 2.0f, 0.0f));
		effect->SetPos(Vector3(g_oApplicationMain.DispSizeW() / 2.0f, g_oApplicationMain.DispSizeH() - (h / 2.0f), 0.0f));
		effect->SetDrawWithScreenPos(true);
		effect->SetAnimSize(w, h);
		effect->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT);

		AnimationInfo* animInfo = new AnimationInfo();
		std::vector<int> graphHandle;
		animInfo->_graphHandle.push_back(-1);
		animInfo->_graphHandle.push_back(-1);
		ResourceServer::LoadDivGraph("res/Effect/Explosion/Eye/ef_eye_sheet_02.png", 4, 4, 1, w, h, graphHandle);
		animInfo->_graphHandle.insert(animInfo->_graphHandle.end(), graphHandle.begin(), graphHandle.end());

		animInfo->_framePerSheet = 8;
		effect->AddAnimInfo(animInfo);

		mode->AddGameObject(effect);
	}

	// ŒŒ
	{
		Effect* effect = new Effect(mode);
		effect->SetPos(vPos);
		effect->SetAnimSize(w, h);
		effect->SetDrawOrder(DRAW_ORDER_OVERLAP_OBJECT);

		AnimationInfo* animInfo = new AnimationInfo();
		ResourceServer::LoadDivGraph("res/Effect/Explosion/Blood/ef_blood_sheet.png", 7, 7, 1, w, h, animInfo->_graphHandle);
		animInfo->_framePerSheet = 8;
		effect->AddAnimInfo(animInfo);

		mode->AddGameObject(effect);
	}
}
