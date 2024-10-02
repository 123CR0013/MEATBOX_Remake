#include"UIScreen.h"
#include"../Mode/ModeUI.h"
#include"UI.h"
#include"Track.h"
#include"UIAnimation.h"
#include<algorithm>

UIScreen::UIScreen(ModeUI* owner)
	:_owner(owner)
{
	_owner->AddUIScreen(this);
	Init();
}

UIScreen::~UIScreen() {
	for (auto&& ui : _UIs) {
		delete ui;
	}
	_UIs.clear();
	mAnimTrackMap.clear();
}

void UIScreen::Init() {
	UIAnimation::AddCreateFunc(LocationTrack::LOCATION_TRACK, [](UIAnimation* owner)->Track* {return new LocationTrack(owner); });
	UIAnimation::AddCreateFunc(LocationXTrack::LOCATION_X_TRACK, [](UIAnimation* owner)->Track* {return new LocationXTrack(owner); });
	UIAnimation::AddCreateFunc(LocationYTrack::LOCATION_Y_TRACK, [](UIAnimation* owner)->Track* {return new LocationYTrack(owner); });
	UIAnimation::AddCreateFunc(OpacityTrack::OPACITY_TRACK, [](UIAnimation* owner)->Track* {return new OpacityTrack(owner); });
	UIAnimation::AddCreateFunc(RotateTrack::ROTATE_TRACK, [](UIAnimation* owner)->Track* {return new RotateTrack(owner); });
	UIAnimation::AddCreateFunc(ScaleTrack::SCALE_TRACK, [](UIAnimation* owner)->Track* {return new ScaleTrack(owner); });
}


void UIScreen::Update() {
	for (auto&& ui : _UIs) {
		ui->Process();
	}
}

void UIScreen::Draw() {
	for (auto&& ui : _UIs) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * ui->GetAlpha());
		ui->UpdateAnimation();
		ui->Draw();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void UIScreen::RegistUI(UI* ptr) {
	_UIs.emplace_back(ptr);
	Sort();
}

void UIScreen::RemoveUI(UI* ptr)
{
	auto iter = std::find(_UIs.begin(), _UIs.end(), ptr);

	if (iter != _UIs.end())
	{
		_UIs.erase(iter);
	}
}

void UIScreen::Sort() {
	std::sort( _UIs.begin(), _UIs.end(), [](UI* a, UI* b) {return a->GetOrder() < b->GetOrder(); });
}

void UIScreen::DeleteUIScreen(UIScreen* screen)
{
	_owner->DeleteUIScreen(screen);
}

void UIScreen::CreateLocationXAnim(const std::string& animName, float x, unsigned int toFrame, unsigned char easingNum)
{
	mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK].emplace_back();
	mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK].back()[UIAnimation::LOCATION_X] = x;
	mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK].back()[UIAnimation::TO_FRAME] = (float)toFrame;

	size_t size = mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK].size();
	size = size < 0 ? 0 : size - 1;

	for (size_t i = 0; i < size; ++i) {
		mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK].back()[UIAnimation::LOCATION_X] += mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK][i][UIAnimation::LOCATION_X];
		mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK].back()[UIAnimation::TO_FRAME] += mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK][i][UIAnimation::TO_FRAME];
	}

	mAnimTrackMap[animName][LocationXTrack::LOCATION_X_TRACK].back()[UIAnimation::EASING_NUM] = (float)easingNum;
}

void UIScreen::CreateLocationYAnim(const std::string& animName, float y, unsigned int toFrame, unsigned char easingNum)
{
	mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK].emplace_back();
	mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK].back()[UIAnimation::LOCATION_Y] = y;
	mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK].back()[UIAnimation::TO_FRAME] = (float)toFrame;

	size_t size = mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK].size();
	size = size < 0 ? 0 : size - 1;

	for (size_t i = 0; i < size; ++i) {
		mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK].back()[UIAnimation::LOCATION_Y] += mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK][i][UIAnimation::LOCATION_Y];
		mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK].back()[UIAnimation::TO_FRAME] += mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK][i][UIAnimation::TO_FRAME];
	}

	mAnimTrackMap[animName][LocationYTrack::LOCATION_Y_TRACK].back()[UIAnimation::EASING_NUM] = (float)easingNum;
}

