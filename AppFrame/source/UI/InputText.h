#pragma once
#include"Text.h"

class InputText :public Text
{
public:
	InputText(class UIScreen* owner, unsigned int order = 100);
	virtual ~InputText();

	void Process()override;
	void Draw()override;
private:
	int _handle;
	bool _isInputing;
};