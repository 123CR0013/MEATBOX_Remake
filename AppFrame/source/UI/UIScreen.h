#pragma once
#include<vector>
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

	void RegistUI(class UI* ptr);
	void RemoveUI(class UI* ptr);

	//配列外だったら、nullptr(エラー)を返す
	class UI* GetUI(size_t num) {
		return 0 <= num && num < _UIs.size() ? _UIs[num] : nullptr;
	}

	void DeleteUIScreen(UIScreen* screen);

	AnimTrackMap GetAnimTrackMap() { return mAnimTrackMap; }

protected:

	ModeUI* GetOwner()const { return _owner; }

	void CreateLocationXAnim(const std::string& animName, float x, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateLocationYAnim(const std::string& animName,float y, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateLocationAnim(const std::string& animName, float x, float y, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateRotateAnim(const std::string& animName, float rotate, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateScaleAnim(const std::string& animName, float x, float y, unsigned int toFrame, unsigned char easingNum = 0);
	void CreateOpacityAnim(const std::string& animName, float alpha, unsigned int toFrame, unsigned char easingNum = 0);

	//*******************************

private:

	std::vector<class UI*>_UIs;

	class ModeUI* _owner;

	AnimTrackMap mAnimTrackMap;
};