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

#include "BeamStand.h"
#include "Sticky.h"
#include "StickyGroup.h"

#include "Effect.h"

#include "Animation.h"

#include <fstream>
#include <sstream>

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	_mapData = new Map(this);
	CreateMap();

	_plStepCnt = 0;

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

	DrawDebug();

	return true;
}

bool ModeGame::DrawDebug()
{
	//DrawFormatString(0, 100, COLOR_WHITE, "StepCnt: %d", _plStepCnt);


	_mapData->DrawDebug();
	for(auto& object : _objects) {
		object->DrawDebug();
	}

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

		// ÉãÅ[ÉgèÓïÒÇéÊìæ
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
			else if (name == "BeamStand")
			{
				int dir = -1;

				nlohmann::json properties = object.at("properties");
				for(auto& prop : properties)
				{
					if (prop.at("name") == "Direction") {
						dir = prop.at("value");
						break;
					}
				}
				
				std::array<Vector3, 5> dirTbl = {
					Vector3(0, 0, 0),
					Vector3(0, -1, 0),
					Vector3(0, 1, 0),
					Vector3(-1, 0, 0),
					Vector3(1, 0, 0),
				};

				CreateBeamStand(pos, dirTbl[dir + 1]);
			}
			else if (name == "Sticky")
			{
				CreateSticky(pos);
			}
		}
	}
}

void ModeGame::CreatePlayer(Vector3 vPos)
{
	Player* player = new Player(this);
	player->SetPos(vPos);
	player->SetDrawOffset(Vector3(0, -0.4f, 0));
	std::vector<int> plDrawTbl = { 0, 1, 2, 2, 2, 1 };

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/ch_girl_default.png"));
	animInfo->_framePerSheet = 10;
	//animInfo->_drawTbl = plDrawTbl;
	player->AddAnimInfo(animInfo);
	player->SetAnimSize(160, 160);

	//AnimationInfo* animInfo = new AnimationInfo();
	//animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/chararight_a.png"));
	//animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/chararight_b.png"));
	//animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/chararight_c.png"));
	//animInfo->_framePerSheet = 10;
	//animInfo->_drawTbl = plDrawTbl;
	//player->AddAnimInfo(animInfo);

	//AnimationInfo* animInfo2 = new AnimationInfo();
	//animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/charaleft_a.png"));
	//animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/charaleft_b.png"));
	//animInfo2->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/Player/charaleft_c.png"));
	//animInfo2->_framePerSheet = 10;
	//animInfo2->_drawTbl = plDrawTbl;
	//player->AddAnimInfo(animInfo2);

	_objects.push_back(player);
}

void ModeGame::CreateMeatBox(Vector3 vPos)
{
	MeatBox* meatBox = new MeatBox(this);
	meatBox->SetPos(vPos);
	meatBox->SetDrawOffset(Vector3(0, -0.2f, 0));
	std::vector<int> mbDrawTbl = { 0, 1, 2, 2, 2, 1, 0 };

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/MeatBox/ch_meatbox_default.png"));
	animInfo->_framePerSheet = 10;
	//animInfo->_drawTbl = mbDrawTbl;
	meatBox->AddAnimInfo(animInfo);

	//AnimationInfo* animInfo = new AnimationInfo();
	//animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/MeatBox/Meat_a.png"));
	//animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/MeatBox/Meat_b.png"));
	//animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Character/MeatBox/Meat_c.png"));
	//animInfo->_framePerSheet = 10;
	//animInfo->_drawTbl = mbDrawTbl;
	//meatBox->AddAnimInfo(animInfo);

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

void ModeGame::CreateBeamStand(Vector3 vPos, Vector3 vDir)
{
	BeamStand* beamStand = new BeamStand(this);
	beamStand->SetPos(vPos);
	beamStand->SetDir(vDir);

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Gimmick/Beam/obj1_beam_01.png"));
	animInfo->_framePerSheet = 1;
	beamStand->AddAnimInfo(animInfo);

	float angle = 0.0f;
	if(vDir == Vector3(0, -1, 0)) {
		angle = PI;
	}
	else if (vDir == Vector3(0, 1, 0)) {
		angle = 0.0f;
	}
	else if (vDir == Vector3(-1, 0, 0)) {
		angle = PI / 2.0f;
	}
	else if (vDir == Vector3(1, 0, 0)) {
		angle = PI * 3.0f / 2.0f;
	}
	beamStand->SetAnimAngle(angle);

	_objects.push_back(beamStand);
}

void ModeGame::CreateSticky(Vector3 vPos)
{
	Sticky* sticky = new Sticky(this);
	sticky->SetPos(vPos);
	sticky->SetDrawOffset(Vector3(0, -0.2f, 0));

	AnimationInfo* animInfo = new AnimationInfo();
	animInfo->_graphHandle.push_back(ResourceServer::LoadGraph("res/Gimmick/Sticky/Sticky.png"));
	animInfo->_framePerSheet = 10;
	sticky->AddAnimInfo(animInfo);

	StickyGroup* stickyGroup = new StickyGroup(this);
	stickyGroup->AddSticky(sticky);

	_objects.push_back(sticky);
	_objects.push_back(stickyGroup);
}

