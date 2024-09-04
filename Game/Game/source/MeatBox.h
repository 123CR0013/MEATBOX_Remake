#pragma once
#include "GameObject.h"
class StickyGroup;

class MeatBox : public GameObject
{
public:
	MeatBox(ModeBase* mode);
	~MeatBox() override;
	
	// 移動できるかどうか
	// クラス外部から呼び出すインターフェース
	bool CheckMove(Vector3 vMove) override;

	// 移動できるかどうか
	// クラス内部、又は、StickyGroupで使用する関数
	bool _CheckMove(Vector3 vMove);

	// 移動処理
	void Move(Vector3 vMove);

	void DrawDebug() override;

	StickyGroup* GetStickyGroup() { return _pStickyGroup; }
	// StickyGroupクラス内で使用すること
	void SetStickyGroup(StickyGroup* pSticky) { _pStickyGroup = pSticky; } 

protected:
	MapChip* CheckMapChip(Vector3 vPos);
	GameObject* CheckObject(Vector3 vPos);


	StickyGroup* _pStickyGroup;
	void SetExistHole(bool bExistHole);
	void SetExistFloor(bool bExistFloor);
	void SetGoThroughHole(bool bGoThroughHole);
};