void UIScreen::CreateLocationAnim(const std::string& animName, float x, float y, unsigned int toFrame, unsigned char easingNum)
{
	mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].emplace_back();
	mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].back()[UIAnimation::LOCATION_X] = x;
	mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].back()[UIAnimation::LOCATION_Y] = y;
	mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].back()[UIAnimation::TO_FRAME] = (float)toFrame;

	size_t size = mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].size();
	size = size < 0 ? 0 : size - 1;

	for (size_t i = 0; i < size; ++i) {
		mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].back()[UIAnimation::LOCATION_X] += mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK][i][UIAnimation::LOCATION_X];
		mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].back()[UIAnimation::LOCATION_Y] += mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK][i][UIAnimation::LOCATION_Y];
		mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].back()[UIAnimation::TO_FRAME] += mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK][i][UIAnimation::TO_FRAME];
	}

	mAnimTrackMap[animName][LocationTrack::LOCATION_TRACK].back()[UIAnimation::EASING_NUM] = (float)easingNum;
}

void UIScreen::CreateOpacityAnim(const std::string& animName, float alpha, unsigned int toFrame, unsigned char easingNum)
{
	mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK].emplace_back();

	mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK].back()[UIAnimation::ALPHA] = alpha;
	mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK].back()[UIAnimation::TO_FRAME] = (float)toFrame;

	size_t size = mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK].size();
	size = size < 0 ? 0 : size - 1;

	for (size_t i = 0; i < size; ++i) {
		mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK].back()[UIAnimation::ALPHA] += mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK][i][UIAnimation::ALPHA];
		mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK].back()[UIAnimation::TO_FRAME] += mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK][i][UIAnimation::TO_FRAME];
	}

	mAnimTrackMap[animName][OpacityTrack::OPACITY_TRACK].back()[UIAnimation::EASING_NUM] = (float)easingNum;
}

void UIScreen::CreateRotateAnim(const std::string& animName, float rotate, unsigned int toFrame, unsigned char easingNum)
{
	mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK].emplace_back();
	mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK].back()[UIAnimation::ROTATE] = rotate;
	mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK].back()[UIAnimation::TO_FRAME] = (float)toFrame;

	size_t size = mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK].size();
	size = size < 0 ? 0 : size - 1;

	for (size_t i = 0; i < size; ++i) {
		mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK].back()[UIAnimation::ROTATE] += mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK][i][UIAnimation::ROTATE];
		mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK].back()[UIAnimation::TO_FRAME] += mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK][i][UIAnimation::TO_FRAME];
	}

	mAnimTrackMap[animName][RotateTrack::ROTATE_TRACK].back()[UIAnimation::EASING_NUM] = (float)easingNum;
}

void UIScreen::CreateScaleAnim(const std::string& animName, float x, float y, unsigned int toFrame, unsigned char easingNum)
{
	mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].emplace_back();
	mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].back()[UIAnimation::SCALE_X] = x;
	mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].back()[UIAnimation::SCALE_Y] = y;
	mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].back()[UIAnimation::TO_FRAME] = (float)toFrame;

	size_t size = mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].size();
	size = size < 0 ? 0 : size - 1;

	for (size_t i = 0; i < size; ++i) {
		mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].back()[UIAnimation::SCALE_X] += mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK][i][UIAnimation::SCALE_X];
		mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].back()[UIAnimation::SCALE_Y] += mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK][i][UIAnimation::SCALE_Y];
		mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].back()[UIAnimation::TO_FRAME] += mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK][i][UIAnimation::TO_FRAME];
	}

	mAnimTrackMap[animName][ScaleTrack::SCALE_TRACK].back()[UIAnimation::EASING_NUM] = (float)easingNum;
}