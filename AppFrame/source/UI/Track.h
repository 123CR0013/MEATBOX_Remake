#pragma once
#include<functional>
#include<string>
#include"../Math/Transform2/Transform2.h"
#include"UIScreen.h"

class Track {
public:
	Track(class UIAnimation* owner);
	virtual ~Track();

	virtual void Update() = 0;

	virtual const std::string& GetName()const = 0;

	unsigned int GetKeyNum();

	unsigned int GetMaxToFrame();

	AnimTrackMap GetOwnerAnimTrackMap();
protected:
	class UIAnimation* mOwner;

};

class LocationTrack final: public Track {
public:
	LocationTrack(class UIAnimation* owner);
	~LocationTrack();

	void Update()override;

	const std::string& GetName()const override { return LOCATION_TRACK; }

	static const std::string LOCATION_TRACK;

};

class LocationXTrack final : public Track {
public:
	LocationXTrack(class UIAnimation* owner);
	~LocationXTrack();

	void Update()override;

	const std::string& GetName()const override { return LOCATION_X_TRACK; }

	static const std::string LOCATION_X_TRACK;

};

class LocationYTrack final : public Track {
public:
	LocationYTrack(class UIAnimation* owner);
	~LocationYTrack();

	void Update()override;

	const std::string& GetName()const override { return LOCATION_Y_TRACK; }

	static const std::string LOCATION_Y_TRACK;

};

class OpacityTrack final :public Track {
public:
	OpacityTrack(class UIAnimation* owner);
	~OpacityTrack();

	void Update()override;

	const std::string& GetName()const override { return OPACITY_TRACK; }

	static const std::string OPACITY_TRACK;

};

class RotateTrack final :public Track {
public:
	RotateTrack(class UIAnimation* owner);
	~RotateTrack();

	void Update()override;

	const std::string& GetName()const override { return ROTATE_TRACK; }

	static const std::string ROTATE_TRACK;

};

class ScaleTrack final :public Track {
public:
	ScaleTrack(class UIAnimation* owner);
	~ScaleTrack();

	void Update()override;

	const std::string& GetName()const override { return SCALE_TRACK; }

	static const std::string SCALE_TRACK;

};;