#pragma once
#include "appframe.h"
#include "MapChip.h"

#include <array>
#include <vector>
#include <string>
#include <map>

#include <nlohmann/json.hpp>

// チップのサイズ
#define	CHIP_W		(100)
#define	CHIP_H		(100)

// マップのサイズ（1辺のチップ数）
#define	MAP_W		(20)
#define	MAP_H		(11)

class GameObject;

class Map
{
public:
	Map(ModeBase* mode);
	virtual ~Map();

	void CreateMap(nlohmann::json json);

	void Process();
	void Draw();
	void DrawDebug();
	void AnimProcess();

	MapChip* GetMapChip(int x, int y);
	MapChip* GetMapChip(Vector3 pos);

	GameObject* GetGameObject(int x, int y);
	GameObject* GetGameObject(Vector3 pos);


	void SetGameObject(GameObject* object, int x, int y);
	void SetGameObject(GameObject* object, Vector3 pos);

	void EraseGameObject(GameObject* object);


	// スクリーン座標をマップ座標に変換
	static Vector3 ConvScreenPosToMapPos(Vector3 vScreenPos);


protected:
	ModeBase* _mode;

	std::array<MapChip*, MAP_W * MAP_H> _mapChips;

	std::array<GameObject*, MAP_W * MAP_H> _objects;







	static void LoadChipCSV();
	struct LOADCHIP
	{
		MapChip::TYPE chipType;
		std::string fileName;
		int animNum;
	};

	static std::unordered_map<int, LOADCHIP> _loadChipData;

	std::array<int, MAP_W * MAP_H> LoadMapData(nlohmann::json json);
};