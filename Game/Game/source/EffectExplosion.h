#pragma once
#include "Effect.h"

class EffectExplosion : public Effect {
public:
	EffectExplosion(ModeBase* mode);
	~EffectExplosion();

	void Process()override;

protected:

};