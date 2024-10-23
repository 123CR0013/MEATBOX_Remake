#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeGameOver.h"
#include "ResultScreen.h"
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

#include "TutorialObject.h"

#include "Animation.h"

#include <fstream>
#include <sstream>

#include"MainScreen.h"

ModeGame::ModeGame(int worldID, int stageID)
{
	_worldID = worldID;
	_stageID = stageID;
	_bGameOver = false;
	_mapData = nullptr;
	_player = nullptr;
	_enMoveCnt = 0;
	_bResult = false;
	_resultData.remainingStepCnt = 0;
	_resultData.killCntMax = 0;

	_bPause = false;
}

bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	_bGameOver = false;
	_mapData = nullptr;
	_player = nullptr;
	_enMoveCnt = 0;
	_bResult = false;
	_resultData.remainingStepCnt = 0;
	_resultData.killCntMax = 0;

	_mapData = new Map(this);
	CreateMap();

	_enMoveCnt = 0;

	NEW MainScreen(this);

	_bResult = false;

	_bPause = false;

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();

	delete _mapData;
	delete _player;
	for(auto& object : _objects) {
		delete object;
	}
	_objects.clear();

	for (auto& object : _objectsToAdd) {
		delete object;
	}
	_objectsToAdd.clear();

	_objectsToRemove.clear();

	return true;
}

bool ModeGame::Process() {
	if (!_bPause) {
		if (global._trg & PAD_INPUT_1) {
			Terminate();
			Initialize();
		}
	}

	base::Process();

	CheckObjectsToRemove();
	CheckObjectsToAdd();

	if (!_bPause) {


		_mapData->Process();

		_player->ProcessInit();
		for (auto& object : _objects) {
			object->ProcessInit();
		}

		if (!_player->IsMove())_player->Process();
		_player->ProcessFinish();
		_player->ProcessChildObjects();
		//_player->AnimProcess();

		for (auto& object : _objects) {
			object->ProcessInit();
			if (!object->IsMove())object->Process();
			object->ProcessFinish();
			object->ProcessChildObjects();
			//object->AnimProcess();
		}

		if (_enMoveCnt >= 2) {
			_enMoveCnt = 0;
		}

		if (!_player->IsMove()) {
			// クリア判定（敵が残っていなかったらクリア）
			bool bEnemyExist = false;
			for (auto& object : _objects) {
				if (object->GetType() == GameObject::TYPE::ENEMY) {
					bEnemyExist = true;
					break;
				}
			}

			// クリア画面を表示
			if (!bEnemyExist && !_bResult) {
				_bResult = true;
				NEW ResultScreen(this);

				_bPause = true;
			}
		}
	}

	return true;
}

