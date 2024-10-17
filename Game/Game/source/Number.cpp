#include"Number.h"

Number::Number(UIScreen* owner,const std::string& fileName,UINT order)
	:UI(owner,order)
	,_number(0)
	,_numberWidth(50.f)
	,_numberHeight(50.f)
	,_format(Format::kCenter)
{
	for (int i = 0; i < 10; ++i)
	{
		_numHandles[i] = ResourceServer::LoadGraph(fileName + std::to_string(i) + ".png");
	}

	int width, height;
	GetGraphSize(_numHandles[0], &width, &height);
	_numberWidth = static_cast<float>(width);
	_numberHeight = static_cast<float>(height);
}

Number::~Number(){}


void Number::Draw()
{
	
	UINT digit = 1;

	//桁数の算出
	{
		UINT power10 = 10;

		while (_number / power10 != 0)
		{
			power10 *= 10;
			digit++;
		}
	}

	int number = _number;

	_width = _numberWidth * static_cast<float>(digit);
	_height = _numberHeight;

	std::array<Vector2, 4>pos;

	switch (_format)
	{
	case Format::kCenter:
		pos = GetVertexes();
		break;
	case Format::kLeft:
		pos = GetLeftAligVertexes();
		break;
	case Format::kRight:
		pos = GetRightAligVertexes();
			break;
	default :
		//エラー処理
		break;
	}

	Vector2 diff = (pos[1] - pos[0]) / static_cast<float>(digit);

	const int max = digit - 1;

	for (int i = max; i >= 0; --i)
	{
		int power10 = std::pow(10, i);
		int num = number / power10;
		number -= num * power10;

		DrawModiGraphF
		(
			(pos[0] + diff * static_cast<float>(max - i)).x, (pos[0] + diff * static_cast<float>(max - i)).y,
			(pos[0] + diff * static_cast<float>(max - i + 1)).x, (pos[0] + diff * static_cast<float>(max - i + 1)).y,
			(pos[3] + diff * static_cast<float>(max - i + 1)).x, (pos[3] + diff * static_cast<float>(max - i + 1)).y,
			(pos[3] + diff * static_cast<float>(max - i)).x, (pos[3] + diff * static_cast<float>(max - i)).y,
			_numHandles[num],
			TRUE
		);
	}
}

std::array<Vector2, 4>Number::GetLeftAligVertexes()
{
	auto m = GetParent() ? _transform.CreateMatrix3() * GetParent()->GetWorldMatrix() : _transform.CreateMatrix3();

	// 描画する画像の4つの頂点座標
	Vector2 _pos[4] = {
		// 通常用（左上から右回り）
		{-_width + _numberWidth / 2.f, -_height / 2.f},	// 左上
		{_numberWidth / 2.f, -_height / 2.f },			// 右上
		{_numberWidth / 2.f,  _height / 2.f },			// 右下
		{-_width + _numberWidth / 2.f,  _height / 2.f}	// 左下
	};

	if (_isTurn) {
		// 反転用
		Vector2 posTurn[4] = {
			// 反転用（右上から左回り）
		{_width + _numberWidth / 2.f, -_height / 2.f},	// 左上
		{-_numberWidth / 2.f, -_height / 2.f },			// 右上
		{-_numberWidth / 2.f,  _height / 2.f },			// 右下
		{_width + _numberWidth / 2.f,  _height / 2.f}	// 左下
		};
		for (int i = 0; i < 4; i++) {
			_pos[i] = posTurn[i];
		}
	}

	//画面サイズに合わせる
	{
		auto inst = ApplicationBase::GetInstance();

		float scaleW = inst->DispSizeW() / 1920.f;
		float scaleH = inst->DispSizeH() / 1080.f;

		m = m * Matrix3::CreateScale(scaleW, scaleH);
	}

	// 4つの頂点座標全てに行列を掛けて変換する
	for (int i = 0; i < 4; i++) {
		_pos[i] = Vector2::Transform(_pos[i], m);
	}

	return std::array<Vector2, 4>{
		_pos[0],	//左上
		_pos[1],	//右上
		_pos[2],	//右下
		_pos[3]	//左下
	};
}

std::array<Vector2, 4>Number::GetRightAligVertexes()
{
	auto m = GetParent() ? _transform.CreateMatrix3() * GetParent()->GetWorldMatrix() : _transform.CreateMatrix3();

	// 描画する画像の4つの頂点座標
	Vector2 _pos[4] = {
		// 通常用（左上から右回り）
		{-_numberWidth / 2.f, -_height / 2.f},	// 左上
		{_width + _numberWidth / 2.f, -_height / 2.f },			// 右上
		{_width + _numberWidth / 2.f,  _height / 2.f },			// 右下
		{-_numberWidth / 2.f,  _height / 2.f}	// 左下
	};

	if (_isTurn) {
		// 反転用
		Vector2 posTurn[4] = {
			// 反転用（右上から左回り）
		{_numberWidth / 2.f, -_height / 2.f},	// 左上
		{-_width + _numberWidth / 2.f, -_height / 2.f },			// 右上
		{-_width + _numberWidth / 2.f,  _height / 2.f },			// 右下
		{_numberWidth / 2.f,  _height / 2.f}	// 左下
		};
		for (int i = 0; i < 4; i++) {
			_pos[i] = posTurn[i];
		}
	}

	//画面サイズに合わせる
	{
		auto inst = ApplicationBase::GetInstance();

		float scaleW = inst->DispSizeW() / 1920.f;
		float scaleH = inst->DispSizeH() / 1080.f;

		m = m * Matrix3::CreateScale(scaleW, scaleH);
	}

	// 4つの頂点座標全てに行列を掛けて変換する
	for (int i = 0; i < 4; i++) {
		_pos[i] = Vector2::Transform(_pos[i], m);
	}

	return std::array<Vector2, 4>{
		_pos[0],	//左上
		_pos[1],	//右上
		_pos[2],	//右下
		_pos[3]	//左下
	};
}