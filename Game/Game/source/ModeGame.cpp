#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeGameOver.h"
#include "map.h"

#include "Map.h"
#include "GameObject.h"
#include "Player.h"
#include "MeatBox.h"
#include "Enemy.h"
#include "EnemyTomato.h"
#include "Effect.h"

#include "Animation.h"

#include <fstream>
#include <sstream>

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	_mapData = new Map(this);
	CreateMap();

	_plStepCnt = 0;

	// 必ずプレイヤーを最初に作成する
	//CreatePlayer(Vector3(7, 8, 0));
	//CreatePlayer(Vector3(14, 12, 0));
	//CreateMeatBox(Vector3(10, 8, 0));
	//CreateMeatBox(Vector3(14, 9, 0));
	//CreateEnemy(Vector3(15, 9, 0));
	//CreateEnemy(Vector3(16, 9, 0));
	//CreateEnemy(Vector3(21, 8, 0));

	//CreateEnemyTomato(Vector3(20, 7, 0));

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();

	delete _mapData;
	for(auto& object : _objects) {
		delete object;
	}
	_objects.clear();

	return true;
}

bool ModeGame::Process() {
	base::Process();

	CheckObjectsToRemove();
	CheckObjectsToAdd();

	_mapData->Process();
	
	for(auto& object : _objects) {
		object->Process();
		object->ProcessChildObjects();
		object->AnimProcess();
	}

	if (_plStepCnt >= 2) {
		_plStepCnt = 0;
	}

	if (global._trg & PAD_INPUT_1) {
		Terminate();
		Initialize();
	}

	return true;
}

bool ModeGame::Render() {
	base::Render();

	_mapData->Draw();
	
	for(auto& object : _objects) {
		object->Draw();
	}

	//DrawFormatString(0, 100, COLOR_WHITE, "StepCnt: %d", _plStepCnt);
	//DrawDebug();

	return true;
}

bool ModeGame::DrawDebug()
{
	_mapData->DrawDebug();
	return true;
}

void ModeGame::AddGameObject(GameObject* object)
{
	_objectsToAdd.push_back(object);
}

void ModeGame::RemoveGameObject(GameObject* object)
{
	_objectsToRemove.push_back(object);
}

void ModeGame::SetGameOver()
{
	ModeServer::GetInstance()->Del(this);
	ModeServer::GetInstance()->Add(NEW ModeGameOver(), 1, "ModeGameOver");
}

void ModeGame::CheckObjectsToAdd()
{
	for (auto& object : _objectsToAdd)
	{
		_objects.push_back(object);
	}
	_objectsToAdd.clear();
}

void ModeGame::CheckObjectsToRemove()
{
	for (auto& object : _objectsToRemove)
	{
		auto itr = std::find(_objects.begin(), _objects.end(), object);
		if (itr != _objects.end())
		{
			delete *itr;
			_objects.erase(itr);
		}
	}
	_objectsToRemove.clear();
}

void ModeGame::CreateMap()
{
	std::ifstream ifs("data/Map/stagetest.json");
	if (ifs)
	{
		nlohmann::json json;
		ifs >> json;

		nlohmann::json layers = json.at("layers");
		int i = 0;
		for (auto& layer : layers)
		{
			if (i == 0) {
				_mapData->CreateMap(layer);
			}
			else {
				CreateGameObjects(layer);
			}
			i++;
		}
	}
}

void ModeGame::CreateGameObjects(nlohmann::json json)
{
	if (json.at("name") == "EnemyTomato") {

		// ルート情報を取得
		nlohmann::json objects = json.at("objects");
		std::map<int, Vector3> routeMap;
		for(auto& object : objects)
		{
			nlohmann::json properties = object.at("properties");
			int id = -1;
			for(auto& prop : properties)
			{
				if (prop.at("name") == "RouteID") {
					id = prop.at("value");
				}
			}

			if(id == -1) continue;

			Vector3 pos = Vector3(object.at("x"), object.at("y"), 0);
			pos = Map::ConvScreenPosToMapPos(pos);

			routeMap.insert(std::make_pair(id, pos));
		}

		std::vector<Vector3> route;
		for(auto& routeData : routeMap)
		{
			route.push_back(routeData.second);
		}

		CreateEnemyTomato(route);
	}
	else {
		nlohmann::json objects = json.at("objects");
		for(auto& object : objects)
		{
			std::string name = object.at("name");
			Vector3 pos = Vector3(object.at("x"), object.at("y"), 0);
			pos = Map::ConvScreenPosToMapPos(pos);

			if (name == "Player")
			{
				CreatePlayer(pos);
			}
			else if (name == "Meatbox")
			{
				CreateMeatBox(pos);
			}
			else if (name == "EnemyNamako")
			{
				CreateEnemy(pos);
			}
		}
	}
}

