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


	void SetZoom(float zoom) { _zoom = zoom; }
	void SetAngle(float angle) { _angle = angle; }
	void SetWidth(float width) { _width = width; }
	void SetHeight(float height) { _height = height; }
	void SetSize(float width, float height) { _width = width; _height = height; }


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
};


