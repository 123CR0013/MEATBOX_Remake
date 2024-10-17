#pragma once
#include<vector>
#include<list>
#include<unordered_map>
#include<string>

using TrackParamMap = std::unordered_map<std::string, float>;

using TrackKeys = std::vector<TrackParamMap>;

using TrackMap = std::unordered_map<std::string, TrackKeys>;

using AnimTrackMap = std::unordered_map<std::string, TrackMap>;

class UIScreen {
public:
	UIScreen(class ModeUI* owner);
	virtual ~UIScreen();

	static void Init();

	virtual void Update();
	virtual void Draw();

	void Sort();
	void DrawSort();

	void RegistUI(class UI* ptr);
	void RemoveUI(class UI* ptr);

	void DeleteUIScreen(UIScreen* screen);
	void ClearUIScreen();

	AnimTrackMap GetAnimTrackMap() { return mAnimTrackMap; }

protected:

	ModeUI* GetOwner()const { return _owner; }

	//**********アニメーション関連**********
	void CreateLocationXAnim(const std::string& animName, float x, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateLocationYAnim(const std::string& animName,float y, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateLocationAnim(const std::string& animName, float x, float y, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateRotateAnim(const std::string& animName, float rotate, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateScaleAnim(const std::string& animName, float x, float y, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateOpacityAnim(const std::string& animName, float alpha, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateWorldRotateAnim(const std::string& animName, float rotate, unsigned int toFrame, unsigned char easingNum = 0);
	//*******************************

private:

	std::list<class UI*>_UIs;
	std::list<class UI*>_drawUIs;

	class ModeUI* _owner;

	AnimTrackMap mAnimTrackMap;
};