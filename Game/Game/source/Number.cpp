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

	//�����̎Z�o
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
		//�G���[����
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

	// �`�悷��摜��4�̒��_���W
	Vector2 _pos[4] = {
		// �ʏ�p�i���ォ��E���j
		{-_width + _numberWidth / 2.f, -_height / 2.f},	// ����
		{_numberWidth / 2.f, -_height / 2.f },			// �E��
		{_numberWidth / 2.f,  _height / 2.f },			// �E��
		{-_width + _numberWidth / 2.f,  _height / 2.f}	// ����
	};

	if (_isTurn) {
		// ���]�p
		Vector2 posTurn[4] = {
			// ���]�p�i�E�ォ�獶���j
		{_width + _numberWidth / 2.f, -_height / 2.f},	// ����
		{-_numberWidth / 2.f, -_height / 2.f },			// �E��
		{-_numberWidth / 2.f,  _height / 2.f },			// �E��
		{_width + _numberWidth / 2.f,  _height / 2.f}	// ����
		};
		for (int i = 0; i < 4; i++) {
			_pos[i] = posTurn[i];
		}
	}

	//��ʃT�C�Y�ɍ��킹��
	{
		auto inst = ApplicationBase::GetInstance();

		float scaleW = inst->DispSizeW() / 1920.f;
		float scaleH = inst->DispSizeH() / 1080.f;

		m = m * Matrix3::CreateScale(scaleW, scaleH);
	}

	// 4�̒��_���W�S�Ăɍs����|���ĕϊ�����
	for (int i = 0; i < 4; i++) {
		_pos[i] = Vector2::Transform(_pos[i], m);
	}

	return std::array<Vector2, 4>{
		_pos[0],	//����
		_pos[1],	//�E��
		_pos[2],	//�E��
		_pos[3]	//����
	};
}

std::array<Vector2, 4>Number::GetRightAligVertexes()
{
	auto m = GetParent() ? _transform.CreateMatrix3() * GetParent()->GetWorldMatrix() : _transform.CreateMatrix3();

	// �`�悷��摜��4�̒��_���W
	Vector2 _pos[4] = {
		// �ʏ�p�i���ォ��E���j
		{-_numberWidth / 2.f, -_height / 2.f},	// ����
		{_width + _numberWidth / 2.f, -_height / 2.f },			// �E��
		{_width + _numberWidth / 2.f,  _height / 2.f },			// �E��
		{-_numberWidth / 2.f,  _height / 2.f}	// ����
	};

	if (_isTurn) {
		// ���]�p
		Vector2 posTurn[4] = {
			// ���]�p�i�E�ォ�獶���j
		{_numberWidth / 2.f, -_height / 2.f},	// ����
		{-_width + _numberWidth / 2.f, -_height / 2.f },			// �E��
		{-_width + _numberWidth / 2.f,  _height / 2.f },			// �E��
		{_numberWidth / 2.f,  _height / 2.f}	// ����
		};
		for (int i = 0; i < 4; i++) {
			_pos[i] = posTurn[i];
		}
	}

	//��ʃT�C�Y�ɍ��킹��
	{
		auto inst = ApplicationBase::GetInstance();

		float scaleW = inst->DispSizeW() / 1920.f;
		float scaleH = inst->DispSizeH() / 1080.f;

		m = m * Matrix3::CreateScale(scaleW, scaleH);
	}

	// 4�̒��_���W�S�Ăɍs����|���ĕϊ�����
	for (int i = 0; i < 4; i++) {
		_pos[i] = Vector2::Transform(_pos[i], m);
	}

	return std::array<Vector2, 4>{
		_pos[0],	//����
		_pos[1],	//�E��
		_pos[2],	//�E��
		_pos[3]	//����
	};
}