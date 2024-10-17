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
	_clipHandle = MakeScreen(1920, 1080,TRUE);
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


	for (auto&& grandChild : _grandChildren)
	{
		grandChild->SetForceFalseInputMouse(isOnMouse);
		grandChild->Process();
	}

}

void Clip::Draw()
{
	SetDrawScreen(_screenHandle);
	ClearDrawScreen();

	for (auto&& grandChild : _grandChildren)
	{
		grandChild->Draw();
	}

	//切り抜くための図形を描画
	SetDrawScreen(_clipHandle);
	ClearDrawScreen();
	{
		auto _pos = GetVertexes();
		auto whiteColor = GetColor(255, 255, 255);

		DrawTriangle(
			static_cast<int>(_pos[0].x), static_cast<int>(_pos[0].y),	// 左上
			static_cast<int>(_pos[1].x), static_cast<int>(_pos[1].y),	// 右上
			static_cast<int>(_pos[3].x), static_cast<int>(_pos[3].y),	// 左下
			whiteColor, TRUE
		);

		DrawTriangle(
			static_cast<int>(_pos[1].x), static_cast<int>(_pos[1].y),	// 右上
			static_cast<int>(_pos[3].x), static_cast<int>(_pos[3].y),	// 左下
			static_cast<int>(_pos[2].x), static_cast<int>(_pos[2].y),	// 右下
			whiteColor, TRUE
		);
	}

	//切り抜き
	//ただ、_screenHandleで描画されていない部分が切り抜かれれば、そこにはアルファチャンネルがないので黒く描画される
	 GraphBlendBlt(_screenHandle, _clipHandle, _blendHandle, 255,
		DX_GRAPH_BLEND_RGBA_SELECT_MIX,
		DX_RGBA_SELECT_SRC_R,
		DX_RGBA_SELECT_SRC_G,
		DX_RGBA_SELECT_SRC_B,
		DX_RGBA_SELECT_BLEND_A
	);

	//上記の問題は、出力結果である_brendHandleと_screenHandleのアルファ値の乗算(AND)をすれば、解決する
	GraphBlend(_blendHandle, _screenHandle, 255, DX_GRAPH_BLEND_MULTIPLE_A_ONLY);

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, _blendHandle, TRUE);
}