void ModeGame::CreatePlayer(Vector3 vPos)
{
	Player* player = new Player(this);
	player->SetPos(vPos);
	player->SetDrawOffset(Vector3(0, -0.2f, 0));
	std::vector<int> plDrawTbl = { 0, 1, 2, 2, 2, 1 };

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/chararight_a.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/chararight_b.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/chararight_c.png"));
	animInfo->_framePerSheet = 10;
	animInfo->_drawTbl = plDrawTbl;
	player->AddAnimInfo(animInfo);

	AnimationInfo* animInfo2 = new AnimationInfo();
	animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/charaleft_a.png"));
	animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/charaleft_b.png"));
	animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/charaleft_c.png"));
	animInfo2->_framePerSheet = 10;
	animInfo2->_drawTbl = plDrawTbl;
	player->AddAnimInfo(animInfo2);

	_objects.push_back(player);
}

void ModeGame::CreateMeatBox(Vector3 vPos)
{
	MeatBox* meatBox = new MeatBox(this);
	meatBox->SetPos(vPos);
	meatBox->SetDrawOffset(Vector3(0, -0.2f, 0));
	std::vector<int> mbDrawTbl = { 0, 1, 2, 2, 2, 1, 0 };

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/MeatBox/Meat_a.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/MeatBox/Meat_b.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/MeatBox/Meat_c.png"));
	animInfo->_framePerSheet = 10;
	animInfo->_drawTbl = mbDrawTbl;
	meatBox->AddAnimInfo(animInfo);

	_objects.push_back(meatBox);
}

void ModeGame::CreateEnemy(Vector3 vPos)
{
	Enemy* enemy = new Enemy(this);
	enemy->SetPos(vPos);
	enemy->SetDrawOffset(Vector3(0, -0.2f, 0));
	std::vector<int> enDrawTbl = { 0, 1, 2, 1 };

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Enemy/Namako/namako_a.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Enemy/Namako/namako_b.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Enemy/Namako/namako_c.png"));
	animInfo->_framePerSheet = 10;
	animInfo->_drawTbl = enDrawTbl;
	enemy->AddAnimInfo(animInfo);

	_objects.push_back(enemy);
}

void ModeGame::CreateEnemyTomato(std::vector<Vector3> route)
{
	EnemyTomato* enemyTomato = new EnemyTomato(this);
	enemyTomato->SetPos(route[0]);
	enemyTomato->SetDrawOffset(Vector3(0, -0.2f, 0));

	enemyTomato->SetMoveRoute(route);

	std::vector<int> enDrawTbl = { 0, 1, 2, 1 };

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Enemy/Tomato/tomato_a.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Enemy/Tomato/tomato_b.png"));
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Enemy/Tomato/tomato_c.png"));
	animInfo->_framePerSheet = 10;
	animInfo->_drawTbl = enDrawTbl;
	enemyTomato->AddAnimInfo(animInfo);

	_objects.push_back(enemyTomato);


	Effect* arrow = new Effect(this);
	arrow->SetLoop(true);
	std::string path = "res/Effect/Arrow/";
	std::array<std::string, 4> arrowColor = {
		"Yellow/Yellow_",
		"Red/Red_"
	};

	std::array<std::string, 4> arrowDir = {
		"Up",
		"Down",
		"Left",
		"Right"
	};

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			AnimationInfo* animInfo = new AnimationInfo();
			animInfo->_graphHandle.push_back(ResourceServer::LoadGraph(path + arrowColor[i] + arrowDir[j] + ".png"));
			animInfo->_framePerSheet = 10;
			arrow->AddAnimInfo(animInfo);
		}
	}

	enemyTomato->AddChildObject(arrow);

	enemyTomato->Init();
}

