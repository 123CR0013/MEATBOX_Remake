#include "ApplicationMain.h"
#include "ModeGame.h"
#include"ModeSoundLayer.h"
#include"ModeSelect.h"

// ŽÀ‘Ì
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }
	// ƒ‚[ƒh‚Ì“o˜^
	ModeServer::GetInstance()->Add(NEW ModeSoundLayer(), 0, "SoundLayer");
	ModeServer::GetInstance()->Add(NEW ModeGame(), 1, "ModeGame");

	// FPS‚ðˆÀ’è‚³‚¹‚é‚½‚ß‚ÌƒNƒ‰ƒX‚ð‰Šú‰»
	_fpsController = NEW FpsController();
	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	delete _fpsController;
	ResourceServer::Release();
	return true;
}

bool ApplicationMain::Input() {
	base::Input();

	int oldKey = global._key;
	global._key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	global._trg = (global._key ^ oldKey) & global._key;
	global._rel = (global._key ^ oldKey) & ~global._key;

	oldKey = global._mouseKey;
	global._mouseKey = GetMouseInput();
	global._mouseTrg = (global._mouseKey ^ oldKey) & global._mouseKey;
	global._mouseRel = (global._mouseKey ^ oldKey) & ~global._mouseKey;

	GetMousePoint(&(global._mouseX), &(global._mouseY));

	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	_fpsController->WaitFps();

	return true;
}

bool ApplicationMain::Render() {
	base::Render();

#ifdef _DEBUG
	_fpsController->DrawFps(10, 10);
#endif // _DEBUG

	return true;
}

