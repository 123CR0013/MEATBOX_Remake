#include"Clip.h"
#include"UIScreen.h"
#include"Empty.h"
#include"../Application/ApplicationBase.h"
#include"../Mode/ModeServer.h"

Clip::Clip(class UIScreen* owner,unsigned int order)
	:UI(owner,order)
	,_child(new Empty(owner,order - 1))
{
	_screenHandle = MakeScreen(1920, 1080,TRUE);
	_clipHandle = MakeScreen(1920, 1080,FALSE);
	_blendHandle = MakeScreen(1920, 1080,TRUE);
	_child->RegistParent(this);
}

Clip::~Clip()
{
	DeleteGraph(_screenHandle);
	DeleteGraph(_clipHandle);
	DeleteGraph(_blendHandle);

	for (auto&& gChild : _grandChildren)
	{
		delete gChild;
	}
}

void Clip::AddChild(UI* ptr)
{
	ptr->GetOwner()->RemoveUI(ptr);
	ptr->RegistParent(_child);
	_grandChildren.emplace_back(ptr);

	for (size_t i = 0; i < ptr->GetChildren().size(); ++i)
	{
		auto child = ptr->GetChildren()[i];
		child->GetOwner()->RemoveUI(child);
		_grandChildren.emplace_back(child);
	}

}

void Clip::Process()
{
	UI::Process();

	bool isOnMouse = OnMouse();

	if(OnMouse()){
		auto pos = _child->GetLocation();

		pos.y -= GetMouseWheelRotVolF() * 10.f;

		_child->SetLocation(pos);
	}


	for (auto&& child : _grandChildren)
	{
		child->SetForceFalseInputMouse(isOnMouse);
		child->Process();
	}

}

void Clip::Draw()
{
	SetDrawScreen(_screenHandle);
	ClearDrawScreen();

	for (auto&& child : _grandChildren)
	{
		child->Draw();
	}

	//�؂蔲�����߂̐}�`��`��
	SetDrawScreen(_clipHandle);
	ClearDrawScreen();
	{
		auto _pos = GetVertexes();
		auto whiteColor = GetColor(255, 255, 255);

		DrawTriangle(
			static_cast<int>(_pos[0].x), static_cast<int>(_pos[0].y),	// ����
			static_cast<int>(_pos[1].x), static_cast<int>(_pos[1].y),	// �E��
			static_cast<int>(_pos[3].x), static_cast<int>(_pos[3].y),	// ����
			whiteColor, TRUE
		);

		DrawTriangle(
			static_cast<int>(_pos[1].x), static_cast<int>(_pos[1].y),	// �E��
			static_cast<int>(_pos[3].x), static_cast<int>(_pos[3].y),	// ����
			static_cast<int>(_pos[2].x), static_cast<int>(_pos[2].y),	// �E��
			whiteColor, TRUE
		);
	}

	//�؂蔲��
	GraphBlendBlt(_screenHandle, _clipHandle, _blendHandle, 255,
		DX_GRAPH_BLEND_RGBA_SELECT_MIX,
		DX_RGBA_SELECT_SRC_R,
		DX_RGBA_SELECT_SRC_G,
		DX_RGBA_SELECT_SRC_B,
		DX_RGBA_SELECT_BLEND_R
	);

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);
	
	DrawGraph(0, 0, _blendHandle, TRUE);
}