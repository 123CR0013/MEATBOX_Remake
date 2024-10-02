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

	//�I�[�i�[�̎擾
	class UIScreen* GetOwner()const { return _owner; }

	//�A�j���[�V�����̍Đ�
	void PlayAnimation(const std::string& animName,unsigned int loopNum = 1);

	//�Đ����Ă���A�j���[�V�������t�Đ�
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
	//�e�������炷�ׂă��[�J���ɂȂ�
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

	//�`�悷��4���_�̎擾
	//0:����
	//1:�E��
	//2:�E��
	//3:����
	std::array<Vector2, 4> GetVertexes(bool isTurn = false);

	//���W�E��]�l�E�X�P�[��
	Transform2 _transform;

	//�����x			����			�s����
	float _alpha;	//0.f	�`	1.f

	//��ʂɕ`�悷��傫��
	float _width;
	float _height;

	//���]�`�悷�邩
	bool _isTurn;
protected:
	bool OnMouse();	

public:
	void SetForceFalseInputMouse(bool isForceFalseInputMouse) { _isForceFalseInputMouse = isForceFalseInputMouse; }
private:

	//�e�I�u�W�F�N�g
	UI* _parent;

	//�o�^���Ă���q�I�u�W�F�N�g
	std::vector<UI*>_children;

	//�Đ����Ă���A�j���[�V����
	std::pair<std::string, std::unique_ptr<class UIAnimation>>_playAnimation;

	//�}�E�X�J�[�\�����u����Ă��邩
	bool _isOnMouse;
	bool _isOldOnMouse;

	//�}�E�X�J�[�\���̓��͂������I��false�ɂ��邩
	bool _isForceFalseInputMouse;

	//Animation�̕ێ�����BeforeTransform���X�V���邩
	bool _isUpdateBeforeTransform;

	//�����E�`�揇��
	unsigned int _order;

	//�I�[�i�[(���g��ێ�����N���X)
	class UIScreen* _owner;
};