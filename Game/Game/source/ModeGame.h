#pragma once
#include "appframe.h"
#include <nlohmann/json.hpp>

class Map;
class GameObject;
class Animation;

// モード
class ModeGame : public ModeUI
{
	typedef ModeUI base;
public:
	ModeGame(int worldID, int stageID);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	virtual bool DrawDebug();

	int GetWorldID() override { return _worldID; }
	int GetStageID() override { return _stageID; }
	void ChangeStage(int worldID, int stageID) override;

	ResultData GetResultData() override { return _resultData; }

	Map* GetMapData() override { return _mapData; }

	void AddGameObject(GameObject* object) override;
	void RemoveGameObject(GameObject* object) override;

	void AddPlayerStepCnt() override { 
		_resultData.remainingStepCnt--;
		_enMoveCnt++; 
	}
	int GetEnemyMoveCnt() override { return _enMoveCnt; }

	void CheckKillCnt(int killCnt) override;

	bool GetGameOver() override { return _bGameOver; }
	void SetGameOver() override;

protected:
	int _worldID, _stageID;

	bool _bGameOver;

	Map* _mapData;
	GameObject* _player;
	std::vector<GameObject*> _objects;

	// 追加予約オブジェクトの確認
	void CheckObjectsToAdd();
	// 追加予約オブジェクト
	std::vector<GameObject*> _objectsToAdd;
	// 削除予約オブジェクトの確認
	void CheckObjectsToRemove();
	// 削除予約オブジェクト
	std::vector<GameObject*> _objectsToRemove;


	// プレイヤーが移動するたびに1加算
	// 2になったら敵が移動する
	// Process()終了時に2以上なら0に戻す
	int _enMoveCnt;

	void LoadAnimData(Animation* animationClass, std::string name);
	bool _bExistAnimData = false;
	nlohmann::json _animDataJson;

	void CreateMap();
	void CreateGameObjects(nlohmann::json json);

	void CreatePlayer(Vector3 vPos);
	void CreateMeatBox(Vector3 vPos);
	void CreateEnemy(Vector3 vPos);
	void CreateEnemyTomato(std::vector<Vector3> route);
	void CreateBeamStand(Vector3 vPos, Vector3 vDir);
	void CreateSticky(Vector3 vPos);
	void CreateTutorial(Vector3 vPos, int id);

	bool _bResult;
	ResultData _resultData;

	// 描画順序をソート
	// Animationクラス単位でソート
	void SortAnimationInDrawOrder(std::multimap<int, Animation*>& result, GameObject* gameObject);

	bool _bPause;
};