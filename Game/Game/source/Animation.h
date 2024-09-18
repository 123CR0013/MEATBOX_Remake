#pragma once
#include "appframe.h"
#include "AnimationInfo.h"

class ObjectBase;

class Animation final
{
public:
	Animation(ObjectBase* object);
	~Animation();

	void Process();
	void Draw();

	void AddAnimInfo(AnimationInfo* animInfo);
	void SetAnimIndex(int index);
	int GetAnimIndex() { return _animIndex; }

	bool IsEnd() { return _bAnimEnd; }

	int GetGraphHandle();

	float GetZoom() { return _zoom; }
	void SetZoom(float zoom) { _zoom = zoom; }

	float GetAngle() { return _angle; }
	void SetAngle(float angle) { _angle = angle; }

	float GetWidth() { return _width; }
	void SetWidth(float width) { _width = width; }

	float GetHeight() { return _height; }
	void SetHeight(float height) { _height = height; }

	Vector2 GetSize() { return Vector2(_width, _height); }
	void SetSize(float width, float height) { _width = width; _height = height; }
	void SetSize(Vector2 size) { _width = size.x; _height = size.y; }

	void SetDrawWithScreenPos(bool bDrawWithScreenPos) { _bDrawWithScreenPos = bDrawWithScreenPos; }


protected:
	ObjectBase* _parentObj;

	// �A�j���[�V�������
	std::vector<AnimationInfo*> _animInfo;
	// �A�j���[�V�����C���f�b�N�X
	int _animIndex;
	// �A�j���[�V�����C���f�b�N�X�̍ő�l
	int _animIndexMax;
	// �A�j���[�V�����J�E���^
	int _animCnt;

	// �A�j���[�V�����I���t���O
	bool _bAnimEnd;


	float _zoom;
	float _angle;
	float _width, _height;

	// _parentObj��vPos���X�N���[�����W�Ƃ��Ĉ�����
	bool _bDrawWithScreenPos = false;
};


