#pragma once
#include "../ResourceServer/ResourceServer.h"
#include "../Timer/Timer.h"
#include "../Sound/SoundItem.h"
#include "../Sound/SoundServer.h"
class Global
{
public:
//ŠÖ”
	Global();
	~Global();
	bool Init();

//•Ï”EƒNƒ‰ƒX
	Timer* _timer;
	SoundServer* _soundServer;

	// “ü—Í
	int _key, _trg, _rel;
};
extern Global global;

