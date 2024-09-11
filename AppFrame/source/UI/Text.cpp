#include"Text.h"

void Text::Draw()
{

	std::string text;

	switch (_drawType)
	{
	case StringDrawType::kLeft:
		text = OrganizeLeftText();
		break;

	case StringDrawType::kRight:
		//text = OrganizeRightText();
		break;
	}

	auto _pos = GetVertexes();

	size_t columSize = static_cast<size_t>(_width / _textSize);

	size_t lineSize = 0;
	{	size_t pos = text.find("\n");
	while (pos != std::string::npos)
	{
		pos++;
		pos = text.find("\n", pos);
		lineSize++;
	}
	}

	lineSize = (size_t)((float)(text.length() - lineSize) / (float)columSize + 0.99999f);
	lineSize = lineSize * _textSize < _height ? (size_t)(_height / _textSize + 0.99999f) : lineSize;

	size_t lineNum = 0;
	size_t columNum = 0;

	Vector2 drawAreaLeftBottom = Vector2::Lerp(_pos[0], _pos[3], ((float)lineSize * _textSize) / _height);
	Vector2 drawAreaRightBottom = Vector2::Lerp(_pos[1], _pos[2], ((float)lineSize * _textSize) / _height);

	for (size_t pos = 0; pos + 1 < text.length();)
	{
		//整えてあるので、改行するまで横に描画していく
		if (text.substr(pos, 1) == "\n") { pos++; lineNum++; columNum = 0; continue; }

		//表示するバイト数
		size_t size = StringUtility::Check(text[pos]);

		Vector2 lineRightTop = Vector2::Lerp(_pos[1], drawAreaRightBottom, (float)lineNum / (float)lineSize);
		Vector2 lineRightBottom = Vector2::Lerp(_pos[1], drawAreaRightBottom, ((float)(lineNum + 1) / (float)lineSize));

		Vector2 lineLeftTop = Vector2::Lerp(_pos[0], drawAreaLeftBottom, (float)lineNum / (float)lineSize);
		Vector2 lineLeftBottom = Vector2::Lerp(_pos[0], drawAreaLeftBottom, ((float)(lineNum + 1) / (float)lineSize));

		Vector2 leftTop = Vector2::Lerp(lineLeftTop, lineRightTop, (float)columNum / (float)columSize);
		Vector2 leftBottom = Vector2::Lerp(lineLeftBottom, lineRightBottom, (float)(columNum) / (float)columSize);

		Vector2 rightTop = Vector2::Lerp(lineLeftTop, lineRightTop, (float)(columNum + size) / (float)columSize);
		Vector2 rightBottom = Vector2::Lerp(lineLeftBottom, lineRightBottom, (float)(columNum + size) / (float)columSize);

		DrawModiFormatStringF
		(
			leftTop.x, leftTop.y,
			rightTop.x, rightTop.y,
			rightBottom.x, rightBottom.y,
			leftBottom.x, leftBottom.y,
			_color,
			_color,
			0,
			"%s",
			text.substr(pos, size).c_str()
		);

		//auto color = GetColor(255, 255, 255);

		//DrawCircleAA(leftTop.x, leftTop.y, 5.f, 5, color);
		//DrawCircleAA(rightTop.x, rightTop.y, 5.f, 5, color);
		//DrawCircleAA(rightBottom.x, rightBottom.y, 5.f, 5, color);
		//DrawCircleAA(leftBottom.x, leftBottom.y, 5.f, 5, mColor);
		pos += size;
		columNum += size;
	}

	columNum = 0;

	//for (size_t pos = 0; pos + 1 < text.length();)
	//{
	//	//整えてあるので、改行するまで横に描画していく
	//	if (text.substr(pos, 1) == "\n") { pos++; lineNum++; columNum = 0; continue; }

	//	//表示するバイト数
	//	size_t size = StringUtility::Check(text[pos]);

	//	Vector2 lineRightTop = Vector2::Lerp(_pos[0], drawAreaLeftBottom, (float)lineNum / (float)lineSize);
	//	Vector2 lineRightBottom = Vector2::Lerp(_pos[0], drawAreaLeftBottom, ((float)(lineNum + 1) / (float)lineSize));

	//	Vector2 lineLeftTop = Vector2::Lerp(_pos[1], drawAreaRightBottom, (float)lineNum / (float)lineSize);
	//	Vector2 lineLeftBottom = Vector2::Lerp(_pos[1], drawAreaRightBottom, ((float)(lineNum + 1) / (float)lineSize));

	//	Vector2 leftTop = Vector2::Lerp(lineLeftTop, lineRightTop, (float)columNum / (float)columSize);
	//	Vector2 leftBottom = Vector2::Lerp(lineLeftBottom, lineRightBottom, (float)(columNum) / (float)columSize);

	//	Vector2 rightTop = Vector2::Lerp(lineLeftTop, lineRightTop, (float)(columNum + size) / (float)columSize);
	//	Vector2 rightBottom = Vector2::Lerp(lineLeftBottom, lineRightBottom, (float)(columNum + size) / (float)columSize);

	//	DrawCircleAA(leftTop.x, leftTop.y, 5.f, 5, mColor);
	//	DrawCircleAA(rightTop.x, rightTop.y, 5.f, 5, mColor);
	//	DrawCircleAA(rightBottom.x, rightBottom.y, 5.f, 5, mColor);
	//	DrawCircleAA(leftBottom.x, leftBottom.y, 5.f, 5, mColor);

	//	pos += size;
	//	columNum += size;
	//}

}