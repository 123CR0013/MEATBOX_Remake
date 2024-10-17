#pragma once
#include<vector>
#include"../Math/Transform2/Transform2.h"
#include<functional>
#include<string>
#include<unordered_map>

class UIAnimation {
public:
	UIAnimation(class UI* owner);
	virtual ~UIAnimation();

	void Play(const std::string& animName, bool isUpdate);
	void Update();

	unsigned int GetFromFrameCount()const { return _fromFrameCount; }
	char GetFramePassSpeed()const { return _framePassSpeed; }
	unsigned int GetMaxFrame()const { return _maxFrame; }

	const Transform2& GetBeforeTransform()const { return _beforeTransform; }
	float GetBeforeAlpha()const { return _beforeAlpha; }

	const std::string& GetAnimName()const { return _animName; }

	class UI* GetOwner()const { return _owner; }

	class Track* CreateTrack(const std::string& trackName);

	void Reverse() { 
		_framePassSpeed = -1; 
		_fromFrameCount = _fromFrameCount == _maxFrame ? _maxFrame - 1 : _fromFrameCount; 
	}

	void Reset()
	{
		_fromFrameCount = 1;
		_fromFrameCount = 1;
		_loopNum = 0;
	}

	void SetLoopNum(unsigned int loopNum) { _loopNum = loopNum; }
protected:

	void LoadAnimData(); 

	std::vector<class Track*>_tracks;

	//�A�j���[�V�����O�̃p�����[�^�[
	Transform2 _beforeTransform;
	float _beforeAlpha;

	//�Đ�����A�j���[�V������
	std::string _animName;

private:
	class UI* _owner;

	//�A�j���[�V�����ɂ�����t���[��
	unsigned int _maxFrame;

	//�A�j���[�V�������J�n���Ă���o�����t���[����
	unsigned int _fromFrameCount;

	//�Đ������
	unsigned int _loopNum;

	//�Đ�������
	unsigned int _loopCount;

	//�t���[�����i�ރX�s�[�h(��{�I�ɐ�Βl��1)
	char _framePassSpeed;

	static std::unordered_map<std::string, std::function<class Track* (UIAnimation*)>>mCreateTrackList;

public:

	static const std::string LOCATION_X;
	static const std::string LOCATION_Y;
	static const std::string ROTATE;
	static const std::string SCALE_X;
	static const std::string SCALE_Y;
	static const std::string ALPHA;
	static const std::string TO_FRAME;
	static const std::string EASING_NUM;

	static void AddCreateFunc(const std::string& trackName, std::function<class Track* (UIAnimation*)> func = nullptr);
};