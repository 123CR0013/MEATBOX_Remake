#pragma once
#include"UI.h"
#include<algorithm>
#include"../ScriptEngine/StringUtil.h"

//描画位置
enum class StringDrawType {
	kLeft,
	kRight,
	kCenter
};

/*
	mWidthとmHeightは描画する範囲のオフセット値だったが、このクラスでは描画する文字の大きさとして扱う
*/

class Text :public UI {
public:
	Text(class UIScreen* owner,unsigned int order = 100)
		:UI(owner,order)
		,_drawType(StringDrawType::kLeft)
		,_color(GetColor(255,0,0))
		,_textSize(20.f)
	{
	}
	virtual ~Text(){}

	virtual void Draw()override;
private:
	std::string OrganizeLeftText() {
		//横に描画できる文字のサイズ
		size_t columSize = static_cast<size_t>(_width / _textSize);

		std::string rText;

		for (size_t front = 0; front < _text.length();)
		{
			size_t breakPos = _text.substr(front).find_first_of("\n");

			//mTextのコピーサイズ
			size_t copySize = columSize ;

			bool isFound = false;

			//
			if (0 < breakPos && breakPos < columSize) { copySize = breakPos; isFound = true; }

			if (_text.length() - front < copySize) { copySize = _text.length() - front; }

			//コピーする最後の文字が最大で2バイト、サイズが奇数なら、改行で切れないようにする
			else  
			{
				size_t pos = 0;
				for (pos = 0; pos < copySize;)
				{
					pos += StringUtility::Check(_text[front + pos]);
				}

				copySize = pos > copySize ? columSize - 1 : copySize;
			}

			rText += _text.substr(front, copySize) + "\n";

			//\n分飛ばす
			if (isFound)
			{
				front++;
			}

			/*空白の挿入*/
			for (size_t i = 0; i < columSize - copySize; ++i)
			{
				rText.insert(rText.length() - 1, " ");
			}

			front += copySize;
		}

		return rText;
	}



public:
	void SetText(const std::string& text) { _text = text; }
	void SetTextDrawType(const StringDrawType type) { _drawType = type; }
	void SetColor(unsigned char r, unsigned char g, unsigned char b) { _color = GetColor(r, g, b); }
	void SetTextSize(float textSize) { _textSize = textSize; }

	std::string GetText()const { return _text; }
protected:
	std::string _text;
	StringDrawType _drawType;
	unsigned int _color;		//描画する文字の色
	float _textSize;			//1バイトの文字の大きさ
};