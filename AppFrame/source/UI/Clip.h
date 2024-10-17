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
	int _mixHandle;

	/*
		mWidthとmHeightがクリップする矩形サイズ
	*/

	class Empty* _child;

	std::vector<UI*>_grandChildren;

};