#include"ModeUI.h"
#include"../UI/UIScreen.h"

ModeUI::ModeUI()
{

}

ModeUI::~ModeUI()
{
	Terminate();
}

bool ModeUI::Initialize()
{
	return true;
}

bool ModeUI::Terminate() {

	for (auto&& screen : _UIScreens)
	{
		delete screen;
	}
	_UIScreens.clear();

	return true;
}

bool ModeUI::Process() {
	ModeBase::Process();

	if (!_addUIScreens.empty()) {
		for (auto&& addUIScreens : _addUIScreens) {
			_UIScreens.emplace_back(addUIScreens);
		}
		_addUIScreens.clear();
	}

	if (!_deleteUIScreens.empty()) {
		for (auto&& removeUIScreens : _deleteUIScreens) {
			auto iter = std::find(_UIScreens.begin(), _UIScreens.end(), removeUIScreens);
			if (iter != _UIScreens.end()) {
				_UIScreens.erase(iter);
				delete removeUIScreens;
			}
		}
		_deleteUIScreens.clear();
	}

	if (!_UIScreens.empty()) {
		_UIScreens.back()->Update();
	}
	return true;
}

bool ModeUI::Render() {
	//オブジェクトを巡回処理
	ModeBase::Render();

	for (auto&& UIScreen : _UIScreens) {
		UIScreen->Draw();
	}

	return true;
}

void ModeUI::AddUIScreen(UIScreen* screen) {
	//既に追加されているか
	auto itr = std::find(_UIScreens.begin(), _UIScreens.end(), screen);
	if (itr != _UIScreens.end()) {
		return;
	}
	//まだ、追加されていないだけで予約は入っているのでは？
	itr = std::find(_addUIScreens.begin(), _addUIScreens.end(), screen);
	if (itr != _addUIScreens.end()) {
		return;
	}

	_addUIScreens.emplace_back(screen);
}

void ModeUI::DeleteUIScreen(UIScreen* screen) {
	//既に削除予約されているか
	auto iter = std::find(_deleteUIScreens.begin(), _deleteUIScreens.end(), screen);
	if (iter != _deleteUIScreens.end()) {
		return;
	}

	iter = std::find(_addUIScreens.begin(), _addUIScreens.end(), screen);
	if (iter != _addUIScreens.end()) {
		_addUIScreens.erase(iter);
		delete (*iter);
		return;
	}
	_deleteUIScreens.emplace_back(screen);
}