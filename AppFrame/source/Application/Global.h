#pragma once
#include "../ResourceServer/ResourceServer.h"
#include "../Timer/Timer.h"
#include "../Sound/SoundItem.h"
#include "../Sound/SoundServer.h"
class Global
{
public:
//�֐�
	Global();
	~Global();
	bool Init();

//�ϐ��E�N���X
	Timer* _timer;
	SoundServer* _soundServer;

	// ����
	int _key, _trg, _rel;
};
extern Global global;

