#pragma once
#pragma once
#include<vector>
#include"ModeBase.h"

class ModeUI :public ModeBase {
public:
	ModeUI();
	virtual ~ModeUI();

	virtual bool Initialize()override;		//初期化
	virtual bool Terminate()override;		//終了処理
	virtual bool Process()override;			//処理
	virtual bool Render()override;		//描画

	void AddUIScreen(class UIScreen* screen);
	void DeleteUIScreen(class UIScreen* screen);

private:

	std::vector<class UIScreen*>_UIScreens;
	std::vector<class UIScreen*>_addUIScreens;
	std::vector<class UIScreen*>_deleteUIScreens;
};