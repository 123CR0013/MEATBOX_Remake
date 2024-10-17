#include"Track.h"
#include"UIAnimation.h"
#include"UI.h"
#include"../Easing/Easing.h"

Track::Track(UIAnimation* owner)
	:mOwner(owner)
{

}

Track::~Track(){}

void Track::Update() {

}

AnimTrackMap Track::GetOwnerAnimTrackMap()
{
	return mOwner->GetOwner()->GetOwner()->GetAnimTrackMap();
}

unsigned int Track::GetKeyNum() {

	auto animTrackMap = GetOwnerAnimTrackMap();

	unsigned int maxFrame = static_cast<unsigned int>(animTrackMap[mOwner->GetAnimName()][GetName()].back()[UIAnimation::TO_FRAME]);
	unsigned int fromFrame = mOwner->GetFromFrameCount() % (mOwner->GetMaxFrame() + 1);

	//Trackの最大より大きければ、まだアニメーションは一周していない
	if (fromFrame > maxFrame) {
		return static_cast<unsigned int>(animTrackMap[mOwner->GetAnimName()][GetName()].size());
	}

	for (unsigned int i = 0; i < (unsigned int)animTrackMap[mOwner->GetAnimName()][GetName()].size(); ++i)
	{
		unsigned int frame = static_cast<unsigned int>(animTrackMap[mOwner->GetAnimName()][GetName()][i][UIAnimation::TO_FRAME]);

		if (fromFrame <= frame)
		{
			return i;
		}
	}

	return 0;
}

unsigned int Track::GetMaxToFrame() { return (unsigned int)GetOwnerAnimTrackMap()[mOwner->GetAnimName()][GetName()].back()[UIAnimation::TO_FRAME]; }

//---------------------------------
const std::string LocationTrack::LOCATION_TRACK = "LOCATION_TRACK";

LocationTrack::LocationTrack(UIAnimation* owner)
	:Track(owner)
{

}

LocationTrack::~LocationTrack(){}

void LocationTrack::Update()
{
	Vector2 v;

	auto animTrackmap = GetOwnerAnimTrackMap();

	auto beforeTransform = mOwner->GetBeforeTransform();
	auto animName = mOwner->GetAnimName();

	//現在のキーフレームの番号
	auto keyNum = GetKeyNum();
	unsigned int beforeKeyNum = keyNum != 0 ? keyNum - 1 : 0;

	float ToX = beforeTransform.mLocation.x + animTrackmap[animName][LOCATION_TRACK][keyNum][UIAnimation::LOCATION_X]; 
	float ToY = beforeTransform.mLocation.y + animTrackmap[animName][LOCATION_TRACK][keyNum][UIAnimation::LOCATION_Y]; 
	float beforeToX = beforeTransform.mLocation.x ;
	float beforeToY = beforeTransform.mLocation.y ;
	beforeToX += keyNum != 0 ? animTrackmap[animName][LOCATION_TRACK][beforeKeyNum][UIAnimation::LOCATION_X] : 0.f;
	beforeToY += keyNum != 0 ? animTrackmap[animName][LOCATION_TRACK][beforeKeyNum][UIAnimation::LOCATION_Y] : 0.f;

	float beforeToFrame = keyNum != 0 ? animTrackmap[animName][LOCATION_TRACK][beforeKeyNum][UIAnimation::TO_FRAME] : 0.f;
	float toFrame = animTrackmap[animName][LOCATION_TRACK][keyNum][UIAnimation::TO_FRAME] - beforeToFrame;

	float frameCount = (float)mOwner->GetFromFrameCount() - beforeToFrame;

	unsigned char easingNum = (unsigned char)(animTrackmap[animName][LOCATION_TRACK][keyNum][UIAnimation::EASING_NUM]);

	v.x = Easing::EasingFuncs[easingNum](frameCount, beforeToX, ToX, toFrame);
	v.y = Easing::EasingFuncs[easingNum](frameCount, beforeToY, ToY, toFrame);

	mOwner->GetOwner()->SetLocation(v);
}
//------------------------------------------------

const std::string LocationXTrack::LOCATION_X_TRACK = "LOCATION_X_TRACK";

LocationXTrack::LocationXTrack(UIAnimation* owner)
	:Track(owner)
{

}

LocationXTrack::~LocationXTrack(){}