bool ModeGame::Render() {

	_mapData->Draw();

	// 描画順に並び替え
	std::multimap<int, Animation*> drawObjects;
	SortAnimationInDrawOrder(drawObjects, _player);
	for (auto& object : _objects) {
		SortAnimationInDrawOrder(drawObjects, object);
	}

	for (auto& object : drawObjects) {
		object.second->Draw();
	}

	base::Render();

	_mapData->AnimProcess();

	_player->AnimProcess();
	for (auto& object : _objects) {
		object->AnimProcess();
	}

	Animation::VibrationProcess();

	//DrawDebug();

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

void ModeGame::ChangeStage(int worldID, int stageID)
{
	_worldID = worldID;
	_stageID = stageID;

	Terminate();
	Initialize();
}

void ModeGame::AddGameObject(GameObject* object)
{
	_objectsToAdd.push_back(object);
}

void ModeGame::RemoveGameObject(GameObject* object)
{
	_objectsToRemove.push_back(object);
}

void ModeGame::CheckKillCnt(int killCnt)
{
	_resultData.killCntMax = MyMath::Max(_resultData.killCntMax, killCnt);
}

void ModeGame::SetGameOver()
{
	_bGameOver = true;
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

void ModeGame::LoadAnimData(Animation* animationClass, std::string name)
{
	if (animationClass == nullptr) return;

	if (!_bExistAnimData) 
	{
		std::ifstream ifs("data/Animation/AnimationData.json");
		if (ifs)
		{
			ifs >> _animDataJson;
			_bExistAnimData = true;
		}
		else
		{
			MessageBox(NULL, "アニメーションデータの読み込みに失敗しました", NULL, MB_OK);
		}
	}

	if (_bExistAnimData) 
	{
		for (auto& character : _animDataJson)
		{
			std::string charaName = character.at("Name");
			if (charaName != name) continue;

			nlohmann::json anims = character.at("Animation");
			for (auto& anim : anims)
			{
				AnimationInfo* animInfo = new AnimationInfo();

				std::string fileName = anim.at("FileName");
				ResourceServer::LoadDivGraph("res/" + fileName, anim.at("Num"), anim.at("Num"), 1, anim.at("Width"), anim.at("Height"), animInfo->_graphHandle);
				animInfo->_framePerSheet = anim.at("Frame");

				nlohmann::json drawTbl = anim.at("DrawTable");
				std::vector<int> drawTable;
				for (auto& tbl : drawTbl)
				{
					drawTable.push_back(tbl);
				}
				if (drawTable.size() != 0 && drawTable.at(0) != -1) 
				{
					animInfo->_drawTbl = drawTable;
				}

				animationClass->AddAnimInfo(animInfo, anim.at("Group"));				
			}

			break;
		}
	}
}

void ModeGame::CreateMap()
{
	std::string path = "data/Map/stage_" + std::to_string(_worldID) + "_" + std::to_string(_stageID) + ".json";
	std::ifstream ifs(path);
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
			else if (name == "Tutorial")
			{
				int id = -1;

				nlohmann::json properties = object.at("properties");
				for (auto& prop : properties)
				{
					if (prop.at("name") == "InfoNum") {
						id = prop.at("value");
						break;
					}
				}

				CreateTutorial(pos, id);
			}
		}
	}
}

void ModeGame::CreatePlayer(Vector3 vPos)
{
	Player* player = new Player(this);
	player->SetPos(vPos);

	Animation* anim = player->AddAnimationClass();
	anim->SetDrawOffset(Vector3(0, -0.5f, 0));
	anim->SetSize(160, 160);
	anim->SetDrawOrder(5);
	LoadAnimData(anim, "Player");

	//_objects.push_back(player);
	_player = player;
}

void ModeGame::CreateMeatBox(Vector3 vPos)
{
	MeatBox* meatBox = new MeatBox(this);
	meatBox->SetPos(vPos);

	Animation* anim = meatBox->AddAnimationClass();
	anim->SetDrawOffset(Vector3(0, -0.2f, 0));
	anim->SetDrawOrder(2);
	LoadAnimData(anim, "Meatbox");

	_objects.push_back(meatBox);
}

void ModeGame::CreateEnemy(Vector3 vPos)
{
	Enemy* enemy = new Enemy(this);
	enemy->SetPos(vPos);

	Animation* anim = enemy->AddAnimationClass();
	anim->SetDrawOffset(Vector3(0, -0.2f, 0));
	anim->SetDrawOrder(2);
	LoadAnimData(anim, "Namako");

	_objects.push_back(enemy);
}

void ModeGame::CreateEnemyTomato(std::vector<Vector3> route)
{
	EnemyTomato* enemyTomato = new EnemyTomato(this);
	enemyTomato->SetPos(route[0]);
	enemyTomato->SetMoveRoute(route);

	Animation* anim = enemyTomato->AddAnimationClass();
	anim->SetDrawOffset(Vector3(0, -0.2f, 0));
	anim->SetDrawOrder(2);
	LoadAnimData(anim, "Tomato");

	_objects.push_back(enemyTomato);


	Effect* arrow = new Effect(this);
	arrow->SetLoop(true);
	std::string path = "res/Effect/MoveArea/";
	std::array<std::string, 4> fileName = {
		"effect_movearea_01",
		"effect_movearea_02",
	};

	Animation* arrowAnim = arrow->AddAnimationClass();
	arrowAnim->SetDrawOrder(DRAW_ORDER_UNDERLAP_OBJECT);
	for (int i = 0; i < 2; i++) {
		AnimationInfo* animInfo = new AnimationInfo();
		animInfo->_graphHandle.push_back(ResourceServer::LoadGraph(path + fileName[i] + ".png"));
		animInfo->_framePerSheet = 10;
		arrowAnim->AddAnimInfo(animInfo, i);
	}

	enemyTomato->AddChildObject(arrow);

	enemyTomato->Init();
}

