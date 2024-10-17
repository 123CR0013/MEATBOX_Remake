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
	// デフォルトの描画サイズはマップチップサイズと同じ
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
	// エラーチェック

	// 描画順テーブルが空の場合はデフォルトの描画順を設定
	if(animInfo->_drawTbl.size() == 0)
	{
		int graphNum = animInfo->_graphHandle.size();
		for(int i = 0; i < graphNum; i++)
		{
			animInfo->_drawTbl.push_back(i);
		}
	}

	// テーブルの要素数を取得
	animInfo->_tblNum = animInfo->_drawTbl.size();

	// 1枚あたりの表示フレーム数が0の場合は1を設定
	if(animInfo->_framePerSheet == 0)
	{
		animInfo->_framePerSheet = 1;
	}

	// --------------------------------------------------
	
	// アニメーション情報を追加
	this->_animInfo[index].push_back(animInfo);
	// 同じグループのアニメーションの数をカウント
	// 初めてのグループの場合は1を設定
	if (_animNum.find(index) == _animNum.end())
	{
		_animNum[index] = 1;
		// グループインデックスの最大値を更新
		_groupIndexMax++;
	}
	// 既に同じグループのアニメーションがある場合はカウントアップ
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
