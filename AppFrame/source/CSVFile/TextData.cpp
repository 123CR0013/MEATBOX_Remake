#include"TextData.h"
#include"../ScriptEngine/StringUtil.h"

TextData::TextData(){}

TextData::TextData(std::string data)
	:_data(data)
{
	_type = Analize();
}

TextData::TextData(const TextData& textData)
	:_data(textData._data)
	,_type(textData._type)
{

}

TextData::TextData(bool value)
	:_data(value ? "TRUE" : "FALSE")
{
	_type = Analize();
}


TextData::Type TextData::Analize()
{
	//��
	if (_data.length() == 0)
	{
		return Type::Empty;
	}

	//Bool
	if (_data == "TRUE" || _data == "FALSE")
	{
		return Type::Bool;
	}

	Type type = Type::Int;			//�Ԃ�l

	for (size_t pos = 0; pos < _data.length();++pos)
	{
		int size = StringUtility::Check(_data[pos]);

		//�啶���������ꍇ��String�Ɣ��f
		if (size > 1)
		{
			return Type::String;
		}

		if (pos == 0 && _data[pos] == '-' || _data[pos] == '+')
		{
			continue;
		}

		//���ł�Double�Ɣ��肳�ꂢ����A�u.�v�������Ƃ�������

		else if (_data.length() > 1 && _data[pos] == '.' && type != Type::Double)
		{
			type = Type::Double;
		}
		else if (pos == _data.length() - 1 && _data[pos] == 'f' && type == Type::Double)
		{
			type = Float;
		}
		//�����łȂ���΁A������Ɣ��f
		else if (!('0' <= _data[pos] && _data[pos] <= '9'))
		{
			return Type::String;
		}
	}

	return type;
}

