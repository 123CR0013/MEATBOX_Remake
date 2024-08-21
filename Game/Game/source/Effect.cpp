#include "Effect.h"
#include "EffectExplosion.h"
#include "ModeGame.h"

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

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Move/dust_a.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Move/dust_b.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Move/dust_c.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Move/dust_d.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Move/dust_e.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Move/dust_f.png"));
	animInfo->_framePerSheet = 4;
	effect->AddAnimInfo(animInfo);

	mode->AddGameObject(effect);
}

void CreateEffect_Impact(Vector3 vPos, ModeBase* mode)
{
	Effect* effect = new Effect(mode);
	effect->SetPos(vPos);

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Impact/Impact_a.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Impact/Impact_b.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Impact/Impact_c.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Impact/Impact_d.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Impact/Impact_e.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Impact/Impact_f.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Impact/Impact_g.png"));
	animInfo->_framePerSheet = 1;
	effect->AddAnimInfo(animInfo);

	mode->AddGameObject(effect);
}

void CreateEffect_Question(Vector3 vPos, ModeBase* mode)
{
	Effect* effect = new Effect(mode);
	effect->SetPos(vPos);
	effect->SetDrawOffset(Vector3(-0.5f, -0.5f, 0));

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

		AnimationInfo* animInfo = new AnimationInfo();
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Meat/nikuhen1.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Meat/nikuhen2.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Meat/nikuhen3.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Meat/nikuhen4.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Meat/nikuhen5.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Meat/nikuhen6.png"));
		animInfo->_framePerSheet = 8;
		effect->AddAnimInfo(animInfo);

		mode->AddGameObject(effect);
	}

	// –Ú
	{
		EffectExplosion* effect = new EffectExplosion(mode);
		effect->SetPos(vPos);
		effect->SetAnimSize(w, h);

		AnimationInfo* animInfo = new AnimationInfo();
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Eye/me1.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Eye/me2.png"));
		animInfo->_framePerSheet = 8;
		effect->AddAnimInfo(animInfo);

		AnimationInfo* animInfo2 = new AnimationInfo();
		animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Eye/me3.png"));
		animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Eye/me4.png"));
		animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Eye/me5.png"));
		animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Eye/me6.png"));
		animInfo2->_framePerSheet = 8;
		effect->AddAnimInfo(animInfo2);

		mode->AddGameObject(effect);
	}

	// ŒŒ
	{
		Effect* effect = new Effect(mode);
		effect->SetPos(vPos);
		effect->SetAnimSize(w, h);

		AnimationInfo* animInfo = new AnimationInfo();
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi1.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi2.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi3.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi4.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi5.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi6.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi7.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi8.png"));
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Effect/Explosion/Blood/chi9.png"));
		animInfo->_framePerSheet = 8;
		effect->AddAnimInfo(animInfo);

		mode->AddGameObject(effect);
	}
}
