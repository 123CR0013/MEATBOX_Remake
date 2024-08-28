#include"UIAnimation.h"
#include"UI.h"
#include"Track.h"
#include"../Math/MyMath/mymath.h"
#include"UIScreen.h"

const std::string UIAnimation::LOCATION_X = "LOCATION_X";
const std::string UIAnimation::LOCATION_Y = "LOCATION_Y";
const std::string UIAnimation::ALPHA = "ALPHA";
const std::string UIAnimation::ROTATE = "ROTATE";
const std::string UIAnimation::SCALE_X = "SCALE_X";
const std::string UIAnimation::SCALE_Y = "SCALE_Y";
const std::string UIAnimation::TO_FRAME = "TO_FRAME";
const std::string UIAnimation::EASING_NUM = "EASING_NUM";

std::unordered_map<std::string, std::function<Track* (UIAnimation*)>> UIAnimation::mCreateTrackList;

void UIAnimation::AddCreateFunc(const std::string& trackName, std::function<Track* (UIAnimation*)> func) {
	mCreateTrackList.emplace(trackName, func);
}

UIAnimation::UIAnimation(UI* owner)
	:_owner(owner)
	, _beforeTransform(owner->GetTransform())
	, mBeforeAlpha(owner->GetAlpha())
	, _maxFrame(0)
	, _fromFrameCount(1)
	, _framePassSpeed(1)
	, _loopNum(0)
	, _loopCount(0)
{
}

void UIAnimation::Play(const std::string& animName, bool isUpdate)
{
	mBeforeAlpha = 0.f;
	_maxFrame = 0;
	_framePassSpeed = 1;
	_animName = animName;
	_loopNum = 0;
	_loopCount = 0;

	if (isUpdate) {
		mBeforeAlpha = _owner->GetAlpha();
		_beforeTransform = _owner->GetTransform();
		_fromFrameCount = 1;
	}
	LoadAnimData();

	_fromFrameCount = _fromFrameCount < 1 ? 1 : _fromFrameCount;
}

UIAnimation::~UIAnimation() {
	for (auto&& track : _tracks) {
		delete track;
	}
}

void UIAnimation::Update() {

	//アニメーションが終了している場合は更新しない
	_fromFrameCount = 0 < _fromFrameCount && _fromFrameCount < _maxFrame ? _fromFrameCount + _framePassSpeed : _fromFrameCount;

	//アニメーションが一周したため、ループカウントを増やす
	if (0 != _fromFrameCount && 0 == _fromFrameCount % _maxFrame && _loopNum != 0) {
		_loopCount++;
	}

	//アニメーションの更新
	for (auto&& track : _tracks) {
		//アニメーションの終了フレームに達していない場合は更新
		if(_fromFrameCount <= track->GetMaxToFrame())track->Update(); 
	}

	//ループするためにフレームカウントを初期化
	if ((_loopNum == 0 || _loopNum > _loopCount) && _fromFrameCount >= _maxFrame) {
		_fromFrameCount = 1;
	}
}

void UIAnimation::LoadAnimData() {
	auto animTrackMap = _owner->GetOwner()->GetAnimTrackMap();
	if (animTrackMap.find(_animName) != animTrackMap.end()) {
		for (auto&& trackData: animTrackMap[_animName]) {

			_tracks.emplace_back(CreateTrack(trackData.first));

			//アニメーションにかかるフレームを更新
			unsigned int trackMaxFrame = static_cast<unsigned int>(animTrackMap[_animName][_tracks.back()->GetName()].back()[TO_FRAME]);
			_maxFrame = trackMaxFrame > _maxFrame ? trackMaxFrame : _maxFrame;
		}
	}
}

Track* UIAnimation::CreateTrack(const std::string& trackName) {
	return mCreateTrackList[trackName](this);
}

