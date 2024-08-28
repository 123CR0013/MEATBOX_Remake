#include"InputText.h"

constexpr size_t CAN_INPUT_STR_SIZE = 1000;

InputText::InputText(UIScreen* owner, unsigned int order)
	:Text(owner)
	,_isInputing(false)
{
	_handle = MakeKeyInput(CAN_INPUT_STR_SIZE,FALSE,TRUE,FALSE);
}

InputText::~InputText()
{
	DeleteKeyInput(_handle);
}

void InputText::Process()
{
	if (IsLeftClickKey())
	{
		//���͂���n���h�����w��
		SetActiveKeyInput(_handle);

		//���ɓ��͂���Ă��镶������ɓ��͂��Ă���
		SetKeyInputString(_text.c_str(), _handle);

		_isInputing = true;
	}

	if (_isInputing)
	{
		if (!OnMouse() && GetMouseInput() & MOUSE_INPUT_LEFT || CheckKeyInput(_handle) != 0)
		{
			char* getText = new char[CAN_INPUT_STR_SIZE];
			GetKeyInputString(getText, _handle);
			_isInputing = false;
			_text = getText;
			delete[] getText;
		}
	}
}

void InputText::Draw()
{
	if (_isInputing)
	{
		auto leftTop = GetVertexes()[1];

		// ���͓r���̕������`��
		DrawKeyInputString(leftTop.x, leftTop.y, _handle);
	}
	else
	{
		Text::Draw();
	}
}