void LocationXTrack::Update()
{

	auto animTrackmap = GetOwnerAnimTrackMap();

	auto beforeTransform = mOwner->GetBeforeTransform();
	auto animName = mOwner->GetAnimName();

	//現在のキーフレームの番号
	auto keyNum = GetKeyNum();
	unsigned int beforeKeyNum = keyNum != 0 ? keyNum - 1 : 0;

	float ToX = beforeTransform.mLocation.x + animTrackmap[animName][LOCATION_X_TRACK][keyNum][UIAnimation::LOCATION_X];
	float beforeToX = beforeTransform.mLocation.x;
	beforeToX += keyNum != 0 ? animTrackmap[animName][LOCATION_X_TRACK][beforeKeyNum][UIAnimation::LOCATION_X] : 0.f;

	float beforeToFrame = keyNum != 0 ? animTrackmap[animName][LOCATION_X_TRACK][beforeKeyNum][UIAnimation::TO_FRAME] : 0.f;
	float toFrame = animTrackmap[animName][LOCATION_X_TRACK][keyNum][UIAnimation::TO_FRAME] - beforeToFrame;

	float frameCount = (float)mOwner->GetFromFrameCount() - beforeToFrame;

	unsigned char easingNum = (unsigned char)(animTrackmap[animName][LOCATION_X_TRACK][keyNum][UIAnimation::EASING_NUM]);

	Vector2 v = mOwner->GetOwner()->GetLocation();
	v.x = Easing::EasingFuncs[easingNum](frameCount, beforeToX, ToX, toFrame);
	mOwner->GetOwner()->SetLocation(v);
}

//------------------------------------------------

const std::string LocationYTrack::LOCATION_Y_TRACK = "LOCATION_Y_TRACK";

LocationYTrack::LocationYTrack(UIAnimation* owner)
	:Track(owner)
{

}

LocationYTrack::~LocationYTrack() {}

void LocationYTrack::Update()
{

	auto animTrackmap = GetOwnerAnimTrackMap();

	auto beforeTransform = mOwner->GetBeforeTransform();
	auto animName = mOwner->GetAnimName();

	//現在のキーフレームの番号
	auto keyNum = GetKeyNum();
	unsigned int beforeKeyNum = keyNum != 0 ? keyNum - 1 : 0;

	float ToY = beforeTransform.mLocation.y + animTrackmap[animName][LOCATION_Y_TRACK][keyNum][UIAnimation::LOCATION_Y];
	float beforeToY = beforeTransform.mLocation.y;
	beforeToY += keyNum != 0 ? animTrackmap[animName][LOCATION_Y_TRACK][beforeKeyNum][UIAnimation::LOCATION_Y] : 0.f;

	float beforeToFrame = keyNum != 0 ? animTrackmap[animName][LOCATION_Y_TRACK][beforeKeyNum][UIAnimation::TO_FRAME] : 0.f;
	float toFrame = animTrackmap[animName][LOCATION_Y_TRACK][keyNum][UIAnimation::TO_FRAME] - beforeToFrame;

	float frameCount = (float)mOwner->GetFromFrameCount() - beforeToFrame;

	unsigned char easingNum = (unsigned char)(animTrackmap[animName][LOCATION_Y_TRACK][keyNum][UIAnimation::EASING_NUM]);

	Vector2 v = mOwner->GetOwner()->GetLocation();
	v.y = Easing::EasingFuncs[easingNum](frameCount, beforeToY, ToY, toFrame);
	mOwner->GetOwner()->SetLocation(v);
}

//----------------------------------------------
const std::string OpacityTrack::OPACITY_TRACK = "OPACITY_TRACK";

OpacityTrack::OpacityTrack(UIAnimation* owner) 
	:Track(owner)
{

}

OpacityTrack::~OpacityTrack(){}

void OpacityTrack::Update() 
{
	auto animTrackmap = GetOwnerAnimTrackMap();

	auto animName = mOwner->GetAnimName();
	auto keyNum = GetKeyNum();
	unsigned int beforeKeyNum = keyNum != 0 ? keyNum - 1 : 0;

	float toAlpha = mOwner->GetBeforeAlpha() + animTrackmap[animName][OPACITY_TRACK][keyNum][UIAnimation::ALPHA];
	float beforeAlpha = mOwner->GetBeforeAlpha();
	beforeAlpha += keyNum != 0 ? animTrackmap[animName][OPACITY_TRACK][beforeKeyNum][UIAnimation::ALPHA] : 0.f;

	float beforeToFrame = keyNum != 0 ? animTrackmap[animName][OPACITY_TRACK][beforeKeyNum][UIAnimation::TO_FRAME] : 0.f;
	float toFrame = animTrackmap[animName][OPACITY_TRACK][keyNum][UIAnimation::TO_FRAME] - beforeToFrame;
	float frameCount = (float)mOwner->GetFromFrameCount() - beforeToFrame;

	unsigned char easingNum = (unsigned char)(animTrackmap[animName][OPACITY_TRACK][keyNum][UIAnimation::EASING_NUM]);
	float alpha = Easing::EasingFuncs[easingNum](frameCount, beforeAlpha, toAlpha, toFrame);

	mOwner->GetOwner()->SetAlpha(alpha);
}

//----------------------------------------------

const std::string RotateTrack::ROTATE_TRACK = "ROTATE_TRACK";

RotateTrack::RotateTrack(UIAnimation* owner)
	:Track(owner)
{

}

RotateTrack::~RotateTrack(){}

