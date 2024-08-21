#include "EffectExplosion.h"

EffectExplosion::EffectExplosion(ModeBase* mode) : Effect(mode) 
{

}

EffectExplosion::~EffectExplosion()
{

}

void EffectExplosion::Process()
{
	if (_anim->IsEnd()) {
		if (_anim->GetAnimIndex() == 0) {			
			_vPos = Vector3((float)MAP_W / 2.0f, (float)MAP_H / 2.0f, 0.0f);
			_anim->SetAnimIndex(1);
		}
		else {
			_mode->RemoveGameObject(this);
		}
	}
}
