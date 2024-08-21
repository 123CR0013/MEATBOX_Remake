#include "Global.h"

Global global;

Global::Global() {
	_timer = new Timer();
	_soundServer = new SoundServer();
	_timer->Start();

	_key = 0;
	_trg = 0;
	_rel = 0;
};

Global::~Global() {
	delete _timer;
	delete _soundServer;
};

bool Global::Init() {
	return true;
};