#include "Animation.h"
#include "ObjectBase.h"
#include "DrawForMapChip.h"


Animation::Animation(ObjectBase* object)
{
	_parentObj = object;

	_animIndex = 0;
	_animIndexMax = -1;
	_animCnt = 0;

	_bAnimEnd = false;

	_zoom = 1.0f;
	_angle = 0.0f;
	// �f�t�H���g�̕`��T�C�Y�̓}�b�v�`�b�v�T�C�Y�Ɠ���
	_width = 64;
	_height = 64;
}

Animation::~Animation()
{
	for (auto& anim : _animInfo)
	{
		delete anim;
	}
	_animInfo.clear();
}

void Animation::Process()
{
	if (_animIndex <= _animIndexMax)
	{
		_animCnt++;

		AnimationInfo* anim = _animInfo[_animIndex];
		if (anim->_framePerSheet * anim->_tblNum <= _animCnt)
		{
			_animCnt = 0;
			_bAnimEnd = true;
		}
	}
}

void Animation::Draw()
{
	DrawForMapChip::MyDrawModiGraph(MGetIdent(), _parentObj->GetPos() + _parentObj->GetDrawOffset(), _zoom, _angle, _width, _height, GetGraphHandle(), 0);
}

void Animation::AddAnimInfo(AnimationInfo* animInfo)
{
	// --------------------------------------------------
	// �G���[�`�F�b�N

	// �`�揇�e�[�u������̏ꍇ�̓f�t�H���g�̕`�揇��ݒ�
	if(animInfo->_drawTbl.size() == 0)
	{
		int graphNum = animInfo->_graphHandle.size();
		for(int i = 0; i < graphNum; i++)
		{
			animInfo->_drawTbl.push_back(i);
		}
	}

	// �e�[�u���̗v�f�����擾
	animInfo->_tblNum = animInfo->_drawTbl.size();

	// 1��������̕\���t���[������0�̏ꍇ��1��ݒ�
	if(animInfo->_framePerSheet == 0)
	{
		animInfo->_framePerSheet = 1;
	}

	// --------------------------------------------------
	
	// �A�j���[�V��������ǉ�
	this->_animInfo.push_back(animInfo);
	// �A�j���[�V�����C���f�b�N�X�̍ő�l���X�V
	_animIndexMax++;
}

void Animation::SetAnimIndex(int index)
{
	if(0 <= index && index <= _animIndexMax)
	{
		int oldIndex = _animIndex;
		_animIndex = index;
		if (oldIndex != _animIndex) {
			_animCnt = 0;
			_bAnimEnd = false;
		}
	}
}

int Animation::GetGraphHandle()
{
	if(_animIndexMax < _animIndex)
	{
		return -1;
	}
	AnimationInfo* anim = _animInfo[_animIndex];
	return anim->_graphHandle[anim->_drawTbl[(_animCnt / anim->_framePerSheet) % anim->_tblNum]];
}
