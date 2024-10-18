#include "Map.h"
#include "MapChip.h"

#include "GameObject.h"

#include <fstream>
#include <sstream>

std::unordered_map<int, Map::LOADCHIP> Map::_loadChipData;

Map::Map(ModeBase* mode)
{
	_mode = mode;
	_mapChips.fill(nullptr);
	_objects.fill(nullptr);
}

Map::~Map()
{
	for (auto& mapChip : _mapChips)
	{
		if (mapChip == nullptr) continue;
		delete mapChip;
	}
	_mapChips.fill(nullptr);
}

void Map::CreateMap(nlohmann::json json)
{
	LoadChipCSV();
	std::array<int, MAP_W* MAP_H> mapData = LoadMapData(json);

	for (int y = 0; y < MAP_H; y++)
	{
		for (int x = 0; x < MAP_W; x++)
		{
			int chip = mapData[y * MAP_W + x];

			if (chip == 0)
			{
				continue;
			}

			MapChip* mapChip = new MapChip(_mode);
			mapChip->SetPos(VGet(x , y, 0));

			LOADCHIP loadChip = _loadChipData[chip];
			mapChip->SetChipType(loadChip.chipType);
			
			AnimationInfo* animInfo = new AnimationInfo();
			ResourceServer::LoadDivGraph(loadChip.fileName, loadChip.animNum, loadChip.animNum, 1, 100, 100, animInfo->_graphHandle);
			animInfo->_framePerSheet = 10;
			mapChip->AddAnimInfo(animInfo);

			_mapChips[y * MAP_W + x] = mapChip;
		}
	}
}

void Map::Process()
{
	for (auto& mapChip : _mapChips)
	{
		if(mapChip == nullptr) continue;
		mapChip->AnimProcess();
	}
}

void Map::Draw()
{
	for (auto& mapChip : _mapChips)
	{
		if (mapChip == nullptr) continue;
		mapChip->Draw();
	}
}

void Map::DrawDebug()
{
	//for(auto& mapChip : _mapChips)
	//{
	//	if (mapChip == nullptr) continue;
	//	mapChip->DrawDebug();
	//}

	for(auto& object : _objects)
	{
		if (object == nullptr) continue;
		GameObject::TYPE type = object->GetType();
		std::array < std::string, 8> typeTbl = {
			"NONE",
			"PLAYER",
			"MEATBOX",
			"ENEMY",
			"BEAMSTAND",
			"BEAM_BODY",
			"STICKY",
			"STICKYGROUP"
		};
		DrawFormatString(object->GetPos().x * CHIP_W, object->GetPos().y * CHIP_H, COLOR_RED, typeTbl[static_cast<int>(type)].c_str());
	}
}

MapChip* Map::GetMapChip(int x, int y)
{
	if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
	{
		return nullptr;
	}
	return _mapChips[y * MAP_W + x];
}

MapChip* Map::GetMapChip(Vector3 pos)
{
	return GetMapChip(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

GameObject* Map::GetGameObject(int x, int y)
{
	if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
	{
		return nullptr;
	}
	return _objects[y * MAP_W + x];
}

GameObject* Map::GetGameObject(Vector3 pos)
{
	return GetGameObject(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void Map::SetGameObject(GameObject* object, int x, int y)
{
	if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
	{
		return;
	}

	EraseGameObject(object);

	_objects[y * MAP_W + x] = object;
}

void Map::SetGameObject(GameObject* object, Vector3 pos)
{
	SetGameObject(object, static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void Map::EraseGameObject(GameObject* object)
{
	for (int i = 0; i < MAP_W * MAP_H; i++) {
		if (_objects[i] == object) {
			_objects[i] = nullptr;
			break;
		}
	}
}

Vector3 Map::ConvScreenPosToMapPos(Vector3 vScreenPos)
{
	Vector3 vMapPos;
	vMapPos.x = vScreenPos.x / CHIP_W;
	vMapPos.x = floor(vMapPos.x);
	vMapPos.y = vScreenPos.y / CHIP_H;
	vMapPos.y = floor(vMapPos.y);
	vMapPos.z = 0;

	return vMapPos;
}

void Map::LoadChipCSV()
{
	if (!(_loadChipData.empty()))
	{
		return;
	}

	std::ifstream ifs("data/MapChip/MapChip.csv");
	if (!ifs)
	{
		return;
	}

	std::string str;
	while (std::getline(ifs, str))
	{
		std::vector<std::string> strArray;
		std::string token;
		std::istringstream stream(str);
		while (std::getline(stream, token, ','))
		{
			strArray.push_back(token);
		}

		LOADCHIP loadChip;
		loadChip.chipType = static_cast<MapChip::TYPE>(std::stoi(strArray[1]) + 1);
		std::array<std::string, 3> folderPath = {
			"res/Mapchip/base/", 
			"res/Mapchip/wall/",
			"res/Mapchip/hole/"
		};
		loadChip.fileName = folderPath[std::stoi(strArray[1])] + strArray[2] + ".png";
		loadChip.animNum = std::stoi(strArray[3]);

		_loadChipData.insert(std::make_pair(std::stoi(strArray[0]), loadChip));
	}
}

std::array<int, MAP_W * MAP_H> Map::LoadMapData(nlohmann::json json)
{
	std::array<int, MAP_W* MAP_H> mapData;
	mapData.fill(0);

	nlohmann::json data = json.at("data");
	int i = 0;
	for (auto& value : data)
	{
		mapData[i] = value;
		i++;
	}

	return mapData;
}


