#pragma once
#include<string>


class TextData
{
public:

	enum Type
	{
		Empty,
		Int,
		Float,
		Double,
		Bool,
		String
	};

	TextData();
	TextData(std::string data);
	TextData(const TextData& textData);
	TextData(bool value);

	template <typename T>
	TextData(T value)
		:_data(std::to_string(value))
	{
		_type = Analize();
	}

	void operator = (const TextData& right) { _data = right._data; _type = right._type; }
	void operator = (int right) { _data = std::to_string(right); }
	void operator = (float right) { _data = std::to_string(right); }
	void operator = (double right) { _data = std::to_string(right); }
	void operator = (const std::string& right) { _data = right; }
	void operator = (bool right) { _data = right ? "TRUE" : "FALSE"; }

	friend TextData		operator + (const TextData& left,const TextData& right) { return left._data + right._data; }
	friend int			operator + (TextData& left,int right) { return left.GetInt() + right; }
	friend float		operator + (TextData& left,float right) { return left.GetFloat() + right; }
	friend double		operator + (TextData& left, double right) { return left.GetDouble() + right; }
	friend std::string	operator + (TextData& left,const std::string& right) { return left._data + right; }

	friend int			operator + (int left, const TextData& right) { return left + right.GetInt(); }
	friend float		operator + (float left, const TextData& right) { return left + right.GetFloat(); }
	friend double		operator + (double left, const TextData& right) { return left + right.GetDouble(); }
	friend std::string	operator + (const std::string& left, const TextData& right) { return left + right._data; }

	friend int			operator - (TextData& left, int right) { return left.GetInt() - right; }
	friend float		operator - (TextData& left, float right) { return left.GetFloat() - right; }
	friend double		operator - (TextData& left, double right) { return left.GetDouble() - right; }

	friend int			operator - (int left, const TextData& right) { return left - right.GetInt(); }
	friend float		operator - (float left, const TextData& right) { return left - right.GetFloat(); }
	friend double		operator - (double left, const TextData& right) { return left - right.GetDouble(); }

	void operator += (TextData& right) { *this = *this + right; }
	void operator += (int right) { *this = *this + right; }
	void operator += (float right) { *this = *this + right; }
	void operator += (double right) { *this = *this + right; }
	void operator += (const std::string& right) { *this = *this + right; }

	void operator -= (int right) { *this = *this - right; }
	void operator -= (float right) { *this = *this - right; }
	void operator -= (double right) { *this = *this - right; }

	bool operator != (TextData& right)
	{
		return this != &right;
	}

	//ÉfÅ[É^ÇÃéÌóﬁÇÃâêÕ
	Type Analize();

	//ïœä∑
	int GetInt()const { return std::stoi(_data); }
	float GetFloat()const { return std::stof(_data); }
	double GetDouble()const { return std::stod(_data); }
	bool GetBool()const { return _data == "TRUE"; }
	std::string GetStr()const { return _data; }
	Type GetType()const { return _type; }

	std::string _data;
	Type _type;
};
