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
	//空
	if (_data.length() == 0)
	{
		return Type::Empty;
	}

	//Bool
	if (_data == "TRUE" || _data == "FALSE")
	{
		return Type::Bool;
	}

	Type type = Type::Int;			//返り値

	for (size_t pos = 0; pos < _data.length();++pos)
	{
		int size = StringUtility::Check(_data[pos]);

		//大文字だった場合はStringと判断
		if (size > 1)
		{
			return Type::String;
		}

		if (pos == 0 && _data[pos] == '-' || _data[pos] == '+')
		{
			continue;
		}

		//すでにDoubleと判定されいたら、「.」が二つあるということ

		else if (_data.length() > 1 && _data[pos] == '.' && type != Type::Double)
		{
			type = Type::Double;
		}
		else if (pos == _data.length() - 1 && _data[pos] == 'f' && type == Type::Double)
		{
			type = Float;
		}
		//数字でなければ、文字列と判断
		else if (!('0' <= _data[pos] && _data[pos] <= '9'))
		{
			return Type::String;
		}
	}

	return type;
}

