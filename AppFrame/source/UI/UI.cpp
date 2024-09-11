#include"UI.h"
#include"../ResourceServer/ResourceServer.h"
#include"UIAnimation.h"
#include<algorithm>
#include"Track.h"
#include"../Application/ApplicationBase.h"
#include"../Math/Mymath/mymath.h"
#include"UIScreen.h"
#include"../Application/Global.h"
#include"../Collision/3DCollision.h"

UI::UI(UIScreen* owner,unsigned int order)
	:_owner(owner)
	,_alpha(1.f)
	,_isTurn(false)
	,_width(60.f)
	,_height(20.f)
	,_isOnMouse(false)
	,_isOldOnMouse(false)
	,_isUpdateBeforeTransform(true)
	,_isForceFalseInputMouse(true)
	,_parent(nullptr)
	,_order(order)
{
	_owner->RegistUI(this);
	_playAnimation.second = std::make_unique<UIAnimation>(this);
}

UI::~UI(){

}

void UI::Process(){

	_isOldOnMouse = _isOnMouse;
	_isOnMouse = OnMouse();

}

void UI::Draw(){

}

void UI::UpdateAnimation()
{
	//再生するアニメーションがあるか
	if (_playAnimation.first.size() > 0) {
		_playAnimation.second->Update();
	}
}

void UI::PlayAnimation(const std::string& animName,unsigned int loopNum) {
	auto animTrackMap = _owner->GetAnimTrackMap();

	if (animTrackMap.find(animName) != animTrackMap.end()
		&& _playAnimation.first != animName 
		|| _playAnimation.second->GetFramePassSpeed() < 0
		) {
		//逆再生が終わっていれば、前パラメーターを更新する
		if(!_isUpdateBeforeTransform){_isUpdateBeforeTransform = _playAnimation.second->GetFromFrameCount() == 0; }

		_playAnimation.first = animName;
		_playAnimation.second->Play(animName, _isUpdateBeforeTransform);
		_playAnimation.second->SetLoopNum(loopNum);
		_isUpdateBeforeTransform = true;
	}
}

void UI::ReverseAnimation() {
	if (_playAnimation.second) {
		_playAnimation.second->Reverse();
		_isUpdateBeforeTransform = false;
	}
}

bool UI::IsFinishAnimation()
{
	return
		_playAnimation.second->GetFramePassSpeed() > 0
		&& _playAnimation.second->GetMaxFrame() == _playAnimation.second->GetFromFrameCount()
		//逆再生しているとき
		|| _playAnimation.second->GetFramePassSpeed() < 0
		&& 0 == _playAnimation.second->GetFromFrameCount();
}

std::array<Vector2, 4> UI::GetVertexes(bool isTurn) {
	auto m = _parent ? _transform.CreateMatrix4() * _parent->GetWorldMatrix() : _transform.CreateMatrix4();

	// 描画する画像の4つの頂点座標
	Vector3 _pos[4] = {
		// 通常用（左上から右回り）
		Vector3(-_width / 2.f, -_height / 2.f, 0.f),	// 左上
		Vector3(_width / 2.f, -_height / 2.f ,0.f),	// 右上
		Vector3(_width / 2.f,  _height / 2.f ,0.f),	// 右下
		Vector3(-_width / 2.f,  _height / 2.f ,0.f),	// 左下
	};

	if (_isTurn) {
		// 反転用
		Vector3 posTurn[4] = {
			// 反転用（右上から左回り）
			Vector3(_width / 2.f, -_height / 2.f ,0.f),	// 右上
			Vector3(-_width / 2.f, -_height / 2.f, 0.f),	// 左上
			Vector3(-_width / 2.f,  _height / 2.f ,0.f),	// 左下
			Vector3(_width / 2.f,  _height / 2.f ,0.f),	// 右下
		};
		for (int i = 0; i < 4; i++) {
			_pos[i] = posTurn[i];
		}
	}

	//画面サイズに合わせる
	{
		auto inst = ApplicationBase::GetInstance();

		float scaleW = inst->DispSizeW() / 1920.f;
		float scaleH = inst->DispSizeH() / 1080.f;

		m = m * Matrix4::CreateScale(scaleW, scaleH, 0.f);
	}

	// 4つの頂点座標全てに行列を掛けて変換する
	for (int i = 0; i < 4; i++) {
		_pos[i] = Vector3::Transform(_pos[i], m);
	}

	return std::array<Vector2, 4>{
		Vector2(_pos[0].x, _pos[0].y),	//左上
		Vector2(_pos[1].x, _pos[1].y),	//右上
		Vector2(_pos[2].x, _pos[2].y),	//右下
		Vector2(_pos[3].x, _pos[3].y)	//左下
	};
}

bool UI::OnMouse() {
	auto mousePos = Vector2(static_cast<float>(global._mouseX), static_cast<float>(global._mouseY));
	
	auto vers = GetVertexes();

	return Collision3D::IsOnTrianglePoint(
		Vector3(mousePos.x, mousePos.y, 0.f),
		Vector3(vers[0].x, vers[0].y, 0.f),
		Vector3(vers[1].x, vers[1].y, 0.f),
		Vector3(vers[3].x, vers[3].y, 0.f)) && _isForceFalseInputMouse
		|| Collision3D::IsOnTrianglePoint(
			Vector3(mousePos.x, mousePos.y, 0.f),
			Vector3(vers[1].x, vers[1].y, 0.f),
			Vector3(vers[3].x, vers[3].y, 0.f),
			Vector3(vers[2].x, vers[2].y, 0.f)
		) && _isForceFalseInputMouse;
}

bool UI::OnMouseTrg() const{
	return _isOnMouse && !_isOldOnMouse;
}

bool UI::OnMouseRel() const{
	return !_isOnMouse && _isOldOnMouse;
}

bool UI::IsLeftClickKey() {
	return OnMouse() && (global._mouseKey & MOUSE_INPUT_LEFT);
}

bool UI::IsLeftClickTrg() {
	return OnMouse() && (global._mouseTrg & MOUSE_INPUT_LEFT);
}

bool UI::IsLeftClickRel() {
	return OnMouse() && (global._mouseRel & MOUSE_INPUT_LEFT);
}

bool UI::IsRightClickKey() {
	return OnMouse() && (global._mouseKey & MOUSE_INPUT_RIGHT);
}

bool UI::IsRightClickTrg() {
	return OnMouse() && (global._mouseTrg & MOUSE_INPUT_RIGHT);
}

bool UI::IsRightClickRel() {
	return OnMouse() && (global._mouseRel & MOUSE_INPUT_RIGHT);
}

void UI::RegistParent(UI* parent)
{
	if (this == parent)return;
	_parent = parent;
	_order = parent->GetOrder() + 1;
	_owner->Sort();

	_children.emplace_back(parent);
}

void UI::RemoveParent()
{
	if (_parent)
	{
		auto iter = std::find(_parent->GetChildren().begin(), _parent->GetChildren().end(), this);

		if (iter != _parent->GetChildren().end())
		{
			_parent->GetChildren().erase(iter);
		}

		_parent = nullptr;
	}
}