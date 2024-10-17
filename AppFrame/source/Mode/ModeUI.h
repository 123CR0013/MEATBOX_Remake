#pragma once
#pragma once
#include<vector>
#include"ModeBase.h"

class ModeUI :public ModeBase {
public:
	ModeUI();
	virtual ~ModeUI();

	virtual bool Initialize()override;		//������
	virtual bool Terminate()override;		//�I������
	virtual bool Process()override;			//����
	virtual bool Render()override;		//�`��

	void AddUIScreen(class UIScreen* screen);
	void DeleteUIScreen(class UIScreen* screen);
	void ClearUIScreen();

private:

	std::vector<class UIScreen*>_UIScreens;
	std::vector<class UIScreen*>_addUIScreens;
	std::vector<class UIScreen*>_deleteUIScreens;
};