#include "AnimationInfo.h"

AnimationInfo::AnimationInfo()
{
	_graphHandle.clear();
	_drawTbl.clear();
	_tblNum = 0;
	_framePerSheet = 0;
}

AnimationInfo::~AnimationInfo()
{
	_graphHandle.clear();
	_drawTbl.clear();
}
