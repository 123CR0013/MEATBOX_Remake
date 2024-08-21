#pragma once
#include "../ResourceServer/ResourceServer.h"
#include "../Timer/Timer.h"
#include "../Sound/SoundItem.h"
#include "../Sound/SoundServer.h"
class Global
{
public:
//関数
	Global();
	~Global();
	bool Init();

//変数・クラス
	Timer* _timer;
	SoundServer* _soundServer;

	// 入力
	int _key, _trg, _rel;
};
extern Global global;

