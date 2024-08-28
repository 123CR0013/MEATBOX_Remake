#pragma once
#include"UI.h"

class Clip final:public UI {
public:
	Clip(class UIScreen* owner, unsigned int order = 100);
	virtual ~Clip();

	void Process()override;
	void Draw()override;

	void AddChild(UI* ptr);

private:
	int _screenHandle;
	int _clipHandle;
	int _blendHandle;

	/*
		mWidth��mHeight���N���b�v�����`�T�C�Y
	*/

	class Empty* _child;

	std::vector<UI*>_grandChildren;

};