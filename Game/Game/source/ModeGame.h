#pragma once
#include "appframe.h"
#include <nlohmann/json.hpp>

class Map;
class GameObject;

// モード
class ModeGame : public ModeUI
{
	typedef ModeUI base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	virtual bool DrawDebug();

	Map* GetMapData() override { return _mapData; }

	void AddGameObject(GameObject* object) override;
	void RemoveGameObject(GameObject* object) override;

	void AddPlayerStepCnt() override { _plStepCnt++; }
	int GetPlayerStepCnt() override { return _plStepCnt; }

	void SetGameOver() override;

protected:
	Map* _mapData;
	std::vector<GameObject*> _objects;

	// 追加予約オブジェクトの確認
	void CheckObjectsToAdd();
	// 追加予約オブジェクト
	std::vector<GameObject*> _objectsToAdd;
	// 削除予約オブジェクトの確認
	void CheckObjectsToRemove();
	// 削除予約オブジェクト
	std::vector<GameObject*> _objectsToRemove;



	int _plStepCnt;

	void LoadAnimData(GameObject* gameObject, std::string name);
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

	bool _bResult = false;
};