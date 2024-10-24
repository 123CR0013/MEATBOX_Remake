#pragma once
#include "DxLib.h"		// VECTOR‚âMATRIX‚ğ’è‹`‚Æ‚µ‚Äg‚¤‚½‚ßA.h‚É“ü‚ê‚Ä‚¨‚­
#include "../Collision/MyStructure.h"
#include "../Math/Vector3/Vector3.h"

namespace MyDraw {

	// ‰æ‘œ‚Ì’†S‚Å‰ñ“]‚µ‚Ä•`‰æ‚·‚é
	void MyDrawModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle);

	// ‰æ‘œ‚Ì’†S‚Å‰ñ“]‚µ‚Ä•`‰æ‚·‚é(¶‰E”½“]”Å)
	void MyDrawTurnModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle);

	void MyDrawModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle, int turn);
	void MyDrawModiGraph(MATRIX mView, Vector3 position, float zoom, float angle, int width, int height, int cgHandle, int turn);

	void MyDrawModiGraphWithScreenSize(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle, int turn);
	void MyDrawModiGraphWithScreenSize(MATRIX mView, Vector3 position, float zoom, float angle, int width, int height, int cgHandle, int turn);


	// 4“_‚ğw’è‚µ‚ÄlŠp‚ğ•`‚­
	void MyDraw4PointBox(MATRIX mView, VECTOR pos[4], unsigned int Color, int FillFlag);

	// ‹éŒ`‚ğ•`‚­
	void MyDrawBox(MATRIX mView, int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag);

	void MyDrawExtendGraph(MATRIX mView, int x1, int y1, int x2, int y2, unsigned int handle, int FillFlag);

	void MyDrawDeformationGraph(MATRIX mView, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float direction, int handle, int FillFlag);

	void MyDrawRect(MATRIX mView, MYRECT_PLUS rc, unsigned int Color, int FillFlag);

	void MyDrawLine(MATRIX mView, int x1, int y1, int x2, int y2, int Color);

	void MyDrawCircle(MATRIX mView, int x, int y, int r);

	// ˆÊ’u‚ğ‘µ‚¦‚Ä•¶š—ñ‚ğ•`‚­
	// posBaseX = -1:¶‘µ‚¦, 0:’†‰›, 1:‰E‘µ‚¦
	void DrawPosString(int x, int y, int posBaseX, unsigned int color, const char* str);

	//ƒLƒƒƒ‰‚¾‚¯‚ğ“h‚è‚Â‚Ô‚·
	void PortalIn(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int GrHandle, int Color, int turn, float param);

}