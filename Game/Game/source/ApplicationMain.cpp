#include "ApplicationMain.h"
#include "ModeGame.h"

// ����
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }
	// ���[�h�̓o�^
	ModeServer::GetInstance()->Add(NEW ModeGame(), 1, "ModeGame");

	// FPS�����肳���邽�߂̃N���X��������
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

