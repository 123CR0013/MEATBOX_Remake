#pragma once
#include "appframe.h"
#include <nlohmann/json.hpp>

class Map;
class GameObject;
class Animation;

// ���U���g��ʂɓn���f�[�^
struct ResultData
{
	// �c�����
	int stepCnt;
	// �ő哯���L��
	int killCnt;
};

// ���[�h
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

	Map* GetMapData() override { return _mapData; }

	void AddGameObject(GameObject* object) override;
	void RemoveGameObject(GameObject* object) override;

	void AddPlayerStepCnt() override { _plStepCnt++; }
	int GetPlayerStepCnt() override { return _plStepCnt; }
	void CheckKillCnt(int killCnt) override;

	void SetGameOver() override;

protected:
	int _worldID, _stageID;

	Map* _mapData;
	GameObject* _player;
	std::vector<GameObject*> _objects;

	// �ǉ��\��I�u�W�F�N�g�̊m�F
	void CheckObjectsToAdd();
	// �ǉ��\��I�u�W�F�N�g
	std::vector<GameObject*> _objectsToAdd;
	// �폜�\��I�u�W�F�N�g�̊m�F
	void CheckObjectsToRemove();
	// �폜�\��I�u�W�F�N�g
	std::vector<GameObject*> _objectsToRemove;



	int _plStepCnt;

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

	bool _bResult;
	ResultData _resultData;

	// �`�揇�����\�[�g
	// Animation�N���X�P�ʂŃ\�[�g
	void SortAnimationInDrawOrder(std::multimap<int, Animation*>& result, GameObject* gameObject);
};