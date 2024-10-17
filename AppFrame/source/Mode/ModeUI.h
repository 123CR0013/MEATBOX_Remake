#pragma once
#pragma once
#include<vector>
#include"ModeBase.h"

class ModeUI :public ModeBase {
public:
	ModeUI();
	virtual ~ModeUI();

	virtual bool Initialize()override;		//‰Šú‰»
	virtual bool Terminate()override;		//I—¹ˆ—
	virtual bool Process()override;			//ˆ—
	virtual bool Render()override;		//•`‰æ

	void AddUIScreen(class UIScreen* screen);
	void DeleteUIScreen(class UIScreen* screen);
	void ClearUIScreen();

private:

	std::vector<class UIScreen*>_UIScreens;
	std::vector<class UIScreen*>_addUIScreens;
	std::vector<class UIScreen*>_deleteUIScreens;
};