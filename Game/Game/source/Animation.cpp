#include "Animation.h"
#include "appframe.h"
#include "ObjectBase.h"
#include "DrawForMapChip.h"

#include "Map.h"

Animation::Animation(ObjectBase* object)
{
	_parentObj = object;

	_groupIndex = 0;
	_groupIndexMax = -1;
	_randomIndex = 0;
	_animCnt = 0;

	_bAnimEnd = false;

	_vDrawOffset = { 0, 0, 0 };
	_zoom = 1.0f;
	_angle = 0.0f;
	// �f�t�H���g�̕`��T�C�Y�̓}�b�v�`�b�v�T�C�Y�Ɠ���
	_width = CHIP_W;
	_height = CHIP_H;

	_drawOrder = 0;
}

Animation::~Animation()
{
	for (auto& anim : _animInfo)
	{
		for (auto& itr : anim.second)
		{
			delete itr;
		}
	}
	_animInfo.clear();
}

void Animation::Process()
{
	if (_groupIndex <= _groupIndexMax)
	{
		_animCnt++;

		AnimationInfo* anim = _animInfo.at(_groupIndex).at(_randomIndex);
		if (anim->_framePerSheet * anim->_tblNum <= _animCnt)
		{
			_animCnt = 0;
			_bAnimEnd = true;

			ChangeRandomIndex();
		}
	}
}

void Animation::Draw()
{
	if (_bDrawWithScreenPos) 
	{
		MyDraw::MyDrawModiGraph(MGetIdent(), _parentObj->GetPos() + _vDrawOffset, _zoom, _angle, _width, _height, GetGraphHandle(), 0);
	}
	else 
	{
		DrawForMapChip::MyDrawModiGraph(MGetIdent(), _parentObj->GetPos() + _vDrawOffset, _zoom, _angle, _width, _height, GetGraphHandle(), 0);
	}
}

void Animation::AddAnimInfo(AnimationInfo* animInfo, int index)
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
	this->_animInfo[index].push_back(animInfo);
	// �����O���[�v�̃A�j���[�V�����̐����J�E���g
	// ���߂ẴO���[�v�̏ꍇ��1��ݒ�
	if (_animNum.find(index) == _animNum.end())
	{
		_animNum[index] = 1;
		// �O���[�v�C���f�b�N�X�̍ő�l���X�V
		_groupIndexMax++;
	}
	// ���ɓ����O���[�v�̃A�j���[�V����������ꍇ�̓J�E���g�A�b�v
	else
	{
		_animNum[index]++;
	}
}

void Animation::SetAnimIndex(int index)
{
	if(0 <= index && index <= _groupIndexMax)
	{
		int oldIndex = _groupIndex;
		_groupIndex = index;
		if (oldIndex != _groupIndex) {
			_animCnt = 0;
			_bAnimEnd = false;

			ChangeRandomIndex();
		}
	}
}

void Animation::ChangeRandomIndex()
{
	if (_animNum.at(_groupIndex) > 1)
	{
		_randomIndex = rand() % _animNum.at(_groupIndex);
	}
}

void Animation::SetAnimCnt(int cnt)
{
	AnimationInfo* anim = _animInfo.at(_groupIndex).at(_randomIndex);
	_animCnt = MyMath::Clamp(0, anim->_framePerSheet * anim->_tblNum, cnt);
	_bAnimEnd = false;
}

void Animation::SetAnimCntForNumOfAnim(int cnt)
{
	AnimationInfo* anim = _animInfo.at(_groupIndex).at(_randomIndex);
	_animCnt = MyMath::Clamp(0, anim->_framePerSheet * anim->_tblNum, cnt * anim->_framePerSheet);
	_bAnimEnd = false;
}

int Animation::GetGraphHandle()
{
	try
	{
		AnimationInfo* anim = _animInfo.at(_groupIndex).at(_randomIndex);

		if (_groupIndexMax < _groupIndex ||
			anim->_framePerSheet == 0 || 
			anim->_tblNum == 0)
		{
			return -1;
		}

		return anim->_graphHandle.at(anim->_drawTbl.at(_animCnt / anim->_framePerSheet) % anim->_tblNum);
	}
	catch (const std::exception&)
	{
		return -1;
	}
	
}
