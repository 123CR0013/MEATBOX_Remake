#pragma once
#include "../Sound/SoundItem.h"
#include "../Sound/SoundServer.h"
class Global
{
public:
//�֐�
	Global();
	~Global();
	bool Init();

	bool IsGameEnd()const { return _isGameEnd; }
	void SetGameEnd(bool isEnd) { _isGameEnd = isEnd; }

//�ϐ��E�N���X
	SoundServer* _soundServer;

	// ����
	int _key, _trg, _rel;
	int _mouseX,_mouseY;
	int _mouseKey, _mouseTrg, _mouseRel;

private:
	bool _isGameEnd;
};
extern Global global;

