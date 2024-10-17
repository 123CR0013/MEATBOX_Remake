#pragma once
#include "appframe.h"
#include "AnimationInfo.h"
#include <map>

class ObjectBase;

class Animation final
{
public:
	Animation(ObjectBase* object);
	~Animation();

	void Process();
	void Draw();

	void AddAnimInfo(AnimationInfo* animInfo, int index);
	void SetAnimIndex(int index);
	int GetAnimIndex() { return _groupIndex; }
	void ChangeRandomIndex();

	int GetAnimCnt() { return _animCnt; }
	void SetAnimCnt(int cnt);
	// �A�j���[�V�����J�E���^���w��
	// �����ڂ̉摜��\�����邩���w�肷��i0~�j
	void SetAnimCntForNumOfAnim(int cnt);

	bool IsEnd() { return _bAnimEnd; }

	int GetGraphHandle();

	Vector3 GetDrawOffset() { return _vDrawOffset; }
	void SetDrawOffset(Vector3 offset) { _vDrawOffset = offset; }

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

	int GetDrawOrder() { return _drawOrder; }
	void SetDrawOrder(int drawOrder) { _drawOrder = drawOrder; }

	void SetDrawWithScreenPos(bool bDrawWithScreenPos) { _bDrawWithScreenPos = bDrawWithScreenPos; }


protected:
	ObjectBase* _parentObj;

	// �A�j���[�V�������
	std::map<int, std::vector<AnimationInfo*>> _animInfo;
	// �O���[�v���̃A�j���[�V�������i�����_���Đ��p�j
	std::map<int, int> _animNum;
	// �O���[�v�C���f�b�N�X
	int _groupIndex;
	// �O���[�v�C���f�b�N�X�̍ő�l
	int _groupIndexMax;
	// �����_���Đ��p�̃C���f�b�N�X
	int _randomIndex;
	// �A�j���[�V�����J�E���^
	int _animCnt;

	// �A�j���[�V�����I���t���O
	bool _bAnimEnd;

	// �`��I�t�Z�b�g
	// _parentObj��vPos�ɂ��̒l�����Z���ĕ`�悷��
	Vector3 _vDrawOffset;

	float _zoom;
	float _angle;
	float _width, _height;

	// �`�揇
	// -1: �I�u�W�F�N�g�̉��ɕ`��i�G�t�F�N�g�j
	// 0 : y���W�����������̂���`��i�I�u�W�F�N�g�j
	// 10000: �I�u�W�F�N�g�̏�ɕ`��i�G�t�F�N�g�j
	int _drawOrder;

	// _parentObj��vPos���X�N���[�����W�Ƃ��Ĉ�����
	bool _bDrawWithScreenPos = false;
};


