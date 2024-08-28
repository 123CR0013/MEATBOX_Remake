#include "Global.h"

Global global;

Global::Global() {
	_soundServer = new SoundServer();

	_isGameEnd = false;

	_key = 0;
	_trg = 0;
	_rel = 0;

	_mouseX = 0;
	_mouseY = 0;

	_mouseKey = 0;
	_mouseTrg = 0;
	_mouseRel = 0;
};

Global::~Global() {
	delete _soundServer;
};

bool Global::Init() {
	return true;
};