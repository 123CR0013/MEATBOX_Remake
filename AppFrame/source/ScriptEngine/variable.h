#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "RevPolishCalc.h"

// スクリプト変数単体
class VariableItem {
public:
	VariableItem() {}
	VariableItem(double num) {
		_type = TYPE::NUM;
		_num = num;
	}
	VariableItem(std::string str) {
		_type = TYPE::STRING;
		_string = str;
	}
	VariableItem(std::vector<VariableItem>* array) {
		_type = TYPE::ARRAY;
		_array = array;
	}
	virtual ~VariableItem();

	enum class TYPE {
		NONE,
		NUM,
		STRING,
		ARRAY,
	};
	TYPE GetType() { return _type; }
	void	clear();
	RevPolishCalc::ItemType		GetNum();
	std::string					GetString();
	std::vector<VariableItem>* GetArray();
public:
	TYPE			_type = TYPE::NONE;
	RevPolishCalc::ItemType		_num = 0;
	std::string		_string = "";
	std::vector<VariableItem>* _array = NULL;
};

// スクリプト変数全体管理
class Variable {
public:
	Variable();
	virtual ~Variable();
	void Clear();

	int		PickupVariableName(const char* p, std::string& str);

	bool	Is(std::string varName);
	bool	IsArray(std::string& varName, std::string& varArray);
	std::vector<int>	GetArrayPos(std::string varArray);
	VariableItem* GetArrayItem(std::string varName, std::string varArray, bool set);

	VariableItem::TYPE GetType(std::string varName);
	bool	Erace(std::string varName);
	bool	Set(std::string varName, RevPolishCalc::ItemType num);
	bool	Set(std::string varName, std::string str);
	RevPolishCalc::ItemType	GetNum(std::string varName);
	std::string	GetString(std::string varName);

private:
	std::unordered_map<std::string, VariableItem>	_map;
};