void RotateTrack::Update()
{

	auto animTrackmap = GetOwnerAnimTrackMap();

	auto animName = mOwner->GetAnimName();

	auto keyNum = GetKeyNum();
	unsigned int beforeKeyNum = keyNum != 0 ? keyNum - 1 : 0;

	float toAngle = mOwner->GetBeforeTransform().mRotate + animTrackmap[animName][ROTATE_TRACK][keyNum][UIAnimation::ROTATE];
	float beforeAngle = mOwner->GetBeforeTransform().mRotate;
	beforeAngle += keyNum != 0 ? animTrackmap[animName][ROTATE_TRACK][beforeKeyNum][UIAnimation::ROTATE] : 0.f;

	float beforeToFrame = keyNum != 0 ? animTrackmap[animName][ROTATE_TRACK][beforeKeyNum][UIAnimation::TO_FRAME] : 0.f;
	float toFrame = animTrackmap[animName][ROTATE_TRACK][keyNum][UIAnimation::TO_FRAME] - beforeToFrame;
	float frameCount = (float)mOwner->GetFromFrameCount() - beforeToFrame;

	unsigned char easingNum = (unsigned char)(animTrackmap[animName][ROTATE_TRACK][keyNum][UIAnimation::EASING_NUM]);

	float angle = Easing::EasingFuncs[easingNum](frameCount, beforeAngle, toAngle, toFrame);
	mOwner->GetOwner()->SetRotate(angle);
}

//----------------------------------------------
const std::string ScaleTrack::SCALE_TRACK = "SCALE_TRACK";


ScaleTrack::ScaleTrack(UIAnimation* owner)
	:Track(owner)
{

}

ScaleTrack::~ScaleTrack(){}

void ScaleTrack::Update() {

	auto animTrackmap = GetOwnerAnimTrackMap();

	auto animName = mOwner->GetAnimName();

	auto keyNum = GetKeyNum();
	unsigned int beforeKeyNum = keyNum != 0 ? keyNum - 1 : 0;

	float toX = mOwner->GetBeforeTransform().mScale.x+animTrackmap[animName][SCALE_TRACK][keyNum][UIAnimation::SCALE_X];
	float toY = mOwner->GetBeforeTransform().mScale.y+animTrackmap[animName][SCALE_TRACK][keyNum][UIAnimation::SCALE_Y];
	float beforeToX = mOwner->GetBeforeTransform().mScale.x;
	float beforeToY = mOwner->GetBeforeTransform().mScale.y;
	beforeToX += keyNum != 0 ? animTrackmap[animName][SCALE_TRACK][beforeKeyNum][UIAnimation::SCALE_X] : 0.f;
	beforeToY += keyNum != 0 ? animTrackmap[animName][SCALE_TRACK][beforeKeyNum][UIAnimation::SCALE_Y] : 0.f;

	float beforeToFrame = keyNum != 0 ? animTrackmap[animName][SCALE_TRACK][beforeKeyNum][UIAnimation::TO_FRAME] : 0.f;
	float toFrame = animTrackmap[mOwner->GetAnimName()][SCALE_TRACK][keyNum][UIAnimation::TO_FRAME] - beforeToFrame;
	float frameCount = (float)mOwner->GetFromFrameCount() - beforeToFrame;

	//使用するイージング関数の配列番号
	unsigned char easingNum = (unsigned char)(animTrackmap[animName][SCALE_TRACK][keyNum][UIAnimation::EASING_NUM]);

	float x = Easing::EasingFuncs[easingNum](frameCount, beforeToX, toX, toFrame);
	float y = Easing::EasingFuncs[easingNum](frameCount, beforeToY, toY, toFrame);

	mOwner->GetOwner()->SetScale(Vector2(x, y));
}


//----------------------------------------------
const std::string WorldRotateTrack::WORLD_ROTATE_TRACK = "WORLD_ROTATE_TRACK";


WorldRotateTrack::WorldRotateTrack(UIAnimation* owner)
	:Track(owner)
{

}

WorldRotateTrack::~WorldRotateTrack() {}

void WorldRotateTrack::Update() {

	auto animTrackmap = GetOwnerAnimTrackMap();

	auto animName = mOwner->GetAnimName();

	auto keyNum = GetKeyNum();
	unsigned int beforeKeyNum = keyNum != 0 ? keyNum - 1 : 0;

	float toAngle = animTrackmap[animName][WORLD_ROTATE_TRACK][keyNum][UIAnimation::ROTATE];
	float beforeAngle = keyNum != 0 ? animTrackmap[animName][WORLD_ROTATE_TRACK][beforeKeyNum][UIAnimation::ROTATE] : mOwner->GetBeforeTransform().mRotate;

	float beforeToFrame = keyNum != 0 ? animTrackmap[animName][WORLD_ROTATE_TRACK][beforeKeyNum][UIAnimation::TO_FRAME] : 0.f;
	float toFrame = animTrackmap[animName][WORLD_ROTATE_TRACK][keyNum][UIAnimation::TO_FRAME] - beforeToFrame;
	float frameCount = (float)mOwner->GetFromFrameCount() - beforeToFrame;

	unsigned char easingNum = (unsigned char)(animTrackmap[animName][WORLD_ROTATE_TRACK][keyNum][UIAnimation::EASING_NUM]);

	float angle = Easing::EasingFuncs[easingNum](frameCount, beforeAngle, toAngle, toFrame);
	mOwner->GetOwner()->SetRotate(angle);
}

