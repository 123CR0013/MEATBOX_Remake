#include "MapChip.h"
#include "DrawForMapChip.h"

#include "Map.h"

MapChip::MapChip(ModeBase* mode) : ObjectBase(mode)
{
	_chipType = TYPE::NONE;

	_vPos = { 0, 0, 0 };
	_vDrawOffset = { 0, 0, 0 };
	_anim = new Animation(this);
}

MapChip::~MapChip()
{
	delete _anim;
}

void MapChip::AnimProcess()
{
	_anim->Process();
}

void MapChip::Draw()
{
	_anim->Draw();
}

void MapChip::DrawDebug()
{
	std::array<std::string, 4> chipTypeStr = { "NONE", "FLOOR", "WALL", "HOLE" };
	int index = static_cast<int>(_chipType);

	DrawFormatString(_vPos.x * CHIP_W, _vPos.y * CHIP_H, COLOR_RED, "%s", chipTypeStr[index].c_str());
}