void ModeGame::CreateBeamStand(Vector3 vPos, Vector3 vDir)
{
	BeamStand* beamStand = new BeamStand(this);
	beamStand->SetPos(vPos);
	beamStand->SetDir(vDir);

	{
		Animation* anim = beamStand->AddAnimationClass();
		anim->SetDrawOrder(2);
		LoadAnimData(anim, "BeamStand");
	}

	int dir = 0;
	if (vDir.x < 0) {
		dir = 2;
	}
	else if (vDir.x > 0) {
		dir = 3;
	}
	else if (vDir.y < 0) {
		dir = 0;
	}
	else if (vDir.y > 0) {
		dir = 1;
	}

	std::array<Vector3, 4> drawOffsetTbl = {
		Vector3(0, -0.5f, 0),
		Vector3(0,  0.5f, 0),
		Vector3(-0.5f, -0.15f, 0),
		Vector3( 0.5f, -0.15f, 0),
	};

	std::array<std::string, 2> name = {
		"BeamOrb_Pink",
		"BeamOrb_White",
	};
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++) {
			Animation* anim = beamStand->AddAnimationClass();
			anim->SetDrawOffset(drawOffsetTbl.at(dir));
			anim->SetSize(30, 30);
			anim->SetDrawOrder(19 + j);
			LoadAnimData(anim, name.at(j));
		}
	}

	_objects.push_back(beamStand);
}

void ModeGame::CreateSticky(Vector3 vPos)
{
	Sticky* sticky = new Sticky(this);
	sticky->SetPos(vPos);

	Animation* anim = sticky->AddAnimationClass();
	anim->SetDrawOffset(Vector3(0, -0.2f, 0));
	anim->SetDrawOrder(0);
	LoadAnimData(anim, "Sticky");

	StickyGroup* stickyGroup = new StickyGroup(this);
	stickyGroup->AddSticky(sticky);

	_objects.push_back(sticky);
	_objects.push_back(stickyGroup);
}

void ModeGame::CreateTutorial(Vector3 vPos, int id)
{
	TutorialObject* tutorial = new TutorialObject(this, id);
	tutorial->SetPos(vPos);

	Animation* anim = tutorial->AddAnimationClass();
	anim->SetDrawOffset(Vector3(0, 0.0f, 0));
	anim->SetDrawOrder(0);
	LoadAnimData(anim, "Tutorial");

	_objects.push_back(tutorial);
}

// 描画順序をソート
// Animationクラス単位でソート
void ModeGame::SortAnimationInDrawOrder(std::multimap<int, Animation*>& result, GameObject* gameObject)
{
	std::vector<Animation*> anims = gameObject->GetAllAnimationClass();
	for (auto& anim : anims)
	{
		// ソート値を計算
		float sortValue = gameObject->GetPos().y + anim->GetDrawOffset().y;
		// マップチップ座標に統一する
		if (anim->GetDrawWithScreenPos()) sortValue / 100.0f;
		sortValue *= 10.0f;
		// 補正値を加算
		sortValue += anim->GetDrawOrder();

		// リザルトに追加
		result.insert(std::make_pair(static_cast<int>(sortValue), anim));
	}

	// 子オブジェクトもソート
	for (auto& child : gameObject->GetChildObjects())
	{
		SortAnimationInDrawOrder(result, child);
	}
}

