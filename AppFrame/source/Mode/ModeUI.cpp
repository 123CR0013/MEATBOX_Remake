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
	//�I�u�W�F�N�g�����񏈗�
	ModeBase::Render();

	for (auto&& UIScreen : _UIScreens) {
		UIScreen->Draw();
	}

	return true;
}

void ModeUI::AddUIScreen(UIScreen* screen) {
	//���ɒǉ�����Ă��邩
	auto itr = std::find(_UIScreens.begin(), _UIScreens.end(), screen);
	if (itr != _UIScreens.end()) {
		return;
	}
	//�܂��A�ǉ�����Ă��Ȃ������ŗ\��͓����Ă���̂ł́H
	itr = std::find(_addUIScreens.begin(), _addUIScreens.end(), screen);
	if (itr != _addUIScreens.end()) {
		return;
	}

	_addUIScreens.emplace_back(screen);
}

void ModeUI::DeleteUIScreen(UIScreen* screen) {
	//���ɍ폜�\�񂳂�Ă��邩
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