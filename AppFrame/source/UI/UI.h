#pragma once
#include"../Math/Transform2/Transform2.h"
#include<vector>
#include<utility>
#include<string>
#include<memory>
#include<array>

class UI {
public:
	UI(class UIScreen* owner,unsigned int order);
	virtual ~UI();

	virtual void Process();
	virtual void Draw() = 0;
	void UpdateAnimation();

	//オーナーの取得
	class UIScreen* GetOwner()const { return _owner; }

	//アニメーションの再生
	void PlayAnimation(const std::string& animName,unsigned int loopNum = 1);

	//再生しているアニメーションを逆再生
	void ReverseAnimation();

	bool IsFinishAnimation();

	//setter
	void SetTransform(const Transform2& transform) { _transform = transform; }
	void SetLocation(float x, float y) { _transform.mLocation = { x,y }; }
	void SetLocation(const Vector2& location) { _transform.mLocation = location; }
	void SetLeftLocation(float x, float y) { _transform.mLocation = { x + _width / 2.f ,y + _height / 2.f }; }
	void SetLeftLocation(const Vector2& location) { SetLeftLocation(location.x, location.y); }
	void SetRotate(float rotate) { _transform.mRotate = rotate; }
	void SetScale(const Vector2& scale) { _transform.mScale = scale; }
	void SetScale(float x, float y) { _transform.mScale = { x,y }; }
	void SetAlpha(float alpha) { _alpha = alpha; }
	void SetWidth(float width) { _width = width; }
	void SetHeight(float height) { _height = height; }
	void SetOrder(unsigned int order) { _order = order; }

	void RegistParent(class UI* parent);
	void RemoveParent();
	std::vector<UI*>& GetChildren()& { return _children; }

	//getter
	//親がいたらすべてローカルになる
	Transform2 GetTransform()const { return _transform; }
	Vector2 GetLocation()const { return _transform.mLocation; }
	float GetRotate()const { return _transform.mRotate; }
	Vector2 GetScale()const { return _transform.mScale; }
	bool IsTurn()const { return _isTurn; }
	float GetAlpha()const { return _alpha; }
	float GetWidth()const { return _width; }
	float GetHeight()const { return _height; }
	Vector2 GetSize()const { return Vector2(_width, _height); }
	unsigned int GetOrder()const { return _order; }
	UI* GetParent()const { return _parent; }

	Matrix3 GetWorldMatrix() 
	{
		return _parent ? _transform.CreateMatrix3() * _parent->GetWorldMatrix() : _transform.CreateMatrix3();
	}

	Vector2 GetWorldLocation()
	{
		return _parent ? _transform.mLocation * _parent->GetWorldMatrix() : _transform.mLocation;
	}

	float GetWorldRotate()
	{
		return _parent ? _transform.mRotate + _parent->GetWorldRotate() : _transform.mRotate;
	}

	Vector2 GetWorldScale()
	{
		return _parent ? _transform.mScale * _parent->GetWorldMatrix() : _transform.mScale;
	}

	bool OnMouseTrg()const;
	bool OnMouseRel()const;

	bool IsLeftClickKey();	
	bool IsLeftClickTrg();
	bool IsLeftClickRel();

	bool IsRightClickKey();
	bool IsRightClickTrg();
	bool IsRightClickRel();

protected:

	//描画する4頂点の取得
	//0:左上
	//1:右上
	//2:右下
	//3:左下
	std::array<Vector2, 4> GetVertexes(bool isTurn = false);

	//座標・回転値・スケール
	Transform2 _transform;

	//透明度			透明			不透明
	float _alpha;	//0.f	～	1.f

	//画面に描画する大きさ
	float _width;
	float _height;

	//反転描画するか
	bool _isTurn;
protected:
	bool OnMouse();	

public:
	void SetForceFalseInputMouse(bool isForceFalseInputMouse) { _isForceFalseInputMouse = isForceFalseInputMouse; }
private:

	//親オブジェクト
	UI* _parent;

	//登録している子オブジェクト
	std::vector<UI*>_children;

	//再生しているアニメーション
	std::pair<std::string, std::unique_ptr<class UIAnimation>>_playAnimation;

	//マウスカーソルが置かれているか
	bool _isOnMouse;
	bool _isOldOnMouse;

	//マウスカーソルの入力を強制的にfalseにするか
	bool _isForceFalseInputMouse;

	//Animationの保持するBeforeTransformを更新するか
	bool _isUpdateBeforeTransform;

	//処理・描画順序
	unsigned int _order;

	//オーナー(自身を保持するクラス)
	class UIScreen* _owner;
};