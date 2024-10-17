#pragma once
#include "GameObject.h"

class Effect : public GameObject
{
public:
	
	enum class TYPE {
		NONE,
		MOVE,
		IMPACT,
		QUESTION,
		EXPLOSION,
	};

	Effect(ModeBase* mode);
	~Effect() override;

	void Process() override;
	void AnimProcess() override;
	void Draw() override;

	void SetLoop(bool bLoop) { _bLoop = bLoop; }

protected:
	// ÉãÅ[Évçƒê∂Ç∑ÇÈÇ©Ç«Ç§Ç©
	bool _bLoop;
};

void CreateEffect(Effect::TYPE type, Vector3 vPos, ModeBase* mode);

void CreateEffect_Move(Vector3 vPos, ModeBase* mode);
void CreateEffect_Impact(Vector3 vPos, ModeBase* mode);
void CreateEffect_Question(Vector3 vPos, ModeBase* mode);
void CreateEffect_Explosion(Vector3 vPos, ModeBase* mode, int num = 0);