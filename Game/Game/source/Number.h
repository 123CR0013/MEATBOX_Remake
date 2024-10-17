#pragma once
#include"appframe.h"

class Number :public UI
{
public:
	Number(UIScreen* owner,const std::string& fileName,UINT order = 100);
	virtual ~Number();

	void Draw()override;

	enum class Format
	{
		kCenter,	//’†‰›‘µ‚¦
		kLeft,		//¶‘µ‚¦
		kRight		//‰E‘µ‚¦
	};

	void SetNumber(UINT number){ _number = number; }
	void SetNumberWidth(float width) { _numberWidth = width; }
	void SetNumberHeight(float height) { _numberHeight = height; }
	void SetFormat(Format format) { _format = format; }

	std::array<Vector2, 4>GetLeftAligVertexes();
	std::array<Vector2, 4>GetRightAligVertexes();


private:
	UINT _number;
	std::array<int, 10>_numHandles;
	float _numberWidth;
	float _numberHeight;
	Format _format;
};